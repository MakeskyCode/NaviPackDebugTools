#include "CommonFunctions.h"

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif
#ifndef M_2PI
#define M_2PI  (3.14159265358979323846*2)
#endif

#if NAVIPACK_WIN
//#include "opencv2/opencv.hpp"
#endif

// select mode 
#define SELECT_MODE_READY   0x001
#define SELECT_MODE_WRITE  0x002

// select return codes 
#define SELECT_STATE_READY         0
#define SELECT_STATE_ERROR         1
#define SELECT_STATE_ABORTED       2
#define SELECT_STATE_TIMEOUT       3

int NormalToPI(int ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
	while (ang > 180)
	{
		ang -= 360;
	}
	while (ang < -180)
	{
		ang += 360;
	}
	return ang;
}

float NormalToPI(float ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
	while (ang > M_PI)
	{
		ang -= (float)M_2PI;
	}
	while (ang < -M_PI)
	{
        ang += (float)M_2PI;
	}
	return ang;
}

int NormalTo2PI(int ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
	while (ang > 360)
	{
		ang -= 360;
	}
	while (ang < 0)
	{
		ang += 360;
	}
	return ang;
}

float NormalTo2PI(float ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
	while (ang > M_2PI)
	{
        ang -= (float)M_2PI;
	}
	while (ang < 0)
	{
        ang += (float)M_2PI;
	}
	return ang;
}

double AngleTo2PI(double ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
	while (ang < 0)
	{
		ang += M_2PI;
	}
	while (ang >= M_2PI)
	{
		ang -= M_2PI;
	}
	return ang;
}

double AngleToPI(double ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
	while (ang < -M_PI)
	{
		ang += M_2PI;
	}
	while (ang >= M_PI)
	{
		ang -= M_2PI;
	}
	return ang;
}

int AngleTo2PI(int ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
    while (ang < 0)
    {
        ang += 360;
    }
    while (ang >= 360)
    {
        ang -= 360;
    }
    return ang;
}

int AngleToPI(int ang)
{
	if (abs(ang)>10000000)
	{
		return 0;
	}
    while (ang < -180)
    {
        ang += 360;
    }
    while (ang >= 180)
    {
        ang -= 360;
    }
    return ang;
}

float TwoPIToAngle(float rad)
{
	if (abs(rad)>10000000)
	{
		return 0;
	}
	rad = NormalTo2PI(rad);

	return (rad/ M_2PI)*360.0;
}
void ShowImg(unsigned char *img_data, int width, int height, const char name_prefix[])
{
#if NAVIPACK_WIN
	printf("show img NULL!");
	//IplImage* gridmapImg = NULL;
	//gridmapImg = cvCreateImage(cvSize(width, height), 8, 1);
	//unsigned char value = 0;
	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		value = (unsigned char)(img_data[i + (height - j)*width]);
	//		gridmapImg->imageData[i + j*gridmapImg->widthStep] = value;
	//	}
	//}
	//cvShowImage(name_prefix, gridmapImg);
	//cvWaitKey(1);

#endif
}

int Select(int sock, int mode, int timeoutUsec)
{
	fd_set fdset;
	fd_set *readSet, *writeSet;
	timeval selectTimeout;
	int ret;

	selectTimeout.tv_sec = timeoutUsec;
	selectTimeout.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sock, &fdset);

	readSet = (mode & SELECT_MODE_READY) ? &fdset : NULL;
	writeSet = (mode & SELECT_MODE_WRITE) ? &fdset : NULL;

	ret = select(sock + 1, readSet, writeSet, NULL, &selectTimeout);

	if (ret == 1)
		return SELECT_STATE_READY;

	if (ret == -1)
	{
		if (errno == EINTR)
			return SELECT_STATE_ABORTED;

		return SELECT_STATE_ERROR;
	}

	return SELECT_STATE_TIMEOUT;
}

int BlockReceive(int sock, char* pBuffer, int readSize, int nTimeOut)
{
	int selectState;
	int recvSize;

	selectState = Select(sock, SELECT_MODE_READY, nTimeOut);
	if (SELECT_STATE_TIMEOUT == selectState)
		return 0;

	if (SELECT_STATE_READY == selectState)
	{
		recvSize = recv(sock, (char*)pBuffer, readSize, 0);
		if (recvSize <= 0)
			return -1;
		return recvSize;
	}
	return -1;
}

bool Compare(const std::pair<int, int> & a, const std::pair<int, int> & b)//
{
	if (a.first < b.first)
		return true;
	else
		return false;
}
