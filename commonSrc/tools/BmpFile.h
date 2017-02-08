
#ifndef __BMP_FILE_H__
#define  __BMP_FILE_H__

#include "inttypes.h"
#include "stdio.h"

#ifdef NAVIPACK_WIN

#include "windows.h"

#else

typedef struct tagBITMAPFILEHEADER {
	uint16_t   bfType;
	uint32_t   bfSize;
	uint16_t   bfReserved1;
	uint16_t   bfReserved2;
	uint32_t   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	uint32_t      biSize;
	int32_t       biWidth;
	int32_t       biHeight;
	uint16_t      biPlanes;
	uint16_t      biBitCount;
	uint32_t      biCompression;
	uint32_t      biSizeImage;
	int32_t       biXPelsPerMeter;
	int32_t       biYPelsPerMeter;
	uint32_t      biClrUsed;
	uint32_t      biClrImportant;
} BITMAPINFOHEADER;

#endif
typedef struct
{
	//unsigned short    bfType;  
	unsigned long    bfSize;
	unsigned short    bfReserved1;
	unsigned short    bfReserved2;
	unsigned long    bfOffBits;
} ClBitMapFileHeader;

typedef struct
{
	unsigned long  biSize;
	long   biWidth;
	long   biHeight;
	unsigned short   biPlanes;
	unsigned short   biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	long   biXPelsPerMeter;
	long   biYPelsPerMeter;
	unsigned long   biClrUsed;
	unsigned long   biClrImportant;
} ClBitMapInfoHeader;

typedef struct
{
	unsigned char rgbBlue; //该颜色的蓝色分量  
	unsigned char rgbGreen; //该颜色的绿色分量  
	unsigned char rgbRed; //该颜色的红色分量  
	unsigned char rgbReserved; //保留值  
} ClRgbQuad;

typedef struct
{
	int width;
	int height;
	int channels;
	unsigned char* imageData;
}ClImage;


class BmpFile
{
public:
	BmpFile(const char * file_path);
	
	~BmpFile();

	int32_t GetWidth();
	int32_t GetHeight();

	int32_t GetPixelSize();

	uint32_t GetPixel(uint32_t x, uint32_t y);
	uint8_t *GetDateBuf();

	static bool clSaveImage(char* path, ClImage* bmpImg);

private:

	uint8_t *mBmpData;
	uint32_t mDataSize;
	uint32_t mLineSize;
	BITMAPFILEHEADER mFileHeader;
	BITMAPINFOHEADER mInfoHeader;

};

#endif