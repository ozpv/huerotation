#pragma once

#include "config.h"

typedef struct {
	HDC screenDC; /* desktop window handle */
	int screenWidth; /* desktop width */
	int screenHeight; /* desktop height */
} METRICS;

typedef struct {
	BYTE Red; /* Red value 0-255 */
	BYTE Green; /* Green value 0-255 */
	BYTE Blue; /* Blue value 0-255 */
#ifdef _WIN32
	BYTE Reserved; /* Must be set to 0 when using WinAPI */
#endif
} RGB;

#ifdef USE_FLOAT_CONVERSION
typedef struct {
	float Hue; /* hue in degrees */
	float Saturation; /* saturation multiplier */
	float Lightness; /* lightness multiplier */
} HSL;

HSL fRGBToHSL(BYTE Red, BYTE Green, BYTE Blue);
RGB fHSLToRGB(float Hue, float Saturation, float Lightness);
RGB fHSLToRGBAlt(float Hue, float Saturation, float Lightness);

#define RGBToHSL fRGBToHSL
#ifdef USE_ALT_TO_RGB
#define HSLToRGB fHSLToRGBAlt
#else
#define HSLToRGB fHSLToRGB
#endif
#else
typedef struct {
	double Hue; /* hue in degrees */
	double Saturation; /* saturation multiplier */
	double Lightness; /* lightness multiplier */
} HSL;

HSL dRGBToHSL(BYTE Red, BYTE Green, BYTE Blue);
RGB dHSLToRGB(double Hue, double Saturation, double Lightness);
RGB dHSLToRGBAlt(double Hue, double Saturation, double Lightness);

#define RGBToHSL dRGBToHSL
#ifdef USE_ALT_TO_RGB
#define HSLToRGB dHSLToRGBAlt
#else
#define HSLToRGB dHSLToRGB
#endif
#endif