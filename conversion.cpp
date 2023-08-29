#ifdef _WIN32
#include <windows.h>
#else 
#define BYTE unsigned char
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define USE_ALT_TO_RGB

template<class Tdecimal>
class ColorspaceConversion {
private:

	Tdecimal HUEToCOLOR(Tdecimal p, Tdecimal q, Tdecimal t) {
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
	Tdecimal tmod(Tdecimal val, Tdecimal mod) {
		if (val >= mod) {
			val -= mod;
			/* recursion to ensure the MOD value */
			tmod(val, mod);
		}
		else
			return val;
	}

public:

	struct HSL {
		Tdecimal H; /* hue in degrees [0, 360] */
		Tdecimal S; /* saturation multiplier [0, 1] */
		Tdecimal L; /* lightness multiplier [0, 1] */
	};

	HSL hsl = { 0 };

	struct RGB {
		BYTE R; /* Red value 0-255 */
		BYTE G; /* Green value 0-255 */
		BYTE B; /* Blue value 0-255 */
#ifdef _WIN32
		BYTE Reserved = 0; /* Must be set to 0 when using WinAPI */
#endif
	};

	RGB rgb = { 0 };

	RGB HSLToRGB(Tdecimal Hue, Tdecimal Saturation, Tdecimal Lightness) {
#ifdef USE_ALT_TO_RGB
		Tdecimal k;

		Tdecimal a = Saturation * min(Lightness, 1 - Lightness);

		/* to catch our RGB values */
		Tdecimal rgbd[3];

		/* to calculate f(0) f(8) f(4) */
		Tdecimal n[3] = { 0.0, 8.0, 4.0 };

		for (int i = 0; i < 3; i++) {
			k = (n[i] + Hue / 30.0);

			/* simple mod template */
			k = tmod(k, 12);

			rgbd[i] = Lightness - a * max(-1.0, min(min(k - 3.0, 9.0 - k), 1.0));
		}

		rgb.R = static_cast<BYTE>(rgbd[0] * 255.0);
		rgb.G = static_cast<BYTE>(rgbd[1] * 255.0);
		rgb.B = static_cast<BYTE>(rgbd[2] * 255.0);
#else
		Tdecimal q;
		Tdecimal x;

		/* make sure the value is in range [0, 360] then find hue "prime" */
		Tdecimal hue = (int)Hue % 360 / 360.0;

		Tdecimal red = 0.0;
		Tdecimal green = 0.0;
		Tdecimal blue = 0.0;

		/* color is achromatic and doesn't need hue calculations */
		if (Saturation == 0.0) {
			red = Lightness;
			green = Lightness;
			blue = Lightness;
		} else {
			q = Lightness < 0.5 ? Lightness * (1.0 + Saturation) : Lightness + Saturation - Lightness * Saturation;
			x = 2.0 * Lightness - q;

			/* convert hue back into RGB color */
			red = HUEToCOLOR(x, q, hue + 1.0 / 3.0);
			green = HUEToCOLOR(x, q, hue);
			blue = HUEToCOLOR(x, q, hue - 1.0 / 3.0);
		}

		/* scale our values back up from [0, 1] */
		rgb.R = static_cast<BYTE>(red * 255.0);
		rgb.G = static_cast<BYTE>(green * 255.0);
		rgb.B = static_cast<BYTE>(blue * 255.0);
#endif
		
		return rgb;
	}

	HSL RGBToHSL(BYTE Red, BYTE Green, BYTE Blue) {
		/* scale our values to between [0, 1]
		 * cast our vaules to Tdecimal to ensure the compiler interprets it correctly
		 */
		Tdecimal red = static_cast<Tdecimal>(Red) / 255.0;
		Tdecimal green = static_cast<Tdecimal>(Green) / 255.0;
		Tdecimal blue = static_cast<Tdecimal>(Blue) / 255.0;

		Tdecimal max = max(max(red, green), blue);
		Tdecimal min = min(min(red, green), blue);

		/* calculate chroma */
		Tdecimal chroma = max - min;

		Tdecimal hue;
		Tdecimal saturation;

		/* calculate the lightness value by averaging max and min */
		Tdecimal lightness = (max + min) / 2.0;

		/* color is achromatic and we don't need to calculate the hue */
		if (max == min) {
			hue = 0.0;
			saturation = 0.0;
		} else {
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
		hsl.H = hue * 60;
		hsl.S = saturation;
		hsl.L = lightness;

		return hsl;
	}
};

#include <iostream>

int example(void) {
	ColorspaceConversion<double> cc;

	cc.RGBToHSL(221, 32, 99);

	std::cout << cc.hsl.H << std::endl;
	std::cout << cc.hsl.S << std::endl;
	std::cout << cc.hsl.L << std::endl;

	cc.HSLToRGB(cc.hsl.H, cc.hsl.S, cc.hsl.L);

	std::cout << (int)cc.rgb.R << std::endl;
	std::cout << (int)cc.rgb.G << std::endl;
	std::cout << (int)cc.rgb.B << std::endl;

	return 0;
}
