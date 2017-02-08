#include "ErrorEvent.h"
#include "tools/LogFile.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef NAVIPACK_WIN
#include <unistd.h>
#endif

#include "tools/windowsdef.h"

ErrorEvent*  ErrorEvent::mPInstance = NULL;
ErrorEvent* ErrorEvent::GetInstance()
{
	if (mPInstance == NULL)
	{
		mPInstance = new ErrorEvent();
	}
	return mPInstance;
}

ErrorEvent::ErrorEvent()
{
	mFrameList.Init(3,128*1024,"ErrorEvent");
}

void ErrorEvent::AddErrorMsg(int32_t errorLevel, int32_t errorCode, uint8_t isNeedSendToRemote, const char* msg, ...)
{
	if (mPInstance == NULL)
	{
		LOGE("*** AddErrorMsg code is %d msg is %s", errorCode, msg);
		return;
	}

	mPInstance->mPrintCS.Enter();
	char buf[1024];
	va_list args;
	va_start(args, msg);
	vsprintf(buf, msg, args);
	va_end(args);

	uint32_t len = strlen(buf);
	if (len > 128)
	{
		len = 128;
	}

	LOGE("AddErrorMsg code = %d msg = %s", errorCode, buf);

	if (isNeedSendToRemote)
	{
		ErrorInfo errorInfo;
		errorInfo.errorLevel = errorLevel;
		errorInfo.errorCode = errorCode;
		memset(errorInfo.info, 0, sizeof(errorInfo.info));
		memcpy(errorInfo.info,buf,len);
		mPInstance->mFrameList.WriteFrame((char*)&errorInfo,sizeof(ErrorInfo));
	}
	mPInstance->mPrintCS.Leave();
}