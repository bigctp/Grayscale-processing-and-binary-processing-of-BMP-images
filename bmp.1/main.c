#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include <math.h>
#include"bmp.h"
#define PI 3.1415926
int system(const char *command);
int main()
{
    FILE *fp = fopen("tu.bmp", "rb");
    if (fp == 0)
    {
        printf("文件打开失败\n");
        return 0;
    }
    char cmd[100];
    int rotateX, rotateY;
    int write_rotateX, write_rotateY;
    BITMAPFILEHEADER *fileHead;
    BITMAPINFOHEADER *infoHead;
    fileHead=(BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER));
    infoHead=(BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
    fread(fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(infoHead, sizeof(BITMAPINFOHEADER), 1, fp);
    int width = infoHead->biWidth;
    int height = infoHead->biHeight;
    int lineByte = (width*infoHead->biBitCount / 8 + 3) / 4 * 4;
    unsigned char *pBmpBufSrc,*pBmpBufDst;
    pBmpBufSrc =(unsigned char *)malloc(lineByte*height);
    fread(pBmpBufSrc, lineByte*height, 1, fp);
    fclose(fp);
    double angle,thelta;
    printf("请输入需要旋转的角度：  ");
    scanf("%lf", &angle);
    thelta = angle * PI / 180;
    int newWidth = abs(width * cos(thelta) + height * sin(thelta));
    int newHeight = abs(width * sin(thelta) + height * cos(thelta));
    infoHead->biWidth=newWidth ;
    infoHead->biHeight=newHeight;
    int newlineByte=(infoHead->biWidth*infoHead->biBitCount / 8 + 3) / 4 * 4;
    pBmpBufDst =(unsigned char *)malloc(newlineByte*infoHead->biHeight);
    infoHead->biSizeImage=newlineByte*infoHead->biHeight;
    fileHead->bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+infoHead->biSizeImage;
    rotateX = width / 2;
    rotateY = height / 2;
    write_rotateX = newWidth / 2;
    write_rotateY = newHeight / 2;
    for (int hnum = 0; hnum < newHeight; ++ hnum)
    {
        for (int wnum = 0; wnum < newWidth; ++ wnum)
        {
            int index = hnum * newlineByte + wnum * 3;
            double d_original_img_hnum = (wnum - write_rotateX) * sin(thelta) + (hnum - write_rotateY) * cos(thelta) + rotateY;
            double d_original_img_wnum = (wnum - write_rotateX) * cos(thelta) - (hnum - write_rotateY) * sin(thelta) + rotateX;
            if (d_original_img_hnum < 0 || d_original_img_hnum > height || d_original_img_wnum < 0 || d_original_img_wnum > width)
            {
                pBmpBufDst[index] = 0; // 这个相当于是R
                pBmpBufDst[index + 1] = 0; // 这个相当于是G
                pBmpBufDst[index + 2] = 0;  // 这个相当于是B
                continue;
            }
            else
            {
                int i_original_img_hnum = d_original_img_hnum;
                int i_original_img_wnum = d_original_img_wnum;
                double distance_to_a_X = d_original_img_wnum - i_original_img_wnum;
                double distance_to_a_Y = d_original_img_hnum - i_original_img_hnum;
                int original_point_A = i_original_img_hnum * lineByte + i_original_img_wnum * 3;
                int original_point_B = i_original_img_hnum * lineByte + (i_original_img_wnum + 1) * 3;
                int original_point_C = (i_original_img_hnum + 1) * lineByte + i_original_img_wnum * 3;
                int original_point_D = (i_original_img_hnum + 1) * lineByte + (i_original_img_wnum + 1) * 3;
                if (i_original_img_wnum == width - 1)
                {
                    original_point_A = original_point_B;
                    original_point_C = original_point_D;
                }
                if (i_original_img_hnum == height - 1)
                {
                    original_point_C = original_point_A;
                    original_point_D = original_point_B;
                }
                pBmpBufDst[index] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * pBmpBufSrc[original_point_A]
                                    + (1 - distance_to_a_X) * distance_to_a_Y * pBmpBufSrc[original_point_B]
                                    + distance_to_a_X * (1 - distance_to_a_Y) * pBmpBufSrc[original_point_C]
                                    + distance_to_a_X * distance_to_a_Y * pBmpBufSrc[original_point_D];
                pBmpBufDst[index + 1] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * pBmpBufSrc[original_point_A + 1]
                                        + (1 - distance_to_a_X) * distance_to_a_Y * pBmpBufSrc[original_point_B + 1]
                                        + distance_to_a_X * (1 - distance_to_a_Y) * pBmpBufSrc[original_point_C + 1]
                                        + distance_to_a_X * distance_to_a_Y * pBmpBufSrc[original_point_D + 1];
                pBmpBufDst[index + 2] = (1 - distance_to_a_X) * (1 - distance_to_a_Y) * pBmpBufSrc[original_point_A + 2]
                                        + (1 - distance_to_a_X) * distance_to_a_Y * pBmpBufSrc[original_point_B + 2]
                                        + distance_to_a_X * (1 - distance_to_a_Y) * pBmpBufSrc[original_point_C + 2]
                                        + distance_to_a_X * distance_to_a_Y * pBmpBufSrc[original_point_D + 2];
            }
        }
    }
    char *DstName = "tu2.bmp";
    FILE *fpo = fopen(DstName, "wb");
    if (fpo == 0)
        return 0;
    fwrite(fileHead, sizeof(BITMAPFILEHEADER), 1, fpo);
    fwrite(infoHead, sizeof(BITMAPINFOHEADER), 1, fpo);
    fwrite(pBmpBufDst, sizeof(unsigned char)*newlineByte*infoHead->biHeight, 1, fpo);
    fclose(fpo);
    sprintf(cmd,DstName);
    system(cmd);
    return 0;
}
