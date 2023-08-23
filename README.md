# huerotation
Small hue to (and from) rgb calculation library written in C

For Windows users (XP or later), there is a fun real-time screen Hue Rotation effect
Just compile using your favorite compiler and run the program to see it! 

Please note that it uses WinGDI so it isn't going to be the fastest, and there is no multi-monitor support
It will assume the primary monitor which you have selected

## Features:

One method of converting RGB to HSL values
Two methods of converting HSL to RGB values
An option to switch between float or double values for the conversion 

## How to use:

move "config.h", "conversion.c", and "huerotation.h" into your repository

RGBToHSL(r, g, b) returns struct HSL
HSLToRGB(h, s, l) returns struct RGB

though you could implement pointers so that:

void RGBToHSL(..., double *h, double *s, double *l);

would assign specific values

but I'll let you figure that out if you so wish

# in "config.h":

define USE_FLOAT_CONVERSION to convert using float values:
```
#define USE_FLOAT_CONVERSION
```
define USE_ALT_TO_RGB to convert HSL to RGB using an alternative calculation:
```
#define USE_ALT_TO_RGB
```
