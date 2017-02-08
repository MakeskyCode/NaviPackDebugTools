#include "BmpFile.h"
#include "string.h"
#include <stdlib.h>

BmpFile::BmpFile(const char * file_path)
{
	mBmpData = NULL;
	memset(&mFileHeader, 0, sizeof(mFileHeader));
	memset(&mInfoHeader, 0, sizeof(mInfoHeader));

	FILE *file = fopen(file_path, "rb");
	if (file == 0)
		return;

	fread(&mFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	if (mFileHeader.bfType != 0x4D42)
		return;

	fread(&mInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	mLineSize = (mInfoHeader.biWidth * mInfoHeader.biBitCount / 8 + 3) / 4 * 4;

	mDataSize = mLineSize * mInfoHeader.biHeight;
	mBmpData = new uint8_t[mDataSize];

	fseek(file, mFileHeader.bfOffBits, SEEK_SET);
	fread(mBmpData, 1, mDataSize, file);

	fclose(file);
}

BmpFile::~BmpFile()
{
	if (mBmpData)
		delete[] mBmpData;
}

int32_t BmpFile::GetWidth()
{
	return mInfoHeader.biWidth;
}

int32_t BmpFile::GetHeight()
{
	return mInfoHeader.biHeight;
}

int32_t BmpFile::GetPixelSize()
{
	return mInfoHeader.biBitCount / 8;
}

uint32_t BmpFile::GetPixel(uint32_t x, uint32_t y)
{
	if (mBmpData == NULL)
		return 0;

	if (x >= (uint32_t)mInfoHeader.biWidth || y >= (uint32_t)mInfoHeader.biHeight)
		return 0;

	int pixsize = mInfoHeader.biBitCount / 8;
	int pos = mLineSize * y + x * pixsize;
	uint8_t data[4] = { 0,0,0,0 };
	memcpy(data, mBmpData + pos, pixsize);
	return *((uint32_t *)data);
}

uint8_t * BmpFile::GetDateBuf()
{
	return mBmpData;
}

bool BmpFile::clSaveImage(char * path, ClImage * bmpImg)
{
	FILE *pFile;
	unsigned short fileType;
	ClBitMapFileHeader bmpFileHeader;
	ClBitMapInfoHeader bmpInfoHeader;
	int step;
	int offset;
	unsigned char pixVal = '\0';
	int i, j;
	ClRgbQuad* quad;

	pFile = fopen(path, "wb");
	if (!pFile)
	{
		return false;
	}

	fileType = 0x4D42;
	fwrite(&fileType, sizeof(unsigned short), 1, pFile);

	if (bmpImg->channels == 3)//24位，通道，彩图  
	{
		step = bmpImg->channels*bmpImg->width;
		offset = step % 4;
		if (offset != 4)
		{
			step += 4 - offset;
		}

		bmpFileHeader.bfSize = bmpImg->height*step + 54;
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfOffBits = 54;
		fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 24;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height*step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 0;
		bmpInfoHeader.biClrImportant = 0;
		fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

		for (i = bmpImg->height - 1; i > -1; i--)
		{
			for (j = 0; j < bmpImg->width; j++)
			{
				pixVal = bmpImg->imageData[i*bmpImg->width * 3 + j * 3];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				pixVal = bmpImg->imageData[i*bmpImg->width * 3 + j * 3 + 1];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				pixVal = bmpImg->imageData[i*bmpImg->width * 3 + j * 3 + 2];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			}
			if (offset != 0)
			{
				for (j = 0; j < offset; j++)
				{
					pixVal = 0;
					fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}
	}
	else if (bmpImg->channels == 1)//8位，单通道，灰度图  
	{
		step = bmpImg->width;
		offset = step % 4;
		if (offset != 4)
		{
			step += 4 - offset;
		}

		bmpFileHeader.bfSize = 54 + 256 * 4 + bmpImg->width;
		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfOffBits = 54 + 256 * 4;
		fwrite(&bmpFileHeader, sizeof(ClBitMapFileHeader), 1, pFile);

		bmpInfoHeader.biSize = 40;
		bmpInfoHeader.biWidth = bmpImg->width;
		bmpInfoHeader.biHeight = bmpImg->height;
		bmpInfoHeader.biPlanes = 1;
		bmpInfoHeader.biBitCount = 8;
		bmpInfoHeader.biCompression = 0;
		bmpInfoHeader.biSizeImage = bmpImg->height*step;
		bmpInfoHeader.biXPelsPerMeter = 0;
		bmpInfoHeader.biYPelsPerMeter = 0;
		bmpInfoHeader.biClrUsed = 256;
		bmpInfoHeader.biClrImportant = 256;
		fwrite(&bmpInfoHeader, sizeof(ClBitMapInfoHeader), 1, pFile);

		quad = (ClRgbQuad*)malloc(sizeof(ClRgbQuad) * 256);
		for (i = 0; i < 256; i++)
		{
			quad[i].rgbBlue = i;
			quad[i].rgbGreen = i;
			quad[i].rgbRed = i;
			quad[i].rgbReserved = 0;
		}
		fwrite(quad, sizeof(ClRgbQuad), 256, pFile);
		free(quad);

		for (i = bmpImg->height - 1; i > -1; i--)
		{
			for (j = 0; j < bmpImg->width; j++)
			{
				pixVal = bmpImg->imageData[i*bmpImg->width + j];
				fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
			}
			if (offset != 0)
			{
				for (j = 0; j < offset; j++)
				{
					pixVal = 0;
					fwrite(&pixVal, sizeof(unsigned char), 1, pFile);
				}
			}
		}
	}
	fclose(pFile);

	return true;
}
