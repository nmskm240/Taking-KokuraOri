#define _CRT_SECURE_NO_WARNINGS

#include "myPng.h"
#include "colorConvert.h"

int main(void) {

    BITMAPDATA_t bitmap;
    int i, j;
    char outname[256];

    FILE* fo;

    RGB rgb;
    HSB hsb;

    if (pngFileReadDecode(&bitmap, "C:\\Users\\bokar\\Downloads\\バーコード\\バーコード\\arigato.png") == -1) 
    {
        printf("pngFileReadDecode error\n");
        return -1;
    }

    printf("bitmap->data = %p\n", bitmap.data);
    printf("bitmap->width = %d\n", bitmap.width);
    printf("bitmap->height = %d\n", bitmap.height);
    printf("bitmap->ch = %d\n", bitmap.ch);

    for (i = 0; i < bitmap.width; i++)
    {
        rgb.r = bitmap.data[bitmap.ch * i];
        rgb.g = bitmap.data[bitmap.ch * i + 1];
        rgb.b = bitmap.data[bitmap.ch * i + 2];

        hsb = RGBtoHSB(&rgb);
        printf("h:%d s:%d b:%d\n", hsb.h, hsb.s, hsb.b);

        hsb.h = 20;
        hsb.s = 90;
        hsb.b = 100 - hsb.b;

        rgb = HSBtoRGB(&hsb);
        printf("r:%d g:%d b:%d\n", rgb.r, rgb.g, rgb.b);

        bitmap.data[bitmap.ch * i] = rgb.r;
        bitmap.data[bitmap.ch * i + 1] = rgb.g;
        bitmap.data[bitmap.ch * i + 2] = rgb.b;
    }

    for (j = 1; j < bitmap.height; j++)
    {
        for (i = 1; i < bitmap.width; i++)
        {
            bitmap.data[bitmap.ch * (i + j * bitmap.width)] = bitmap.data[bitmap.ch * (i + (j - 1) * bitmap.width)];
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 1] = bitmap.data[bitmap.ch * (i + (j - 1) * bitmap.width) + 1];
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 2] = bitmap.data[bitmap.ch * (i + (j - 1) * bitmap.width) + 2];
        }
    }

    sprintf(outname, "%s", "output.PNG");

    if (pngFileEncodeWrite(&bitmap, outname) == -1) 
    {
        printf("pngFileEncodeWrite error\n");
        freeBitmapData(&bitmap);
        return -1;
    }

    freeBitmapData(&bitmap);

    return 0;
}