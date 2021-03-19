#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int r;
	int g;
	int b;
}RGB;

typedef struct
{
	int h;
	int s;
	int b;
}HSB;

HSB RGBtoHSB(RGB*);
RGB HSBtoRGB(HSB*);
