#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "myPng.h"
#include "colorConvert.h"
#include "reversePoland.h"

int main(void) {

    BITMAPDATA_t bitmap;
    char dummy;
    int i, j;
    char filename[256] = { "\0" };
    char inputPath[270] = { "\0" };
    char outputPath[270] = { "\0" };
    char inputBuf[256];
    char hFormula[256] = { '\0' };
    char sFormula[256] = { '\0' };
    char bFormula[256] = { '\0' };

    RGB rgb;
    HSB hsb;

    printf("inputフォルダー内部に存在する、対象とするファイル名を入力してください。\n");
    scanf("%s", &filename);
    snprintf(inputPath, 256, "%s%s", "input\\", filename);

    printf("画像の読み込みを開始");

    if (pngFileReadDecode(&bitmap, inputPath) == -1)
    {
        printf("pngFileReadDecode error\n");
        return -1;
    }

    printf("......完了\n");

    printf("変換式を入力してください。\n");
    printf("H=");
    scanf("%s%c", &inputBuf, &dummy);
    if (convertReversePol(inputBuf, hFormula) == -1)
    {
        printf("error");
        return -1;
    }
    printf("S=");
    scanf("%s%c", &inputBuf, &dummy);
    if(convertReversePol(inputBuf, sFormula) == -1)
    {
        printf("error");
        return -1;
    }
    printf("B=");
    scanf("%s%c", &inputBuf, &dummy);
    if (convertReversePol(inputBuf, bFormula) == -1)
    {
        printf("error");
        return -1;
    }

    printf("色変換を開始");

    for (i = 0; i < bitmap.width; i++)
    {
        rgb.r = bitmap.data[bitmap.ch * i];
        rgb.g = bitmap.data[bitmap.ch * i + 1];
        rgb.b = bitmap.data[bitmap.ch * i + 2];

        hsb = RGBtoHSB(&rgb);

        hsb.h = reversePolandAns(hFormula);
        hsb.s = reversePolandAns(sFormula);
        hsb.b = reversePolandAns(bFormula);

        rgb = HSBtoRGB(&hsb);

        for (j = 0; j < bitmap.height; j++)
        {
            bitmap.data[bitmap.ch * (i + j * bitmap.width)] = rgb.r;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 1] = rgb.g;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 2] = rgb.b;
        }
    }

    printf(".........完了\n");

    sprintf(outputPath, "%s%s", "output\\", filename);

    printf("画像のエンコードを開始");

    if (pngFileEncodeWrite(&bitmap, outputPath) == -1) 
    {
        printf("pngFileEncodeWrite error\n");
        freeBitmapData(&bitmap);
        return -1;
    }

    printf("........完了\n");

    freeBitmapData(&bitmap);

    return 0;
}