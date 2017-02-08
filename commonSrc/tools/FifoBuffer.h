
#ifndef __FIFO_BUFFER_H__
#define __FIFO_BUFFER_H__

#ifdef NAVIPACK_WIN
#include <windows.h>
#else
#include <pthread.h>
#endif

class FifoBuffer
{
public:
	FifoBuffer(int MaxSize);
	~FifoBuffer();

	//Push buffer to FiFo
	//@param in_buf The buffer to push
	//@param len The length of in_buf
	//@param over_write if over_write == 1 and the left space of fifo is not enough, the data in fifo will been over_write by new data
	//@return if over_write == 0 and there are not enough data left, this function will return -1, else will return the len;
	int push(const unsigned char * in_buf, int len, int over_write);
	//Pop buffer from FiFo
	//@param out_buf The buffer to pop to
	//@param len The length of out_buf
	//@return the actual length poped;
	int pop(unsigned char *out_buf, int len);
	
	//Get the size of the data in FIFO
	//@return the size of the data in FIFO
	int size();
	//Check if the FIFO is full
	//@return 1 is full, 0 is not full
	int full();
	//Check if the FIFO is empty
	//@return 1 is empty, 0 is not empty
	int empty();

private:

	int unsafe_size();
	int unsafe_pop(unsigned char *out_buf, int len);

	int mHeader, mTail;
	int mMaxSize, mRealMaxSize;
	unsigned char *mBuffer;

#ifdef NAVIPACK_WIN
	CRITICAL_SECTION mLocker;
	void InitLocker() 
	{
		InitializeCriticalSection(&mLocker); 
	}
	void Lock() 
	{
		EnterCriticalSection(&mLocker); 
	}
	void Unlock() { LeaveCriticalSection(&mLocker); }
	void ReleaseLocker() { DeleteCriticalSection(&mLocker); }
#else
	pthread_mutex_t mLocker;
	void InitLocker() { pthread_mutex_init(&mLocker, 0); }
	void Lock() { pthread_mutex_lock(&mLocker); }
	void Unlock() { pthread_mutex_unlock(&mLocker); }
	void ReleaseLocker() { pthread_mutex_destroy(&mLocker); }
#endif
};

#endif
