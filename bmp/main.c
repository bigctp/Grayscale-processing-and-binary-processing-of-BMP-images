#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#pragma pack(1)//���ֽڶ���
typedef struct tagBITMAPFILEHEADER
{
    unsigned char  bfType[2];//�ļ���ʽ
    unsigned int   bfSize;        // �ļ���С ���ֽ�Ϊ��λ(2-5�ֽ�)
    unsigned short bfReserved1;   // ��������������Ϊ0 (6-7�ֽ�)
    unsigned short bfReserved2;   // ��������������Ϊ0 (8-9�ֽ�)
    unsigned int   bfOffBits;     // ���ļ�ͷ���������ݵ�ƫ��  (10-13�ֽ�)
} BITMAPFILEHEADER;
#pragma pack()

/*λͼ��Ϣͷ*/
#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{
    unsigned int    biSize;          // �˽ṹ��Ĵ�С (14-17�ֽ�)
    long            biWidth;         // ͼ��Ŀ�  (18-21�ֽ�)
    long            biHeight;        // ͼ��ĸ�  (22-25�ֽ�)
    unsigned short  biPlanes;        // ��ʾbmpͼƬ��ƽ��������Ȼ��ʾ��ֻ��һ��ƽ�棬���Ժ����1 (26-27�ֽ�)
    unsigned short  biBitCount;      // һ������ռ��λ����(28-29�ֽ�)��biBitCount=24ʱ����BMPͼ�����24Bit���ͼ��û�е�ɫ���
    unsigned int    biCompression;   // ˵��ͼ������ѹ�������ͣ�0Ϊ��ѹ���� (30-33�ֽ�)
    unsigned int    biSizeImage;     // ����������ռ��С, ���ֵӦ�õ��������ļ�ͷ�ṹ��bfSize-bfOffBits (34-37�ֽ�)
    long            biXPelsPerMeter; // ˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (38-41�ֽ�)
    long            biYPelsPerMeter; // ˵����ֱ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (42-45�ֽ�)
    unsigned int    biClrUsed;       // ˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ����� (46-49�ֽ�)
    unsigned int    biClrImportant;  // ˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��(50-53�ֽ�)
} BITMAPINFOHEADER;
#pragma pack()

/*��ɫ��ṹ*/
#pragma pack(1)
typedef struct tagRGBQUAD
{
    unsigned char rgbBlue;   //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbGreen;  //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbRed;    //����ɫ�ĺ�ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbReserved;// ����������Ϊ0
} RGBQUAD;
#pragma pack()

int main()
{
    /*��������*/
    FILE *fpBMP,*fpTwoValue;//Դ�ļ�fpBMP,Ŀ���ļ�fpTwoValue
    char filename1[20], filename2[20];

    BITMAPFILEHEADER *fileHeader;//λͼ�ļ�ͷ
    BITMAPINFOHEADER *infoHeader;//λͼ��Ϣͷ
    RGBQUAD *ipRGB;//��ɫ��

    int i,j,k=0;
    unsigned char *a;//�洢Դͼÿ������ֵ
    unsigned char b;//�洢ÿ�����صĻҶ�ֵ���ֵ
    unsigned char *c;//�洢ÿ�����صĶ�ֵ
    unsigned char *d;
    char cmd[100];
    int avg=0,sum=0;


    printf("����ͼ���ļ�����");
    scanf("%s", filename1);
    if ((fpBMP = fopen(filename1, "rb")) == NULL)
    {
        printf("��ͼƬʧ��");
        exit(0);
    }

    printf("���ͼ���ļ�����");
    scanf("%s", filename2);
    if ((fpTwoValue = fopen(filename2, "wb")) == NULL)
    {
        printf("����ͼƬʧ��");
        exit(0);
    }
    /********************************************************************/

    /*����λͼ�ļ�ͷ����Ϣͷ����ɫ��*/
    fileHeader=(BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER));
    infoHeader=(BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
    ipRGB=(RGBQUAD *)malloc(256*sizeof(RGBQUAD));

    /*����Դλͼ�ļ�ͷ����Ϣͷ*/
    fread(fileHeader,sizeof(BITMAPFILEHEADER),1,fpBMP);
    fread(infoHeader,sizeof(BITMAPINFOHEADER),1,fpBMP);
    //���������������BMPͼ�����Ϣͷ���ļ�ͷ����fileHeader��infoHeader���������Ը���fileHeader��infoHeader�õ�ͼ��ĸ������ԡ�
    printf("ԭʼͼƬÿ�����ص�λ��:%d\n",infoHeader->biBitCount);
    printf("ԭʼͼƬÿ��������������ƫ��:%d\n",fileHeader->bfOffBits);

    //�޸���Ϣͷ
    //��Ϣͷ����11���֣��ҶȻ�ʱ��Ҫ�޸�4����
    infoHeader->biBitCount=8;//ת���ɶ�ֵͼ����ɫ�����24λ��Ϊ8λ
    infoHeader->biSizeImage=((infoHeader->biWidth+3)/4)*4*infoHeader->biHeight;//ÿ�����������ֽڱ�Ϊ���ֽڣ�ͬʱÿ������Ҫ���ֽڶ���
    infoHeader->biClrUsed=256;//��ɫ����������
    infoHeader->biClrImportant=255;//��Ҫ��ɫ����Ϊ0����ʾ����Ҫ
    //�޸��ļ�ͷ
    //�ļ�ͷ����5���֣��ҶȻ�ʱ��Ҫ�޸�������
    fileHeader->bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);//������ƫ�����������ļ�ͷ����Ϣͷ��������Ĵ�С֮��
    fileHeader->bfSize=fileHeader->bfOffBits+infoHeader->biSizeImage;//�ļ���С������ƫ����������������С
    for (i = 0; i < 256; i++)
	{
		ipRGB[i].rgbBlue = ipRGB[i].rgbGreen = ipRGB[i].rgbRed = i;
		ipRGB[i].rgbReserved=0;
	}
    printf("�޸ĺ��ͼƬÿ�����ص�λ��:%d\n",infoHeader->biBitCount);
    printf("�޸ĺ��ͼƬÿ����������ƫ��:%d\n",fileHeader->bfOffBits);

    /********************************************************************/

    //��ȡBMPͼ�����Ϣͷ���ļ�ͷ��BMP��ɫ�嵽�½���ͼƬ
    fwrite(fileHeader,sizeof(BITMAPFILEHEADER),1,fpTwoValue);
    fwrite(infoHeader,sizeof(BITMAPINFOHEADER),1,fpTwoValue);
    fwrite(ipRGB,256*sizeof(RGBQUAD),1,fpTwoValue);

    /*����ɫͼתΪ��ֵͼ*/
    a=(unsigned char *)malloc((infoHeader->biWidth*3+3)/4*4);//������a����Դͼÿ��������ռ��С�Ŀռ�,�������ֽڶ�������
    c=(unsigned char *)malloc((infoHeader->biWidth+3)/4*4);//������c����Ŀ��ͼÿ��������ռ��С�Ŀռ�,ͬ�����ֽڶ���
    d=(unsigned char *)malloc((infoHeader->biWidth*3+3)/4*4);
    for(i=0; i<infoHeader->biHeight; i++) //����ͼ��ÿ�е�ѭ��
    {
        for(j=0; j<((infoHeader->biWidth*3+3)/4*4); j++) //����ÿ����ÿ���ֽڵ�ѭ��
        {
            fread(a+j,1,1,fpBMP);//��Դͼÿ�е�ÿһ���ֽڶ������a��ָ����ڴ�ռ�
        }
        for(j=0; j<infoHeader->biWidth; j++) //ѭ�����ؿ�ȴ�,�Ͳ������������ֽ����λ
        {
            avg+=(int)(0.114*(float)a[k]+0.587*(float)a[k+1]+0.299*(float)a[k+2]);//a��ÿ�����ֽڷֱ����BGR���������ϲ�ͬȨֵת��Ϊ�Ҷ�ֵ
            k+=3;
        }
        k=0;
    }
    sum=infoHeader->biWidth*infoHeader->biHeight;
    avg/=sum;
    fseek(fpBMP,54,SEEK_SET);
    for(i=0; i<infoHeader->biHeight; i++) //����ͼ��ÿ�е�ѭ��
    {
        for(j=0; j<((infoHeader->biWidth*3+3)/4*4); j++) //����ÿ����ÿ���ֽڵ�ѭ��
        {
            fread(d+j,1,1,fpBMP);//��Դͼÿ�е�ÿһ���ֽڶ������a��ָ����ڴ�ռ�
        }
        for(j=0; j<infoHeader->biWidth; j++) //ѭ�����ؿ�ȴ�,�Ͳ������������ֽ����λ
        {
            b=(int)(0.114*(float)d[k]+0.587*(float)d[k+1]+0.299*(float)d[k+2]);//a��ÿ�����ֽڷֱ����BGR���������ϲ�ͬȨֵת��Ϊ�Ҷ�ֵ
            if(70<=(int)b&&(int)b<=120) //���Ҷ�ֵת��Ϊ��ֵ
                b=255;
            else b=0;
            c[j]=b;   //�洢ÿ�еĶ�ֵ
            k+=3;
        }
        fwrite(c,(infoHeader->biWidth+3)/4*4,1,fpTwoValue);//����ֵ�������ֽ����д���ļ�,���λû�г�ʼ��,Ϊ���ֵ
        k=0;
    }
{
    /*�ͷ��ڴ�ռ䣬�ر��ļ�*/
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
