# ifndef BMP_H
# define BMP_H

/*λͼ�ļ�ͷ*/
#pragma pack(1)//���ֽڶ���
typedef struct tagBITMAPFILEHEADER
{
    unsigned char  bfType[2];//�ļ���ʽ
    unsigned int   bfSize;        // �ļ���С ���ֽ�Ϊ��λ(2-5�ֽ�)
    unsigned short bfReserved1;   // ��������������Ϊ0 (6-7�ֽ�)
    unsigned short bfReserved2;   // ��������������Ϊ0 (8-9�ֽ�)
    unsigned int   bfOffBits;     // ���ļ�ͷ���������ݵ�ƫ��  (10-13�ֽ�)
}BITMAPFILEHEADER;
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
}BITMAPINFOHEADER;
#pragma pack()

/*��ɫ��ṹ*/
#pragma pack(1)
typedef struct tagRGBQUAD
{
    unsigned char rgbBlue;   //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbGreen;  //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbRed;    //����ɫ�ĺ�ɫ����  (ֵ��ΧΪ0-255)
    unsigned char rgbReserved;// ����������Ϊ0
}RGBQUAD;
#pragma pack()

#endif
