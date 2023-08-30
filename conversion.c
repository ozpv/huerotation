#ifdef _WIN32
#include <windows.h>
#endif

#include "huerotation.h"

#ifdef USE_FLOAT_CONVERSION
HSL fRGBToHSL(BYTE Red, BYTE Green, BYTE Blue) {
	HSL hsl;

	/* scale our values to [0, 1] */
	float red = (float)Red / 255.0f;
	float green = (float)Green / 255.0f;
	float blue = (float)Blue / 255.0f;

	float max = max(max(red, green), blue);
	float min = min(min(red, green), blue);

	/* calculate chroma */
	float chroma = max - min;

	float hue;
	float saturation;

	/* calculate the lightness value by averaging max and min */
	float lightness = (max + min) / 2.0f;

	/* color is achromatic and we don't need to calculate the hue */
	if (max == min) {
		hue = 0.0f;
		saturation = 0.0f;
	}
	else {
		/* calculate saturation */
		saturation = (lightness > 0.5) ? chroma / (2.0f - max - min) : chroma / (max + min);

		/* calculate our hue and apply appropriate shift towards M */
		if (max == red)
			/* modulus doesn't work in C on double / float numbers and I don't want to use fmod so we do a quick if else */
			hue = (green - blue) / chroma + (green < blue ? 6.0f : 0.0f);
		else if (max == green)
			hue = (blue - red) / chroma + 2.0f;
		else if (max == blue)
			hue = (red - green) / chroma + 4.0f;
	}

	/* scale the hue from distance to distance in degrees by multiplying by 60 */
	hsl.Hue = hue * 60.0f;
	hsl.Saturation = saturation;
	hsl.Lightness = lightness;

	return hsl;
}

/* it's possible to integrate this into the function itself,
 * but seperating it makes it easier to understand
 */
float fHUEToCOLOR(float p, float q, float t) {
	if (t < 0.0f)
		t++;
	if (t > 1.0f)
		t--;
	if (1.0f / 6.0f > t)
		return p + (q - p) * 6.0f * t;
	if (1.0f / 2.0f > t)
		return q;
	if (2.0f / 3.0f > t)
		return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
	return p;
}

RGB fHSLToRGB(float Hue, float Saturation, float Lightness) {
	RGB rgb;

#ifdef USE_ALT_TO_RGB
	float k;

	float a = Saturation * min(Lightness, 1.0f - Lightness);

	/* to catch our RGB values */
	float rgbd[3];

	/* to calculate f(0) f(8) f(4) */
	float n[3] = { 0.0f, 8.0f, 4.0f };

	for (int i = 0; i < 3; i++) {
		k = (n[i] + Hue / 30.0f);

		/* you could also use fmod here instead to mod 12 the value */
		if (k >= 12.0f)
			k -= 12.0f;

		rgbd[i] = Lightness - a * max(-1.0f, min(min(k - 3.0f, 9.0f - k), 1.0f));
	}

	/* scale our values back up from [0, 1] */
	rgb.Red = (BYTE)(rgbd[0] * 255.0f);
	rgb.Green = (BYTE)(rgbd[1] * 255.0f);
	rgb.Blue = (BYTE)(rgbd[2] * 255.0f);
#else
	float q;
	float x;

	/* make sure the value is in range [0, 360] then find hue "prime" */
	float hue = (int)(Hue) % 360 / 360.0f;

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;

	/* color is achromatic and doesn't need hue calculations */
	if (Saturation == 0.0f) {
		red = Lightness;
		green = Lightness;
		blue = Lightness;
	}
	else {
		q = Lightness < 0.5 ? Lightness * (1.0f + Saturation) : Lightness + Saturation - Lightness * Saturation;
		x = 2.0f * Lightness - q;

		/* convert hue back into RGB color */
		red = dHUEToCOLOR(x, q, hue + 1.0f / 3.0f);
		green = dHUEToCOLOR(x, q, hue);
		blue = dHUEToCOLOR(x, q, hue - 1.0f / 3.0f);
	}

	/* scale our values back up from [0, 1] */
	rgb.Red = (BYTE)(red * 255.0f);
	rgb.Green = (BYTE)(green * 255.0f);
	rgb.Blue = (BYTE)(blue * 255.0f);
#endif

#ifdef _WIN32
	rgb.Reserved = 0;
#endif

	return rgb;
}
#else
HSL dRGBToHSL(BYTE Red, BYTE Green, BYTE Blue) {
	HSL hsl; 

	/* scale our values to [0, 1] */
	double red = (double)Red / 255.0;
	double green = (double)Green / 255.0;
	double blue = (double)Blue / 255.0;

	double max = max(max(red, green), blue);
	double min = min(min(red, green), blue);

	/* calculate chroma */
	double chroma = max - min;

	double hue;
	double saturation;

	/* calculate the lightness value by averaging max and min */
	double lightness = (max + min) / 2.0;

	/* color is achromatic and we don't need to calculate the hue */
	if (max == min) {
		hue = 0.0;
		saturation = 0.0;
	} else { 
	/* calculate saturation */
		saturation = (lightness > 0.5) ? chroma / (2.0 - max - min) : chroma / (max + min);

	/* calculate our hue and apply appropriate shift towards M */
		if (max == red)
	/* modulus doesn't work in C on double / float numbers and I don't want to use fmod so we do a quick if else */
			hue = (green - blue) / chroma + (green < blue ? 6.0 : 0.0); 
		else if (max == green)
			hue = (blue - red) / chroma + 2.0;
		else if (max == blue)
			hue = (red - green) / chroma + 4.0;
	}

	/* scale the hue from distance to distance in degrees by multiplying by 60 */
	hsl.Hue = hue * 60.0;
	hsl.Saturation = saturation;
	hsl.Lightness = lightness;

	return hsl;
}

/* it's possible to integrate this into the function itself,
 * but seperating it makes it easier to understand
 */
double dHUEToCOLOR(double p, double q, double t) {
	if (t < 0.0)
		t++;
	if (t > 1.0)
		t--;
	if (1.0 / 6.0 > t)
		return p + (q - p) * 6.0 * t;
	if (1.0 / 2.0 > t)
		return q;
	if (2.0 / 3.0 > t)
		return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
	return p;
}

RGB dHSLToRGB(double Hue, double Saturation, double Lightness) {
	RGB rgb;

#ifdef USE_ALT_TO_RGB
	double k;

	double a = Saturation * min(Lightness, 1.0 - Lightness);

	/* to catch our RGB values */
	double rgbd[3];

	/* to calculate f(0) f(8) f(4) */
	double n[3] = { 0.0, 8.0, 4.0 };

	for (int i = 0; i < 3; i++) {
		k = (n[i] + Hue / 30.0);

		/* you could also use fmod here instead to mod 12 the value */
		if (k >= 12.0)
			k -= 12.0;

		rgbd[i] = Lightness - a * max(-1.0, min(min(k - 3.0, 9.0 - k), 1.0));
	}

	/* scale our values back up from [0, 1] */
	rgb.Red = (BYTE)(rgbd[0] * 255.0);
	rgb.Green = (BYTE)(rgbd[1] * 255.0);
	rgb.Blue = (BYTE)(rgbd[2] * 255.0);
#else
	double q;
	double x;

	/* make sure the value is in range [0, 360] then find hue "prime" */
	double hue = (int)(Hue) % 360 / 360.0;

	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;

	/* color is achromatic and doesn't need hue calculations */
	if (Saturation == 0.0) {
		red = Lightness;
		green = Lightness;
		blue = Lightness;
	} else {
		q = Lightness < 0.5 ? Lightness * (1.0 + Saturation) : Lightness + Saturation - Lightness * Saturation;
		x = 2.0 * Lightness - q;

		/* convert hue back into RGB color */
		red = dHUEToCOLOR(x, q, hue + 1.0 / 3.0);
		green = dHUEToCOLOR(x, q, hue);
		blue = dHUEToCOLOR(x, q, hue - 1.0 / 3.0);
	}

	/* scale our values back up from [0, 1] */
	rgb.Red = (BYTE)(red * 255.0);
	rgb.Green = (BYTE)(green * 255.0);
	rgb.Blue = (BYTE)(blue * 255.0);
#endif

#ifdef _WIN32
	rgb.Reserved = 0;
#endif

	return rgb;
}
#endif