#include "config.h"

#define BYTE unsigned char

template<typename T>
inline T tmax(T a, T b) {
	return (a > b) ? a : b;
}

template<typename T>
inline T tmin(T a, T b) {
	return (a < b) ? a : b;
}

template<typename T>
inline T tmax3(T a, T b, T c) {
	return tmax<T>(tmax<T>(a, b), c);
}

template<typename T>
inline T tmin3(T a, T b, T c) {
	return tmin<T>(tmin<T>(a, b), c);
}

template<typename T>
class Color {
private:

	inline T HUEToCOLOR(T p, T q, T t) {
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

	/* lightweight float/double mod function */
	inline T tmod(T val, T mod) {
		if (val >= mod) {
			val -= mod;
			/* recursion to ensure the MOD value */
			tmod(val, mod);
		} else
			return val;
	}

public:

	struct HSL {
		T Hue; /* hue in degrees [0, 360] */
		T Saturation; /* saturation multiplier [0, 1] */
		T Lightness; /* lightness multiplier [0, 1] */
	};

	HSL hsl = { 0, 0, 0 };

	struct RGB {
		BYTE Red; /* Red value [0, 255] */
		BYTE Green; /* Green value [0, 255] */
		BYTE Blue; /* Blue value [0, 255] */
#ifdef _WIN32
		BYTE Reserved = 0; /* Must be set to 0 when using WinAPI */
#endif
	};

	RGB rgb = { 0, 0, 0 };

	void RGBToHSL(void) {
		/* scale our values to between [0, 1]
		 * cast our vaules to T to ensure the compiler interprets it correctly
		 */
		T red = static_cast<T>(rgb.Red) / 255.0;
		T green = static_cast<T>(rgb.Green) / 255.0;
		T blue = static_cast<T>(rgb.Blue) / 255.0;

		T max = tmax3<T>(red, green, blue);
		T min = tmin3<T>(red, green, blue);

		/* calculate chroma */
		T chroma = max - min;

		T hue;
		T saturation;

		/* calculate the lightness value by averaging max and min */
		T lightness = (max + min) / 2.0;

		/* color is achromatic and we don't need to calculate the hue */
		if (max == min) {
			hue = 0.0;
			saturation = 0.0;
		}
		else {
			/* calculate saturation */
			saturation = (lightness > 0.5) ? chroma / (2.0 - max - min) : chroma / (max + min);

			/* calculate our hue and apply appropriate shift towards M */
			if (max == red)
				/* modulus doesn't work in C on float / float numbers and I don't want to use fmod so we do a quick if else */
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
	}

	void HSLToRGB(void) {
#ifdef USE_ALT_TO_RGB
		T k;

		T a = hsl.Saturation * tmin<T>(hsl.Lightness, 1 - hsl.Lightness);

		/* to catch our RGB values */
		T rgbd[3];

		/* to calculate f(0) f(8) f(4) */
		T n[3] = { 0.0, 8.0, 4.0 };

		for (int i = 0; i < 3; i++) {
			k = (n[i] + hsl.Hue / 30.0);

			/* simple mod template */
			k = tmod(k, 12);

			rgbd[i] = hsl.Lightness - a * tmax<T>(-1.0, tmin3<T>(k - 3.0, 9.0 - k, 1.0));
		}

		rgb.Red = static_cast<BYTE>(rgbd[0] * 255.0);
		rgb.Green = static_cast<BYTE>(rgbd[1] * 255.0);
		rgb.Blue = static_cast<BYTE>(rgbd[2] * 255.0);
#else
		T q;
		T x;

		/* make sure the value is in range [0, 360] then find hue "prime" */
		T hue = static_cast<int>(hsl.Hue) % 360 / 360.0;

		T red = 0.0;
		T green = 0.0;
		T blue = 0.0;

		/* color is achromatic and doesn't need hue calculations */
		if (hsl.Saturation == 0.0) {
			red = hsl.Lightness;
			green = hsl.Lightness;
			blue = hsl.Lightness;
		} else {
			q = hsl.Lightness < 0.5 ? hsl.Lightness * (1.0 + hsl.Saturation) : hsl.Lightness + hsl.Saturation - hsl.Lightness * hsl.Saturation;
			x = 2.0 * hsl.Lightness - q;

			/* convert hue back into RGB color */
			red = HUEToCOLOR(x, q, hue + 1.0 / 3.0);
			green = HUEToCOLOR(x, q, hue);
			blue = HUEToCOLOR(x, q, hue - 1.0 / 3.0);
		}

		/* scale our values back up from [0, 1] */
		rgb.Red = static_cast<BYTE>(red * 255.0);
		rgb.Green = static_cast<BYTE>(green * 255.0);
		rgb.Blue = static_cast<BYTE>(blue * 255.0);
#endif
	}
};

#include <iostream>

int example(void) {
	Color<double> color;

	color.rgb = { 255, 78, 99 };

	color.RGBToHSL();

	std::cout << color.hsl.Hue << std::endl;
	std::cout << color.hsl.Saturation << std::endl;
	std::cout << color.hsl.Lightness << std::endl;

	color.HSLToRGB();

	std::cout << static_cast<int>(color.rgb.Red) << std::endl;
	std::cout << static_cast<int>(color.rgb.Green) << std::endl;
	std::cout << static_cast<int>(color.rgb.Blue) << std::endl;

	return 0;
}