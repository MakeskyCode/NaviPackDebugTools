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
	static int i = 0;
	switch (msgType) {
	case GET_NAVIPACK_VERSION:
		printf("GET_NAVIPACK_VERSION %d.%d.%d\n", (msgCode >> 24 & 0xff), (msgCode >> 16 & 0xff), (msgCode & 0xffff));
		break;
	case MCU_REG_DEVICE_MSG:
		switch (msgCode) {
		case USER_REG_READ:
		{
			/*
			int len = 0;
			char data[128];
			ReadMessageFromCarrier(navipackInterfaceId, data, &len);
			if (len > 0) {
				PrintBuf(data, len, "USER_REG_READ");
			}
			else {
				printf("USER_REG_READ Fail\n");
			}
			*/
		}
		break;
		}
		break;
	case NAVIGATION_STATUS:
		switch (msgCode) {
			case REACH_POINT:
				printf("到达目标点\n");
				break;
			case TERMINATED:
				printf("运动终止\n");
				break;
			case PATH_UPGRADE:
				printf("路径有更新 %d\n", i++);
				break;
			case START_NAVIGATION:
				printf("开始导航 %d\n", i++);
				break;
			case CANNOT_REACH:
				printf("无法到达该目标，规划最近的点。 %d\n", i++);
				break;
			case FIND_WAY_OUT:
				printf("脱困处理中 %d\n", i++);
				break;
			default:
				break;
			}
			break;
	case INIT_LOCATION_STATUS:
		switch(msgCode){
		case INIT_STATUS_BEGIN:
			printf("开始初始定位\n");
			break;
		case INIT_STATUS_SUCCESS:
			printf("初始定位完成\n");
			break;
		case INIT_STATUS_STOP:
			printf("结束初始定位\n");
			break;
		default:
			break;
		}
		break;
	case SEND_FILE_TYPE_MAP_PACKAGE:
		switch(msgCode){
		case SEND_MAP_FILE_SUCCESS:
			printf("发送地图文件成功\n");
			break;
		case SEND_MAP_FILE_FAILED:
			printf("发送地图文件失败\n");
			break;
			default:
			break;
		}
		break;
	default:
			break;
		}
	
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

	
	int naviId = Create(SERIAL_CONNECT);

	if (naviId < 0)
	{
		printf("create navipack sdk interface failed!\n");
		return -1;
	}

	//r = Open(naviId,"192.168.17.1",9977);
	r = Open(naviId,"/dev/ttyACM0",115200);
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

	
	char * paraVector[10]={};
	char keyboardBuffer[1024]={};
	
	char* para=NULL;
	char space[]=" \r\n";
	int paraNum=0;
	while(fgets(keyboardBuffer,1024,stdin)!=NULL){
		paraNum=0;
		printf("get string=%s\n",keyboardBuffer);
		para=strtok(keyboardBuffer,space);
		while(para !=NULL){
			paraVector[paraNum]=para;
			//printf("Para=%s\n",para);
			para=strtok(NULL,space);
			paraNum++;
		}
		//检查传入参数个数是否正确
		for(int i=0;i<paraNum;i++){
			printf("paraVector[%d]=%s\n",i,paraVector[i]);
			printf("str=%ld\n",strlen(paraVector[i]));
		}
		if(strcmp("initlocal",paraVector[0])==0){
			printf("cmd initlocal\n");
			InitLocation(naviId);
		}else if(strcmp("stopinitlocal",paraVector[0])==0){
			//InitLocation(naviId);
		}
		
	}
	/*
	while (true)
	{
		
		SetSpeed(naviId,0,0);
		usleep(1000*1000);
	}
	*/
	

	return 0;
}