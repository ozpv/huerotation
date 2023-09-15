# huerotation
Small hue to (and from) rgb calculation library written in C & C++

For Windows users (XP or later), there is a fun real-time screen Hue Rotation effect
Just compile using your favorite compiler and run the program to see it! 

Please note that it uses WinGDI so it isn't going to be the fastest, and there is no multi-monitor support
It will assume the primary monitor which you have selected

## Features:

One method of converting RGB to HSL values
Two methods of converting HSL to RGB values
An option to switch between float or double values for the conversion 

There are varying levels of accuracy in each of the HSL to RGB methods
In my experience, the alternative method is more accurate

## How to use (C):

move "config.h", "conversion.c", and "huerotation.h" into your repository

```
RGBToHSL
```
returns struct HSL

```
HSLToRGB
```
returns struct RGB

though, you could implement pointers so that:

```
void RGBToHSL(..., double *h, double *s, double *l);
```
would assign specific values to an address

but I'll let you figure that out if you so wish

## How to use (C++):

structures HSL and RGB are initialized with values 0

you can assign to the structures quite cleverly like in 
```
int example(void); 
```

otherwise:
```
Color<double> color;

/* Color<float> color;
 * for floats
 */

color.hsl.Hue = 360.0;
color.hsl.Saturation = 0.5;
color.hsl.Lightness = 0.5;
```

Usage:
```
color.HSLToRGB();

now we have converted HSL to RGB!
```

# in "config.h":

define USE_FLOAT_CONVERSION to convert using float values:
```
#define USE_FLOAT_CONVERSION
```

define USE_ALT_TO_RGB to convert HSL to RGB using an alternative calculation:
```
#define USE_ALT_TO_RGB
```

# matrix.c
Another way to rotate hue that varies in levels of accuracy
