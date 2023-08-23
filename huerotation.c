#include <windows.h>

#include "huerotation.h"

DWORD WINAPI HueRotation(LPVOID lParam) {
	METRICS *metrics = (METRICS*)lParam;
	
	HDC screenDC = metrics->screenDC;
	HDC compatibleDC = CreateCompatibleDC(screenDC);
	
	RGB *rgb;
	HSL hsl;

	BITMAPINFO bitmapInfo = { sizeof(BITMAPINFOHEADER), metrics->screenWidth, -metrics->screenHeight, 1, 32, BI_RGB };

	HBITMAP hdib = CreateDIBSection(screenDC, &bitmapInfo, DIB_RGB_COLORS, (void**)&rgb, NULL, 0);

	if (hdib == NULL)
		return -1;

	SelectObject(compatibleDC, hdib);

	while (TRUE) {
		BitBlt(compatibleDC, 0, 0, metrics->screenWidth, metrics->screenHeight, screenDC, 0, 0, SRCCOPY);
		
		for (int i = 0; i < metrics->screenWidth * metrics->screenHeight; i++) {
			hsl = RGBToHSL(rgb[i].Red, rgb[i].Green, rgb[i].Blue);
			hsl.Hue += 6.0;
			rgb[i] = HSLToRGB(hsl.Hue, hsl.Saturation, hsl.Lightness);
		}
		BitBlt(screenDC, 0, 0, metrics->screenWidth, metrics->screenHeight, compatibleDC, 0, 0, SRCCOPY);
		
		Sleep(100);
	}
	return 0;
}

int main(void) {
	/* get screen dimensions and prepare to write our hue rotated images to the screen in real-time
	 * NOTE: there is no multi-monitor support and the effect will be drawn to the primary monitor
	 */
	METRICS metrics;
	metrics.screenDC = GetDC(NULL);

	metrics.screenWidth = GetSystemMetrics(SM_CXSCREEN);
	metrics.screenHeight = GetSystemMetrics(SM_CYSCREEN);

	/* launch our thread and pass our variables to test */
	HANDLE handleThread;

	handleThread = CreateThread(NULL, 0, HueRotation, &metrics, 0, NULL);

	/* Run in an infinite loop with no thread cleanup, just kill the process to end: taskkill /f /im huerotation.exe */
	Sleep(INFINITE);

	return 0;
}