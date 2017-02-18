#include <stdio.h>
#include <unistd.h>
#include "NaviPackSdk.h"

void OnGetDeviceMsg(int id, int msgType, int msgCode, void* param)
{
	printf("OnGetDeviceMsg %d %d %d \n", id, msgType, msgCode);
}

void OnGetErrorMsg(s32 id, s32 errorLevel, s32 errorCode, char* msg)
{
	printf("OnGetErrorMsg %s\n", msg);
}


int main()
{
	int r;
	printf(".............NaviPackSdk .............\n");
	printf(".............version:%d .............\n", GetSDKVersion());
	int naviId = Create(SERIAL_CONNECT);
	if (naviId < 0)
	{
		printf("create navipack sdk interface failed!\n");
		return -1;
	}

	r = Open(naviId,"/dev/ttyACM0",115200);
	if (r < 0)
	{
		printf("connect to server failed\n");
		return -1;
	}

	SetCallback(naviId, OnGetDeviceMsg, OnGetErrorMsg,NULL,NULL);

	while (true)
	{
		SetSpeed(naviId,0,0);
		usleep(1000*1000);
	}

	return 0;
}