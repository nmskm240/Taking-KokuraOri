#include "colorConvert.h"

HSB RGBtoHSB(RGB* color)
{
	HSB hsb;
	int max, min, diff, count = 0;

	printf("converter in(r:%d g:%d b:%d)\n", color->r, color->g, color->b);

	max = Max(color->r, color->g, color->b);
	min = Min(color->r, color->g, color->b);

	diff = max - min;

	if (diff == 0)
	{
		hsb.h = 0;
	}
	else if (color->r == max)
	{
		//H = 60 ~ ((G - B) € (MAX - MIN))
		hsb.h = 60 * ((color->g - color->b) / diff);
	}
	else if (color->g == max)
	{
		//H = 60 ~ ((B - R) € (MAX - MIN)) +120
		hsb.h = 60 * ((color->b - color->r) / diff) + 120;
	}
	else
	{
		//H = 60 ~ ((R - G) € (MAX - MIN)) +240
		hsb.h = 60 * ((color->r - color->g) / diff) + 240;
	}

	if (hsb.h < 0)
	{
		hsb.h += 360;
	}

	hsb.s = (max == 0) ? 0 : (diff / max) * 100;
	hsb.b = (max / 255) * 100;

	printf("max:%d min:%d diff:%d\n", max, min, diff);
	printf("converter out(h:%d s:%d b:%d)\n", hsb.h, hsb.s, hsb.b);

	return hsb;
}

RGB HSBtoRGB(HSB* color)
{
	RGB rgb;
	int max, min, diff;

	max = color->b;
	min = max - ((color->s / 100) * max);
	diff = max - min;

	if (0 <= color->h && color->h < 60)
	{
		rgb.r = max;
		rgb.g = (color->h / 60) * diff + min;
		rgb.b = min;
	}
	else if (60 <= color->h && color->h < 120)
	{
		rgb.r = ((120 - color->h) / 60) * diff + min;
		rgb.g = max;
		rgb.b = min;
	}
	else if (120 <= color->h && color->h < 180)
	{
		rgb.r = min;
		rgb.g = max;
		rgb.b = ((color->h -120) / 60) * diff + min;
	}
	else if (180 <= color->h && color->h < 240)
	{
		rgb.r = min;
		rgb.g = ((240 - color->h) / 60) * diff +min;
		rgb.b = max;
	}
	else if (240 <= color->h && color->h < 300)
	{
		rgb.r = ((color->h - 240) / 60) * diff + min;
		rgb.g = min;
		rgb.b = max;
	}
	else if (300 <= color->h && color->h <= 360)
	{
		rgb.r = max;
		rgb.g = min;
		rgb.b = ((360 - color->h) / 60) * diff +min;
	}

	return rgb;
}

int Max(int a, int b, int c)
{
	if (a >= b && a >= c)
	{
		return a;
	}
	else if (b >= a && b >= c)
	{
		return b;
	}
	return c;
}

int Min(int a, int b, int c)
{
	if (a <= b && a <= c)
	{
		return a;
	}
	else if (b <= a && b <= c)
	{
		return b;
	}
	return c;
}