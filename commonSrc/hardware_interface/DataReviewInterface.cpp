#include "DataReviewInterface.h"

FileStreamReaderInterface* DataReviewInterface::mFileStreamReaderInterface = NULL;

DataReviewInterface::DataReviewInterface(InterfaceID id,uint32_t maxTxBufferSize):HardwareInterface(id, maxTxBufferSize)
{
	
	mFifoBuffer = new FifoBuffer(1204 * 100);
	mRxThread.Start();
}

DataReviewInterface::~DataReviewInterface()
{

}

int DataReviewInterface::SeekTo(uint32_t filePos)
{
	return -1;
}

void DataReviewInterface::DataReviewCallback(uint8_t* buf, uint32_t len)
{
//	LOGD("DataReviewInterface::DataReviewCallback %d",mId);
	mFifoBuffer->push(buf,len,1);
	mRxEvent.Set();

}

int DataReviewInterface::Open(const char * file_name, uint32_t param1, uint32_t param2)
{
	int ret = 0;
	if (mFileStreamReaderInterface == NULL)
	{
		mFileStreamReaderInterface = new FileStreamReaderInterface();
		ret = mFileStreamReaderInterface->Open(file_name, param1, param2);
		if (ret < 0)
		{
			delete mFileStreamReaderInterface;
			mFileStreamReaderInterface = NULL;
			return -1;
		}
	}
	ret = mFileStreamReaderInterface->AddReviewCallback(mId, this);
	memset(mFileName,0,sizeof(mFileName));
	memcpy(mFileName, file_name, strlen(file_name));
	mIsOpened = 1;
	return ret;
}

int DataReviewInterface::Close()
{
	if (mFileStreamReaderInterface != NULL)
	{
		mFileStreamReaderInterface->RemoveReviewCallback(mId);
	}
	return 0;
}

int DataReviewInterface::ReOpen()
{
	Close();
	Open(mFileName,0,0);
	return 0;
}

int32_t DataReviewInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	if (WAIT_OBJECT_0 == mRxEvent.Wait(dwTimeOut))
	{
		int len = mFifoBuffer->pop(byBuf, dwLen);
		return len;
	}
	return 0;
}

int32_t DataReviewInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	return 0;
}