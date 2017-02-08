
#ifndef __FILE_STREAM_READER_INTERFACE_H__
#define __FILE_STREAM_READER_INTERFACE_H__

#include "HardwareInterface.h"
#include "tools/Thread.h"
#include "tools/FifoBuffer.h"
#include "tools/Event.h"
#include "stdio.h"

typedef void(*DataReviewCallback)(uint8_t buf,uint32_t len);
#define MAX_DATA_REVIEW_NUM MAX_INTERFACE_ID
class FileStreamReaderInterface
{
public:
	class DataReview {
	public:
		DataReview(uint32_t id, void *callback) {
			mId = id;
			mDataCallback = callback;
		}
		~DataReview() {
			mDataCallback = NULL;
		}
		uint32_t mId;
		void *mDataCallback;
	};
public:

	FileStreamReaderInterface();
	virtual ~FileStreamReaderInterface();

	// Inherited via HardwareInterface
	virtual int Open(const char * file_name, uint32_t param1, uint32_t param2);
	virtual int Close();
	virtual int ReOpen();
	virtual InterfaceType GetInterfaceType() { return FILE_STREAM_READER; }
	virtual int32_t ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1);
	virtual int32_t WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1);

	int AddReviewCallback(int id, void *param);
	void RemoveReviewCallback(int id);

private:
	DataReview* mDataReview[MAX_DATA_REVIEW_NUM];

	FILE *mFile;
	uint8_t *mBuffer;
	FifoBuffer *mFifoBuffer;
	Thread mReadThread;
	Event mRxEvent;
	int64_t mFirstFileTimeStemp, mLastFileTimeStemp, mNextFileTimeStemp, mLastRealTimeStemp;

	static int ReadThreadLoop(void *param);
	char mFileName[128];

	int ReadSection(int64_t *time_stemp, int *id, int *len,  uint8_t *buffer);

};


#endif // !__FILE_STREAM_READER_INTERFACE_H__