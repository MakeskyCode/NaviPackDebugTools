#ifndef _ERROR_EVENT_H_
#define _ERROR_EVENT_H_

#include "tools/CFrameList.h"
#include <inttypes.h>
#include "tools/CriticalSection.h"

enum ErrorLevel{
	EL_DEBUG,
	EL_INFO,
	EL_WARNING,
	EL_ERROR,
};

struct ErrorInfo {
	uint32_t errorLevel;
	uint32_t errorCode;
	uint8_t info[128];
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

class ErrorEvent
{
private:

private:
	static ErrorEvent * mPInstance;
	CriticalSection mPrintCS;
public :
	
	CFrameList mFrameList;
	ErrorEvent();

	static ErrorEvent* GetInstance();
	

	//add error Message
	//@param msg error msg
	//@param isNeedSendToRemote is or not send to remote
	//@return 
	static void AddErrorMsg(int32_t errorLevel,int32_t ErrorCode,uint8_t isNeedSendToRemote, const char* msg, ...);

};

#endif