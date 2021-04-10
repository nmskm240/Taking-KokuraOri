#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myPng.h"
#include "colorConvert.h"
#include "reversePoland.h"

int main(int argc, char *argv[]) 
{
    BITMAPDATA_t bitmap;
    char dummy;
    int i, j, k, c = 1;
    char inputPath[256] = { "\0" };
    char outputPath[256] = { "\0" };
    char inputBuf[256] = { '\0' };
    char hFormula[10][256] = { '\0' };
    char sFormula[10][256] = { '\0' };
    char bFormula[10][256] = { '\0' };
    char formatPNG[] = ".png";
    int rangeMin[256] = { -1 };
    int rangeMax[256] = { -1 };

    RGB rgb;
    HSB hsb;
    HSB gray;

    snprintf(inputPath, 256, "%s%s%s", "..\\..\\input\\", argv[1], formatPNG);

    printf("画像の読み込みを開始");

    if (pngFileReadDecode(&bitmap, inputPath) == -1)
    {
        printf("pngFileReadDecode error\n");
        return -1;
    }

    printf("......完了\n");

    printf("変換元画像のBの値によって、変換式を変更しますか？[y/n]。");
    scanf("%s", &inputBuf);
    if (inputBuf[0] == 'y')
    {
        printf("Bを何分割しますか？(上限10)\n");
        printf("分割数:");
        scanf("%d", &c);
        if (c > 10)
        {
            printf("利用できる最大分割数は10です。\n");
            return -1;
        }
        for (i = 0; i < c; i++)
        {
            printf("%d/%d\n", i + 1, c);
            printf("範囲の下限を入力してください。:");
            scanf("%d", &rangeMin[i]);
            printf("範囲の上限を入力してください。:");
            scanf("%d", &rangeMax[i]);
        }
    }
    else
    {
        rangeMin[0] = 0;
        rangeMax[0] = 100;
    }

    for (i = 0; i < c; i++)
    {
        printf("元画像のBが%d～%dの時に使用する変換式を入力してください。\n", rangeMin[i] < 0 ? 0 : rangeMin[i], rangeMax[i] > 100 ? 100 : rangeMax[i]);
        printf("元画像のHSBを使用する場合、h、s、bを入力することで各値を使用できます。\n");
        printf("H=");
        scanf("%s%c", &inputBuf, &dummy);
        if (convertReversePol(inputBuf, hFormula[i]) == -1)
        {
            printf("error");
            return -1;
        }
        printf("S=");
        scanf("%s%c", &inputBuf, &dummy);
        if (convertReversePol(inputBuf, sFormula[i]) == -1)
        {
            printf("error");
            return -1;
        }
        printf("B=");
        scanf("%s%c", &inputBuf, &dummy);
        if (convertReversePol(inputBuf, bFormula[i]) == -1)
        {
            printf("error");
            return -1;
        }
    }

    printf("色変換を開始");

    for (i = 0; i < bitmap.width; i++)
    {
        rgb.r = bitmap.data[bitmap.ch * i];
        rgb.g = bitmap.data[bitmap.ch * i + 1];
        rgb.b = bitmap.data[bitmap.ch * i + 2];

        hsb = RGBtoHSB(&rgb);
        gray = hsb;

        for (k = 0; k < c; k++)
        {
            //春
            //if (0 <= hsb.b && hsb.b <= 55)
            //{
            //    hsb.h = 0;
            //}
            //else if (55 < hsb.b && hsb.b <= 70)
            //{
            //    hsb.h = 330;
            //}
            //else
            //{
            //    hsb.h = 150;
            //}
            //hsb.s = 100 - hsb.b;
            //hsb.b = 90;

            //夏
            //if (0 <= hsb.b && hsb.b <= 55)
            //{
            //    hsb.h = 56;
            //}
            //else if (55 < hsb.b && hsb.b <= 60)
            //{
            //    hsb.h = 135;
            //}
            //else
            //{
            //    hsb.h = 150;
            //}
            //hsb.s = 100 - hsb.b;
            //hsb.b = 90;

            ////秋
            //if (0 <= hsb.b && hsb.b <= 55)
            //{
            //    hsb.h = 30;
            //    hsb.s = 100 - hsb.b;
            //    hsb.b = 90;
            //}
            //else if (55 < hsb.b && hsb.b <= 60)
            //{
            //    hsb.h = 10;
            //    hsb.s = 100 - hsb.b;
            //    hsb.b = 90;
            //}
            //else
            //{
            //    hsb.h = 10;
            //    hsb.s = 100 - hsb.b;
            //}

            //冬
            //if(80 < hsb.h)
            //{
            //    hsb.h = 260;
            //    hsb.s = 100 - hsb.b;
            //}

            if (rangeMin[k] <= hsb.b && hsb.b <= rangeMax[k])
            {
                if (strchr(hFormula[k], (int)'+') == NULL && strchr(hFormula[k], (int)'-') == NULL && strchr(hFormula[k], (int)'*') == NULL && strchr(hFormula[k], (int)'/') == NULL)
                {
                    hsb.h = atoi(hFormula[k]);
                }
                else
                {
                    hsb.h = reversePolandAns(hFormula[k], gray.h, gray.s, gray.b);
                }
                if (strchr(sFormula[k], (int)'+') == NULL && strchr(sFormula[k], (int)'-') == NULL && strchr(sFormula[k], (int)'*') == NULL && strchr(sFormula[k], (int)'/') == NULL)
                {
                    hsb.s = atoi(sFormula[k]);
                }
                else
                {
                    hsb.s = reversePolandAns(sFormula[k], gray.h, gray.s, gray.b);
                }
                if (strchr(bFormula[k], (int)'+') == NULL && strchr(bFormula[k], (int)'-') == NULL && strchr(bFormula[k], (int)'*') == NULL && strchr(bFormula[k], (int)'/') == NULL)
                {
                    hsb.b = atoi(bFormula[k]);
                }
                else
                {
                    hsb.b = reversePolandAns(bFormula[k], gray.h, gray.s, gray.b);
                }
                break;
            }
        }

        rgb = HSBtoRGB(&hsb);

        for (j = 0; j < bitmap.height; j++)
        {
            bitmap.data[bitmap.ch * (i + j * bitmap.width)] = rgb.r;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 1] = rgb.g;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 2] = rgb.b;
        }
    }

    printf(".........完了\n");

    sprintf(outputPath, "%s%s%s", "..\\..\\output\\", argv[1], formatPNG);

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