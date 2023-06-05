#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#pragma pack(1)//单字节对齐
typedef struct tagBITMAPFILEHEADER
{
    unsigned char  bfType[2];//文件格式
    unsigned int   bfSize;        // 文件大小 以字节为单位(2-5字节)
    unsigned short bfReserved1;   // 保留，必须设置为0 (6-7字节)
    unsigned short bfReserved2;   // 保留，必须设置为0 (8-9字节)
    unsigned int   bfOffBits;     // 从文件头到像素数据的偏移  (10-13字节)
} BITMAPFILEHEADER;
#pragma pack()

/*位图信息头*/
#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{
    unsigned int    biSize;          // 此结构体的大小 (14-17字节)
    long            biWidth;         // 图像的宽  (18-21字节)
    long            biHeight;        // 图像的高  (22-25字节)
    unsigned short  biPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
    unsigned short  biBitCount;      // 一像素所占的位数，(28-29字节)当biBitCount=24时，该BMP图像就是24Bit真彩图，没有调色板项。
    unsigned int    biCompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
    unsigned int    biSizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
    long            biXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
    long            biYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    unsigned int    biClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。 (46-49字节)
    unsigned int    biClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
} BITMAPINFOHEADER;
#pragma pack()

/*调色板结构*/
#pragma pack(1)
typedef struct tagRGBQUAD
{
    unsigned char rgbBlue;   //该颜色的蓝色分量  (值范围为0-255)
    unsigned char rgbGreen;  //该颜色的绿色分量  (值范围为0-255)
    unsigned char rgbRed;    //该颜色的红色分量  (值范围为0-255)
    unsigned char rgbReserved;// 保留，必须为0
} RGBQUAD;
#pragma pack()

int main()
{
    /*变量声明*/
    FILE *fpBMP,*fpTwoValue;//源文件fpBMP,目标文件fpTwoValue
    char filename1[20], filename2[20];

    BITMAPFILEHEADER *fileHeader;//位图文件头
    BITMAPINFOHEADER *infoHeader;//位图信息头
    RGBQUAD *ipRGB;//调色板

    int i,j,k=0;
    unsigned char *a;//存储源图每行像素值
    unsigned char b;//存储每个像素的灰度值或二值
    unsigned char *c;//存储每行像素的二值
    unsigned char *d;
    char cmd[100];
    int avg=0,sum=0;


    printf("输入图像文件名：");
    scanf("%s", filename1);
    if ((fpBMP = fopen(filename1, "rb")) == NULL)
    {
        printf("打开图片失败");
        exit(0);
    }

    printf("输出图像文件名：");
    scanf("%s", filename2);
    if ((fpTwoValue = fopen(filename2, "wb")) == NULL)
    {
        printf("创建图片失败");
        exit(0);
    }
    /********************************************************************/

    /*创建位图文件头，信息头，调色板*/
    fileHeader=(BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER));
    infoHeader=(BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
    ipRGB=(RGBQUAD *)malloc(256*sizeof(RGBQUAD));

    /*读入源位图文件头和信息头*/
    fread(fileHeader,sizeof(BITMAPFILEHEADER),1,fpBMP);
    fread(infoHeader,sizeof(BITMAPINFOHEADER),1,fpBMP);
    //经过这两条程序把BMP图像的信息头、文件头赋给fileHeader和infoHeader变量，可以根据fileHeader和infoHeader得到图像的各种属性。
    printf("原始图片每个像素的位数:%d\n",infoHeader->biBitCount);
    printf("原始图片每个像素像素数据偏移:%d\n",fileHeader->bfOffBits);

    //修改信息头
    //信息头共有11部分，灰度化时需要修改4部分
    infoHeader->biBitCount=8;//转换成二值图后，颜色深度由24位变为8位
    infoHeader->biSizeImage=((infoHeader->biWidth+3)/4)*4*infoHeader->biHeight;//每个像素由三字节变为单字节，同时每行像素要四字节对齐
    infoHeader->biClrUsed=256;//颜色索引表数量
    infoHeader->biClrImportant=255;//重要颜色索引为0，表示都重要
    //修改文件头
    //文件头共有5部分，灰度化时需要修改两部分
    fileHeader->bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);//数据区偏移量，等于文件头，信息头，索引表的大小之和
    fileHeader->bfSize=fileHeader->bfOffBits+infoHeader->biSizeImage;//文件大小，等于偏移量加上数据区大小
    for (i = 0; i < 256; i++)
	{
		ipRGB[i].rgbBlue = ipRGB[i].rgbGreen = ipRGB[i].rgbRed = i;
		ipRGB[i].rgbReserved=0;
	}
    printf("修改后的图片每个像素的位数:%d\n",infoHeader->biBitCount);
    printf("修改后的图片每个像素数据偏移:%d\n",fileHeader->bfOffBits);

    /********************************************************************/

    //读取BMP图像的信息头、文件头、BMP调色板到新建的图片
    fwrite(fileHeader,sizeof(BITMAPFILEHEADER),1,fpTwoValue);
    fwrite(infoHeader,sizeof(BITMAPINFOHEADER),1,fpTwoValue);
    fwrite(ipRGB,256*sizeof(RGBQUAD),1,fpTwoValue);

    /*将彩色图转为二值图*/
    a=(unsigned char *)malloc((infoHeader->biWidth*3+3)/4*4);//给变量a申请源图每行像素所占大小的空间,考虑四字节对齐问题
    c=(unsigned char *)malloc((infoHeader->biWidth+3)/4*4);//给变量c申请目标图每行像素所占大小的空间,同样四字节对齐
    d=(unsigned char *)malloc((infoHeader->biWidth*3+3)/4*4);
    for(i=0; i<infoHeader->biHeight; i++) //遍历图像每行的循环
    {
        for(j=0; j<((infoHeader->biWidth*3+3)/4*4); j++) //遍历每行中每个字节的循环
        {
            fread(a+j,1,1,fpBMP);//将源图每行的每一个字节读入变量a所指向的内存空间
        }
        for(j=0; j<infoHeader->biWidth; j++) //循环像素宽度次,就不会计算读入四字节填充位
        {
            avg+=(int)(0.114*(float)a[k]+0.587*(float)a[k+1]+0.299*(float)a[k+2]);//a中每三个字节分别代表BGR分量，乘上不同权值转化为灰度值
            k+=3;
        }
        k=0;
    }
    sum=infoHeader->biWidth*infoHeader->biHeight;
    avg/=sum;
    fseek(fpBMP,54,SEEK_SET);
    for(i=0; i<infoHeader->biHeight; i++) //遍历图像每行的循环
    {
        for(j=0; j<((infoHeader->biWidth*3+3)/4*4); j++) //遍历每行中每个字节的循环
        {
            fread(d+j,1,1,fpBMP);//将源图每行的每一个字节读入变量a所指向的内存空间
        }
        for(j=0; j<infoHeader->biWidth; j++) //循环像素宽度次,就不会计算读入四字节填充位
        {
            b=(int)(0.114*(float)d[k]+0.587*(float)d[k+1]+0.299*(float)d[k+2]);//a中每三个字节分别代表BGR分量，乘上不同权值转化为灰度值
            if(70<=(int)b&&(int)b<=120) //将灰度值转化为二值
                b=255;
            else b=0;
            c[j]=b;   //存储每行的二值
            k+=3;
        }
        fwrite(c,(infoHeader->biWidth+3)/4*4,1,fpTwoValue);//将二值像素四字节填充写入文件,填充位没有初始化,为随机值
        k=0;
    }
{
    /*释放内存空间，关闭文件*/
    free(fileHeader);
    free(infoHeader);
    free(ipRGB);
    free(a);
    free(c);
    fclose(fpBMP);
    fclose(fpTwoValue);
    sprintf(cmd,filename2);
    system(cmd);
    return 0;
}
}
