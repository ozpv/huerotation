#ifdef _WIN32
#include <windows.h>
#else
#define BYTE unsigned char
#endif

#include "config.h"
#include "huerotation.h"

#ifdef USE_INLINE_CALCULATIONS
double isin(double x) {
	__asm {
		fld x
		fsin
	}
}

double icos(double x) {
	__asm {
		fld x
		fcos
	}
}

double isqrt(double x) {
	__asm {
		fld x
		fsqrt
	}
}

#define M_PI 3.14159265358979323846
#define sin isin
#define cos icos
#define sqrt isqrt
#else
#include <math.h>
#endif

/* bound to [0, 255] */
#define bound(v) (v < 0x00) ? 0x00 : (v > 0xFF) ? 0xFF : (BYTE)v;
#define radians(degree) degree * M_PI / 180

typedef struct {
	HSL hsl;
	METRICS metrics;
} MatrixRotation;

DWORD WINAPI MatrixHueRotation(LPVOID lParam) {
	MatrixRotation *matrixRotation = (MatrixRotation*)lParam;

	HDC screenDC = matrixRotation->metrics.screenDC;
	HDC compatibleDC = CreateCompatibleDC(screenDC);

	RGB *rgb;
	RGB rgbTemp;

	BITMAPINFO bitmapInfo = { sizeof(BITMAPINFOHEADER), matrixRotation->metrics.screenWidth, -matrixRotation->metrics.screenHeight, 1, 32, BI_RGB };

	HBITMAP hdib = CreateDIBSection(screenDC, &bitmapInfo, DIB_RGB_COLORS, (void**)&rgb, NULL, 0);

	if (hdib == NULL)
		return -1;

	SelectObject(compatibleDC, hdib);

	double cosA = matrixRotation->hsl.Saturation * cos(radians(matrixRotation->hsl.Hue));
	double sinA = matrixRotation->hsl.Saturation * sin(radians(matrixRotation->hsl.Hue));
	
	double matrix[3][3] = {
		{cosA + (1.0f - cosA) / 3.0f, 1.0f / 3.0f * (1.0f - cosA) - sqrt(1.0f / 3.0f) * sinA, 1.0f / 3.0f * (1.0f - cosA) + sqrt(1.0f / 3.0f) * sinA},
		{1.0f / 3.0f * (1.0f - cosA) + sqrt(1.0f / 3.0f) * sinA, cosA + 1.0f / 3.0f * (1.0f - cosA), 1.0f / 3.0f * (1.0f - cosA) - sqrt(1.0f / 3.0f) * sinA},
		{1.0f / 3.0f * (1.0f - cosA) - sqrt(1.0f / 3.0f) * sinA, 1.0f / 3.0f * (1.0f - cosA) + sqrt(1.0f / 3.0f) * sinA, cosA + 1.0f / 3.0f * (1.0f - cosA)}
	};

	while (TRUE) {
		BitBlt(compatibleDC, 0, 0, matrixRotation->metrics.screenWidth, matrixRotation->metrics.screenHeight, screenDC, 0, 0, SRCCOPY);

		for (int i = 0; i < matrixRotation->metrics.screenWidth * matrixRotation->metrics.screenHeight; i++) {
			rgbTemp.Red = (BYTE)bound(rgb[i].Red * matrix[0][0] + rgb[i].Green * matrix[0][1] + rgb[i].Blue * matrix[0][2] * matrixRotation->hsl.Lightness);
			rgbTemp.Green = (BYTE)bound(rgb[i].Red * matrix[1][0] + rgb[i].Green * matrix[1][1] + rgb[i].Blue * matrix[1][2] * matrixRotation->hsl.Lightness);
			rgbTemp.Blue = (BYTE)bound(rgb[i].Red * matrix[2][0] + rgb[i].Green * matrix[2][1] + rgb[i].Blue * matrix[2][2] * matrixRotation->hsl.Lightness);
			
			rgb[i] = rgbTemp;
		}
		BitBlt(screenDC, 0, 0, matrixRotation->metrics.screenWidth, matrixRotation->metrics.screenHeight, compatibleDC, 0, 0, SRCCOPY);

		Sleep(1);
	}

	return 0;
}

int proto(void) {
	MatrixRotation matrixRotation;

	matrixRotation.metrics.screenDC = GetDC(NULL);

	matrixRotation.metrics.screenWidth = GetSystemMetrics(SM_CXSCREEN);
	matrixRotation.metrics.screenHeight = GetSystemMetrics(SM_CYSCREEN);

	matrixRotation.hsl.Hue = 6.0;

	/* for this calculation, these values are no longer bound to [0, 1] */
	matrixRotation.hsl.Saturation = 1.0;
	matrixRotation.hsl.Lightness = 1.0;

	HANDLE handleThread;

	handleThread = CreateThread(NULL, 0, MatrixHueRotation, &matrixRotation, 0, NULL);

	Sleep(INFINITE);

	return 0;
}