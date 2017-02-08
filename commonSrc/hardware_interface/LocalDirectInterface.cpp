#include "LocalDirectInterface.h"
#include "tools/LogFile.h"

LocalDirectInterface::LocalDirectInterface(InterfaceID id, uint32_t maxBufferSize,uint32_t maxLocalBuffersize) :HardwareInterface(id, maxBufferSize)
{
	mLocolBuf = new FifoBuffer(maxLocalBuffersize);
}

LocalDirectInterface::~LocalDirectInterface()
{

}

int LocalDirectInterface::Open(const char * file_name, uint32_t param1, uint32_t param2)
{
	strcpy(mFileName,file_name);
	mParam1 = param1;
	mParam2 = param2;
	mTxThread.Start();
	mRxThread.Start();
	return 0;
}

int LocalDirectInterface::Close()
{
	mRxThread.Exit();
	mTxThread.Exit();
	return 0;
}

int LocalDirectInterface::ReOpen()
{
	Close();
	return Open(mFileName, mParam1, mParam2);
}

int32_t LocalDirectInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	return mLocolBuf->pop(byBuf, dwLen);
}

int32_t LocalDirectInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	mLocolBuf->push(byBuf,dwLen,0);
	return 0;
}
