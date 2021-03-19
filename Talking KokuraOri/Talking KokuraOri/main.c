#define _CRT_SECURE_NO_WARNINGS

#include "myPng.h"
#include "colorConvert.h"

int main(void) {

    BITMAPDATA_t bitmap;
    int i, j, c;
    int ave, sum;
    char outname[256];

    FILE* fo;

    if (pngFileReadDecode(&bitmap, "C:\\Users\\bokar\\Downloads\\バーコード\\バーコード\\arigato.png") == -1) {
        printf("pngFileReadDecode error\n");
        return -1;
    }

    printf("bitmap->data = %p\n", bitmap.data);
    printf("bitmap->width = %d\n", bitmap.width);
    printf("bitmap->height = %d\n", bitmap.height);
    printf("bitmap->ch = %d\n", bitmap.ch);

    ///* グレースケールに変換 */
    //for (j = 0; j < bitmap.height; j++) {
    //    for (i = 0; i < bitmap.width; i++) {
    //        sum = 0;
    //        for (c = 0; c < bitmap.ch; c++) {
    //            sum += bitmap.data[bitmap.ch * (i + j * bitmap.width) + c];
    //        }
    //        ave = sum / bitmap.ch;
    //        for (c = 0; c < bitmap.ch; c++) {
    //            bitmap.data[bitmap.ch * (i + j * bitmap.width) + c] = ave;
    //        }
    //    }
    //}

    RGB rgb;
    HSB hsb;

    for (j = 0; j < bitmap.height; j++)
    {
        for (i = 0; i < bitmap.width; i++)
        {
            for (c = 0; c < bitmap.ch; c++)
            {
                if (c == 0)
                {
                    rgb.r = bitmap.data[bitmap.ch * (i + j * bitmap.width) + c];
                }
                else if (c == 1)
                {
                    rgb.g= bitmap.data[bitmap.ch * (i + j * bitmap.width) + c];
                }
                else if (c == 2)
                {
                    rgb.b = bitmap.data[bitmap.ch * (i + j * bitmap.width) + c];
                }
            }
            hsb = RGBtoHSB(&rgb);
        }
    }

    sprintf(outname, "%s", "output.PNG");

    if (pngFileEncodeWrite(&bitmap, outname) == -1) {
        printf("pngFileEncodeWrite error\n");
        freeBitmapData(&bitmap);
        return -1;
    }

    freeBitmapData(&bitmap);

    return 0;
}