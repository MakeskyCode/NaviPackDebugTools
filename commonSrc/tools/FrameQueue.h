
#ifndef __FRAME_QUEUE_H__
#define __FRAME_QUEUE_H__

#include <inttypes.h>

#ifdef NAVIPACK_WIN
#include <windows.h>
#else
#include <pthread.h>
#endif


class FrameQueue
{
public:
	FrameQueue(int32_t max_length, int32_t max_size);
	~FrameQueue();

	bool Push(const uint8_t *data_in, int32_t in_len, bool over_write = true);
	bool Pop(uint8_t *data_out, int32_t *out_len);
	bool IsFull();
	bool IsEmpty();
	int32_t Size();

private:

	uint8_t **mBuffer;
	uint32_t mHeader;
	uint32_t mTail;
	int32_t mRealMaxSize, mMaxSize;
	int32_t mRealMaxLength, mMaxLength;

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