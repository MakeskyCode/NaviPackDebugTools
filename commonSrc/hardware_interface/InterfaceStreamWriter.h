
#ifndef __FILE_STREAM_WRITER_H__
#define __FILE_STREAM_WRITER_H__

#include "inttypes.h"
#include "stdio.h"
#include "tools/FifoBuffer.h"
#include "tools/CriticalSection.h"
#include "Protocol.h"
#include "tools/Thread.h"
#include "tools/Event.h"

//8 Bytes TimeStemp + 4Bytes ID + 4 Bytes DataLen + Data..
//应该是个单例
#define MAX_WRITER_FIFO MAX_INTERFACE_ID

typedef struct {
	uint64_t time;
	uint32_t id;
	uint32_t len;
}SectionHeader;

class InterfaceStreamWriter
{

public:
	InterfaceStreamWriter(const char *file_name);
	~InterfaceStreamWriter();

	int Write(int id,const char * buffer, int len);


private:
	FILE *mFile;
	char mFileName[128];

	FifoBuffer *mFifo;
	int64_t mLastWriteTime;
	uint8_t mBuffer[4096];

	int WriteToFifo(SectionHeader header, const char *buffer);
	Thread mWriteThread;
	Event mWriteEvent;
	static int WriteFileThreadLoop(void *param);
	
	CriticalSection mCs;
};


#endif
