# huerotation
Small hue to (and from) rgb calculation library written in C

For Windows users (XP or later), there is a fun real-time screen Hue Rotation effect
Just compile using your favorite compiler and run the program to see it! 

## Features:

One method of converting RGB to HSL values
Two methods of converting HSL to RGB values
An option to switch between float or double values for the conversion 

## How to use:

move "config.h", "conversion.c", and "huerotation.h" into your repository

# in "config.h":

define USE_FLOAT_CONVERSION to convert using float values:
```
#define USE_FLOAT_CONVERSION
```
define USE_ALT_TO_RGB to convert HSL to RGB using an alternative calculation:
```
#define USE_ALT_TO_RGB
```
