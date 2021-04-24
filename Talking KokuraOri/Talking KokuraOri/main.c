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
    int i, j, k, c = 1;
    char dummy[256] = { "\0" };
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

    if (argc == 2)
    {
        snprintf(inputPath, 256, "%s%s%s", "..\\..\\input\\", argv[1], formatPNG);
    }
    else
    {
        printf("���q�D�ɕϊ�����input�t�H���_����png�t�@�C��������͂��Ă�������\n");
        scanf("%s", &dummy);
        snprintf(inputPath, 256, "%s%s%s", "..\\..\\input\\", dummy, formatPNG);
    }
    printf("�摜�̓ǂݍ��݂��J�n");

    if (pngFileReadDecode(&bitmap, inputPath) == -1)
    {
        printf("pngFileReadDecode error\n");
        return -1;
    }

    printf("......����\n");

    printf("�ϊ����摜��B�̒l�ɂ���āA�ϊ�����ύX���܂����H[y/n]�B");
    scanf("%s", &inputBuf);
    if (inputBuf[0] == 'y')
    {
        printf("B�����������܂����H(���10)\n");
        printf("������:");
        scanf("%d", &c);
        if (c > 10)
        {
            printf("���p�ł���ő啪������10�ł��B\n");
            return -1;
        }
        for (i = 0; i < c; i++)
        {
            printf("%d/%d\n", i + 1, c);
            printf("�͈͂̉�������͂��Ă��������B:");
            scanf("%d", &rangeMin[i]);
            printf("�͈͂̏������͂��Ă��������B:");
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
        printf("���摜��B��%d�`%d�̎��Ɏg�p����ϊ�������͂��Ă��������B\n", rangeMin[i] < 0 ? 0 : rangeMin[i], rangeMax[i] > 100 ? 100 : rangeMax[i]);
        printf("���摜��HSB���g�p����ꍇ�Ah�As�Ab����͂��邱�ƂŊe�l���g�p�ł��܂��B\n");
        printf("H=");
        scanf("%s", &inputBuf);
        if (convertReversePol(inputBuf, hFormula[i]) == -1)
        {
            printf("error");
            return -1;
        }
        printf("S=");
        scanf("%s", &inputBuf);
        if (convertReversePol(inputBuf, sFormula[i]) == -1)
        {
            printf("error");
            return -1;
        }
        printf("B=");
        scanf("%s", &inputBuf);
        if (convertReversePol(inputBuf, bFormula[i]) == -1)
        {
            printf("error");
            return -1;
        }
    }

    printf("�F�ϊ����J�n");

    for (i = 0; i < bitmap.width; i++)
    {
        rgb.r = bitmap.data[bitmap.ch * i];
        rgb.g = bitmap.data[bitmap.ch * i + 1];
        rgb.b = bitmap.data[bitmap.ch * i + 2];

        hsb = RGBtoHSB(&rgb);
        gray = hsb;

        //if (gray.b <= 15)
        //{
        //    hsb.h = 0;
        //    hsb.b = 100;
        //}
        //else if (15 < gray.b && gray.b <= 30)
        //{
        //    hsb.h = 49;
        //    hsb.b = 89;
        //}
        //else if(30 < gray.b && gray.b <= 40)
        //{
        //    hsb.h = 200;
        //    hsb.b = 58;
        //}
        //else if (40 < gray.b && gray.b <= 50)
        //{
        //    hsb.h = 220;
        //    hsb.b = 41;
        //}
        //else if (50 < gray.b && gray.b <= 75)
        //{
        //    hsb.h = 209;
        //    hsb.b = 27;
        //}
        //else
        //{
        //    hsb.h = 217;
        //    hsb.b = 20;
        //}
        //hsb.s = 100 - gray.b;

        for (k = 0; k < c; k++)
        {
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

    printf(".........����\n");

    if (argc == 2)
    {
        snprintf(outputPath, 256, "%s%s%s", "..\\..\\output\\", argv[1], formatPNG);
    }
    else
    {
        snprintf(outputPath, 256, "%s%s%s", "..\\..\\output\\", dummy, formatPNG);
    }

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