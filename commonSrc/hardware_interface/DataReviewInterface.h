#ifndef _DATA_REVIEW_INTERFACE_H_
#define _DATA_REVIEW_INTERFACE_H_

#include "HardwareInterface.h"
#include "tools/Thread.h"
#include "tools/FifoBuffer.h"
#include "tools/Event.h"
#include "FileStreamReaderInterface.h"
#include <stdio.h>

class DataReviewInterface : public HardwareInterface
{
public:
	DataReviewInterface(InterfaceID id,uint32_t maxTxBufferSize);
	virtual ~DataReviewInterface();

	virtual int Open(const char * file_name, uint32_t param1, uint32_t param2);
	virtual int Close();
	virtual int ReOpen();

	virtual int32_t ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1);
	virtual int32_t WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1);

	virtual InterfaceType GetInterfaceType() { return  FILE_STREAM_READER; }
	void DataReviewCallback(uint8_t* buf, uint32_t len);

	static int SeekTo(uint32_t filePos);
private:
	static FileStreamReaderInterface *mFileStreamReaderInterface;		//一个文件读写类
	FifoBuffer *mFifoBuffer;
	Event mRxEvent;
	char mFileName[128];


};

#endif