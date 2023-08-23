#pragma once

/*
 * define USE_FLOAT_CONVERSION to convert using float values
 */

//#define USE_FLOAT_CONVERSION

/*
 * define USE_ALT_TO_RGB to convert HSL to RGB using an alternative calculation
 */

//#define USE_ALT_TO_RGB

/* so the compiler doesn't throw a bajillion errors if you are compiling on unix */
#ifndef _WIN32
#define BYTE unsigned char
#endif