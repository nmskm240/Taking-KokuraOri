#include "colorConvert.h"

HSB RGBtoHSB(RGB* color)
{
	HSB hsb;
	int max, min, diff;

	//printf("converter in(r:%d g:%d b:%d)\n", color->r, color->g, color->b);

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
		hsb.h = (int)(60.0 * ((float)(color->g - color->b) / (float)diff));
	}
	else if (color->g == max)
	{
		//H = 60 ~ ((B - R) € (MAX - MIN)) +120
		hsb.h = (int)(60.0 * ((float)(color->b - color->r) / (float)diff) + 120.0);
	}
	else
	{
		//H = 60 ~ ((R - G) € (MAX - MIN)) +240
		hsb.h = (int)(60.0 * ((float)(color->r - color->g) / (float)diff) + 240.0);
	}

	if (hsb.h < 0)
	{
		hsb.h += 360;
	}

	hsb.s = (max == 0) ? 0 : (int)(((float)diff / (float)max) * 100.0);
	hsb.b = (int)(((float)max / (float)255) * 100.0);

	//printf("max:%d min:%d diff:%d\n", max, min, diff);
	//printf("converter out(h:%d s:%d b:%d)\n", hsb.h, hsb.s, hsb.b);

	return hsb;
}

RGB HSBtoRGB(HSB* color)
{
	RGB rgb;
	int max, min, diff;
	
	//printf("converter in(h:%d s:%d b:%d)\n", color->h, color->s, color->b);

	color->s = color->s * (255.0f / 100.0f);
	color->b = color->b * (255.0f / 100.0f);
	
	max = color->b;
	min = (int)((float)max - (((float)color->s / 255.0f) * max));
	diff = max - min;

	//printf("max:%d min:%d diff:%d\n", max, min, diff);

	if (0 <= color->h && color->h < 60)
	{
		rgb.r = max;
		rgb.g = (int)(((float)color->h / 60.0f) * diff + min);
		rgb.b = min;
	}
	else if (60 <= color->h && color->h < 120)
	{
		rgb.r = (int)(((120.0 - (float)color->h) / 60.0) * diff + min);
		rgb.g = max;
		rgb.b = min;
	}
	else if (120 <= color->h && color->h < 180)
	{
		rgb.r = min;
		rgb.g = max;
		rgb.b = (int)((((float)color->h -120.0) / 60.0) * diff + min);
	}
	else if (180 <= color->h && color->h < 240)
	{
		rgb.r = min;
		rgb.g = (int)(((240.0 - (float)color->h) / 60.0) * diff + min);
		rgb.b = max;
	}
	else if (240 <= color->h && color->h < 300)
	{
		rgb.r = (int)((((float)color->h - 240.0) / 60.0) * diff + min);
		rgb.g = min;
		rgb.b = max;
	}
	else if (300 <= color->h && color->h <= 360)
	{
		rgb.r = max;
		rgb.g = min;
		rgb.b = (int)(((360.0 - (float)color->h) / 60.0) * diff + min);
	}

	//printf("converter out(r:%d g:%d b:%d)\n", rgb.r, rgb.g, rgb.b);

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