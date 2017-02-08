
#ifndef __HARRWARE_INTERFACE_H__
#define __HARRWARE_INTERFACE_H__

#include <stdio.h>


#ifdef NAVIPACK_WIN
#include "windows.h"
#endif


#include "Protocol.h"
#include "InterfaceStreamWriter.h"
#include "tools/Thread.h"
#include "tools/Event.h"
#include "tools/CriticalSection.h"
#include "tools/windowsdef.h"
#include "tools/FifoBuffer.h"
#include "tools/CFrameList.h"

#define MAX_PROTOCOL_NUM 4
#define MAX_FRAME_SIZE (1*1024*1024)

enum InterfaceType
{
	SERIAL_PORT,
	TCP_SERVER,
	TCP_CLIENT,
	UDP,
	FILE_STREAM_READER,
	VIRTUAL_CARRIER,
	LOCOL_DIRECT,
};



class HardwareInterface
{
public:
	HardwareInterface();
	HardwareInterface(InterfaceID id,uint32_t maxBufferSize);
	virtual ~HardwareInterface();

	virtual int Open(const char *file_name, uint32_t param1, uint32_t param2) = 0;
	virtual int Close() = 0;
	virtual int ReOpen() = 0;



	virtual InterfaceType GetInterfaceType() = 0;
	static const char * GetInterfaceTypeName(InterfaceType type);
	
	int32_t GetID() { return mId; }

	int AddRxCallBack(ProtocolType type, HardwareInterfaceCallBack func, void *param,uint32_t maxRxBufferSize);
	int RemoveRxCallBack(ProtocolType type, int id);

	int WriteData(ProtocolType type, uint8_t * points, int32_t len);
	int WriteBuf(uint8_t * points, int32_t len);

	int IsOpened() { return mIsOpened; }

	uint64_t GetTotalRxByteCount() { return mTotalRxBytes; }
	uint64_t GetTotalTxByteCount() { return mTotalTxBytes; }
	void ResetCounters();

	void EnableStreamToFile(const char *stream_file_name);
	void DisableStreamToFile();


protected:
	int mIsOpened, mSysErrCode;
	Thread mRxThread;
	Thread mTxThread;
	Event mTxEvent;
	FifoBuffer *mTxBufferFifo;
	char mFileName[128];
	int32_t mId;
	int32_t mIsNeedWriteToFile;
	CriticalSection mCS;
	Protocol *mProtocolArray[MAX_PROTOCOL_NUM];
	int32_t mProtocolArraySize;
	static InterfaceStreamWriter *mInterfaceStreamWriter;
private:
	void initInterfaceParam(uint32_t MaxTxBufferSize);
	static int32_t mIdCounter;



	uint64_t mTotalRxBytes, mTotalTxBytes;
	
	virtual int32_t ReadBufferFromInterface(uint8_t *byBuf, int32_t dwLen, int32_t dwTimeOut = -1) = 0;
	virtual int32_t WriteBufferToInterface(const uint8_t *byBuf, int32_t dwLen, int32_t dwTimeOut = -1) = 0;

	virtual int RxFun();

    static int RxThreadLoop(void *param);
	static int TxThreadLoop(void *param);
};


#endif