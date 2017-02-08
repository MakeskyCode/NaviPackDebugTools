#include "InterfaceStreamWriter.h"

#include "tools/DateTime.h"
#include "tools/LogFile.h"
#include "tools/windowsdef.h"

InterfaceStreamWriter::InterfaceStreamWriter(const char *file_name)
{
	memset(mFileName,0,sizeof(mFileName));

	mFifo = new FifoBuffer(1204 * 128);
	
	mLastWriteTime = 0;
	mFile = fopen(file_name, "wb");
	memcpy(mFileName, file_name,strlen(file_name));

	const char *head = "InterfaceStream";
	if (mFile) 
	{
		mCs.Enter();
		fwrite(head, strlen(head), 1, mFile);
		fflush(mFile);
		mCs.Leave();
	}
	mWriteThread.Create("StreamWriterThewad", WriteFileThreadLoop,this);
	mWriteThread.Start();
}

InterfaceStreamWriter::~InterfaceStreamWriter()
{
	if (mFile != NULL)
	{
		fclose(mFile);
		mFile = NULL;
	}


	delete mFifo;
}

int InterfaceStreamWriter::Write(int id,const char * buffer, int len)
{
	if (id >= MAX_WRITER_FIFO)
	{
		return 0;
	}
	SectionHeader header;
	header.time = now();
	header.id = id;
	header.len = len;

	mCs.Enter();
	int ret = 0;
	if (mFile)
	{
		//写入fifo
		WriteToFifo(header, buffer);
		mWriteEvent.Set();
	}
	mCs.Leave();
	return 0;
}

int InterfaceStreamWriter::WriteToFifo(SectionHeader header, const char *buffer)
{

	int ret = -1;
	//写入头
	ret = mFifo->push((const unsigned char *)&header,sizeof(header),0);
	while (ret < 0) {
		LOGE("write head --> WriteToFifo error,fifo is full!");
		mWriteEvent.Set();
		Sleep(1);
		ret = mFifo->push((const unsigned char *)&header, sizeof(header), 0);
	}
	//写入数据
	ret = mFifo->push((const unsigned char *)buffer, header.len, 0);
	while (ret < 0) {
		LOGE("write buff --> WriteToFifo error,fifo is full!");
		mWriteEvent.Set();
		Sleep(1);
		ret = mFifo->push((const unsigned char *)buffer, header.len, 0);
	}
	return 0;
	

}

int InterfaceStreamWriter::WriteFileThreadLoop(void *param)
{
	InterfaceStreamWriter *writer = (InterfaceStreamWriter *)param;
	if (0 == writer->mWriteEvent.Wait(100))
	{
		int n = writer->mFifo->pop(writer->mBuffer, sizeof(writer->mBuffer));
		if (n > 0) {
			fwrite(writer->mBuffer, n, 1, writer->mFile);
			fflush(writer->mFile);
		}
	}
	Sleep(2);
	return 0;
}



