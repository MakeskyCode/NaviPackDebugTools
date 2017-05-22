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
	GENERAL_TEXT_MSG = 997,		//ͨ�õ�������Ϣ
	NAVIPACK_INIT_COMPLETE = 998,		//�豸��ʼ���ɹ�
	NAVIPACK_INIT_NOT_COMPLETE = 999,	//�豸��û�г�ʼ��


	SLAM_LOCATION_ERROR = 1000,			//��λʧ��
	TARGET_PATH_NOT_FOUND = 1001,		//���ܹ滮��·��
	TARGET_PATH_NOT_PASS = 1002,		//���ܹ�ͨ������

	SLAM_LOCATION_MSG = 2000,			//��λ��Ϣ
	UPDATE_FILE_FAILED = 2001,			//���������ļ�
	SAVE_MAP_FILE_FAILED = 2002,		//�����ͼ����ʧ��
	SAVE_MAP_FILE_SUCCESS = 2003,		//�����ͼ���ݳɹ�
	SEND_FILE_FAILED = 2004,			//�����ļ�ʧ��
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