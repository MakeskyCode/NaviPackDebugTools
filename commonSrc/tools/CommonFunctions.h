
#ifndef __COMMON_FUNCTIONS_H__
#define __COMMON_FUNCTIONS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

#ifndef NAVIPACK_WIN
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef NAVIPACK_WIN
#define CLOSE closesocket
#define SOCKET_LEN int	
#else
#define CLOSE(fd) {close(fd);fd=0;}
#define SOCKET_LEN socklen_t	
#endif


int NormalToPI(int ang);
float NormalToPI(float ang);
int	NormalTo2PI(int ang);
float NormalTo2PI(float ang);
double AngleTo2PI(double ang);
double AngleToPI(double ang);

int AngleTo2PI(int ang);
int AngleToPI(int ang);

void ShowImg(unsigned char *img_data, int width, int height, const char name_prefix[]);

int BlockReceive(int sock, char* pBuffer, int readSize, int nTimeOut);

bool Compare(const std::pair<int, int> & a, const std::pair<int, int> & b);


#endif