#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char inputBuf[256] = { '\0' };
    char hFormula[256] = { '\0' };
    char sFormula[256] = { '\0' };
    char bFormula[256] = { '\0' };

    RGB rgb;
    HSB hsb;
    HSB gray;

    printf("input�t�H���_�[�����ɑ��݂���A�ΏۂƂ���t�@�C��������͂��Ă��������B\n");
    scanf("%s", &filename);
    snprintf(inputPath, 256, "%s%s", "input\\", filename);

    printf("�摜�̓ǂݍ��݂��J�n");

    if (pngFileReadDecode(&bitmap, inputPath) == -1)
    {
        printf("pngFileReadDecode error\n");
        return -1;
    }

    printf("......����\n");

    //printf("�ϊ����s��H�͈̔͂���͂��Ă��������B\n");
    //printf("0<=");

    printf("�ϊ�������͂��Ă��������B\n");
    printf("���摜��HSB���g�p����ꍇ�Ah�As�Ab����͂��邱�ƂŊe�l���g�p�ł��܂��B\n");
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

    printf("�F�ϊ����J�n");

    for (i = 0; i < bitmap.width; i++)
    {
        rgb.r = bitmap.data[bitmap.ch * i];
        rgb.g = bitmap.data[bitmap.ch * i + 1];
        rgb.b = bitmap.data[bitmap.ch * i + 2];

        hsb = RGBtoHSB(&rgb);
        gray = hsb;

        //�t
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

        //��
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

        ////�H
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

        //�~
        //if(80 < hsb.h)
        //{
        //    hsb.h = 260;
        //    hsb.s = 100 - hsb.b;
        //}

        if (strchr(hFormula, (int)'+') == NULL && strchr(hFormula, (int)'-') == NULL && strchr(hFormula, (int)'*') == NULL && strchr(hFormula, (int)'/') == NULL)
        {
            hsb.h = atoi(hFormula);
        }
        else
        {
            hsb.h = reversePolandAns(hFormula, gray.h, gray.s, gray.b);
        }
        if (strchr(sFormula, (int)'+') == NULL && strchr(sFormula, (int)'-') == NULL && strchr(sFormula, (int)'*') == NULL && strchr(sFormula, (int)'/') == NULL)
        {
            hsb.s = atoi(sFormula);
        }
        else
        {
            hsb.s = reversePolandAns(sFormula, gray.h, gray.s, gray.b);
        }
        if (strchr(bFormula, (int)'+') == NULL && strchr(bFormula, (int)'-') == NULL && strchr(bFormula, (int)'*') == NULL && strchr(bFormula, (int)'/') == NULL)
        {
            hsb.b = atoi(bFormula);
        }
        else
        {
            hsb.b = reversePolandAns(bFormula, gray.h, gray.s, gray.b);
        }

        rgb = HSBtoRGB(&hsb);

        for (j = 0; j < bitmap.height; j++)
        {
            bitmap.data[bitmap.ch * (i + j * bitmap.width)] = rgb.r;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 1] = rgb.g;
            bitmap.data[bitmap.ch * (i + j * bitmap.width) + 2] = rgb.b;
        }
    }

    printf(".........����\n");

    sprintf(outputPath, "%s%s", "output\\", filename);

    printf("�摜�̃G���R�[�h���J�n");

    if (pngFileEncodeWrite(&bitmap, outputPath) == -1) 
    {
        printf("pngFileEncodeWrite error\n");
        freeBitmapData(&bitmap);
        return -1;
    }

    printf("........����\n");

    freeBitmapData(&bitmap);

    return 0;
}