#pragma once

#include "config.h"

/* so the compiler doesn't throw a bajillion errors if you are compiling on unix */
#ifndef _WIN32
#define BYTE unsigned char
#endif

typedef struct {
	HDC screenDC; /* screen device context */
	int screenWidth; /* screen width */
	int screenHeight; /* screen height */
} METRICS;

typedef struct {
	BYTE Red; /* Red value [0, 255] */
	BYTE Green; /* Green value [0, 255] */
	BYTE Blue; /* Blue value [0, 255] */
#ifdef _WIN32
	BYTE Reserved; /* Must be set to 0 when using WinAPI */
#endif
} RGB;

#ifdef USE_FLOAT_CONVERSION
typedef struct {
	float Hue; /* hue in degrees [0, 360] */
	float Saturation;  /* saturation multiplier [0, 1] */
	float Lightness; /* lightness multiplier [0, 1] */
} HSL;

HSL fRGBToHSL(BYTE Red, BYTE Green, BYTE Blue);
RGB fHSLToRGB(float Hue, float Saturation, float Lightness);

#define RGBToHSL fRGBToHSL
#define HSLToRGB fHSLToRGB
#else
typedef struct {
	double Hue; /* hue in degrees [0, 360] */
	double Saturation;  /* saturation multiplier [0, 1] */
	double Lightness; /* lightness multiplier [0, 1] */
} HSL;

HSL dRGBToHSL(BYTE Red, BYTE Green, BYTE Blue);
RGB dHSLToRGB(double Hue, double Saturation, double Lightness);

#define RGBToHSL dRGBToHSL
#define HSLToRGB dHSLToRGB
#endif