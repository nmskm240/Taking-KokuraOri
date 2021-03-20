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

    if (pngFileReadDecode(&bitmap, "C:\\Users\\bokar\\Downloads\\�o�[�R�[�h\\�o�[�R�[�h\\arigato.png") == -1) 
    {
        printf("pngFileReadDecode error\n");
        return -1;
    }

    for (i = 0; i < bitmap.width; i++)
    {
        rgb.r = bitmap.data[bitmap.ch * i];
        rgb.g = bitmap.data[bitmap.ch * i + 1];
        rgb.b = bitmap.data[bitmap.ch * i + 2];

        hsb = RGBtoHSB(&rgb);

        hsb.h = 20;
        hsb.s = 90;
        hsb.b = 100 - hsb.b;

        rgb = HSBtoRGB(&hsb);

        for (j = 0; j < bitmap.height; j++)
        {
            bitmap.data[bitmap.ch * (i + j * bitmap.width)] = rgb.r;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 1] = rgb.g;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 2] = rgb.b;
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