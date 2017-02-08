#include "Camera3DInterface.h"

#define READ_BUF_RATE 6

void Camera3DInterface::InitCamera3DParam()
{
	mFrameWidth = 0;
	mFrameHeight = 0;
#if NAVIPACK_WIN

#endif
}



Camera3DInterface::Camera3DInterface(InterfaceID id, uint32_t maxTxBufferSize) : HardwareInterface(id, maxTxBufferSize)
{
	InitCamera3DParam();
}

Camera3DInterface::~Camera3DInterface()
{
	Close();
}

int Camera3DInterface::Open(const char * file_name, uint32_t param1, uint32_t param2)
{

	Close();

	mFrameWidth = param1;
	mFrameHeight = param2;
	mFrameLen = mFrameWidth*mFrameHeight*READ_BUF_RATE;

	int ret = mCamera.Open(file_name,param1,param2);

	mFrameBuf = new uint8_t[mFrameLen];
	if (ret >= 0)
	{
		mRxThread.Start();
	}

	return ret;
}

int Camera3DInterface::Close()
{
	mRxThread.Exit();
	if (mFrameBuf)
	{
		delete[] mFrameBuf;
	}
	return mCamera.Close();
}

int Camera3DInterface::ReOpen()
{
	Close();
	return Open(mFileName, mFrameWidth, mFrameHeight);
	return 0;
}

int Camera3DInterface::RxFun()
{
	if (mFrameWidth <= 0 || mFrameHeight <= 0)
	{
		return 0;
	}

	int32_t readed;

	readed = ReadBufferFromInterface(mFrameBuf, mFrameLen);

	if (readed == mFrameLen)
	{
		mCS.Enter();
		if (mIsNeedWriteToFile) {
			mInterfaceStreamWriter->Write(mId, (const char *)mFrameBuf, readed);
		}

		for (int32_t i = 0; i < mProtocolArraySize; i++)
		{
			Protocol *ptl = mProtocolArray[i];
			ptl->PushBuffer(mFrameBuf, readed);
		}

		mCS.Leave();
	}
	Sleep(30);
	return readed;
}

int32_t Camera3DInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	int frameLen = mFrameWidth*mFrameHeight;
	if (dwLen < mFrameLen)
	{
		return 0;
	}
	
	float agl;
	int ret = mCamera.ReadDepthCameraFrame(&agl,(unsigned short*)byBuf, (unsigned short*)(byBuf+ frameLen*2), (unsigned char*)(byBuf + frameLen*4), (unsigned char*)(byBuf + frameLen*5));
	if (ret < 0)
	{
		return 0;
	}

	return (frameLen* READ_BUF_RATE);
}

int32_t Camera3DInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	return 0;
}


