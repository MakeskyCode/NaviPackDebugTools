#include <stdio.h>
#include <unistd.h>
#include "NaviPackSdk.h"
#include <string.h>
#include "md5.h"
enum ErrorLevel{
	EL_DEBUG,
	EL_INFO,
	EL_WARNING,
	EL_ERROR,
};
enum ErrorCode {
	GENERAL_TEXT_MSG = 997,		//通用的文字信息
	NAVIPACK_INIT_COMPLETE = 998,		//设备初始化成功
	NAVIPACK_INIT_NOT_COMPLETE = 999,	//设备还没有初始化


	SLAM_LOCATION_ERROR = 1000,			//定位失败
	TARGET_PATH_NOT_FOUND = 1001,		//不能规划出路径
	TARGET_PATH_NOT_PASS = 1002,		//不能够通过区域

	SLAM_LOCATION_MSG = 2000,			//定位消息
	UPDATE_FILE_FAILED = 2001,			//不能升级文件
	SAVE_MAP_FILE_FAILED = 2002,		//保存地图数据失败
	SAVE_MAP_FILE_SUCCESS = 2003,		//保存地图数据成功
	SEND_FILE_FAILED = 2004,			//发送文件失败
};
void OnGetDeviceMsg(int id, int msgType, int msgCode, void* param)
{
	//printf("OnGetDeviceMsg %d %d %d \n", id, msgType, msgCode);
}

void OnGetRobotMsg(int id, s32 Level, s32 Code, char* msg)
{
	switch (Level) {
	
	case EL_INFO:
		if (Code == GENERAL_TEXT_MSG) {
			printf("[TEXT MSG FROM ROBOT]%s\n", msg);
			break;
		}	
//	case EL_WARNING:
//		break;
//	case EL_ERROR:
//		break;
	default:
		printf("[DEFAULT MSG]%s\n", msg);
		break;

	}
}


int main(int argc,char* argv[])
{
	int r;
	printf(".............NaviPackSdk .............\n");
	printf(".............version:%d .............\n", GetSDKVersion());
	printf("argc=%d\n",argc);

	
	int naviId = Create(TCP_CONNECT);

	if (naviId < 0)
	{
		printf("create navipack sdk interface failed!\n");
		return -1;
	}

	r = Open(naviId,"192.168.17.1",9977);
	//r = Open(naviId,"/dev/ttyACM0",115200);
	if (r < 0)
	{
		printf("connect to server failed\n");
		return -1;
	}

	SetCallback(naviId, OnGetDeviceMsg, OnGetRobotMsg,NULL,NULL);

	if(argc>1 && strcmp("uploadmap",argv[1])==0){ //uploadmap
		//printf("arg 1==%s\n",argv[argc-1]);
		printf("local Map Path==%s\n len=%ld\n",argv[2],strlen(argv[2]));
		LoadLocalMap(naviId,argv[2],0);
	}
	while (true)
	{
		SetSpeed(naviId,0,0);
		usleep(1000*1000);
	}

	return 0;
}