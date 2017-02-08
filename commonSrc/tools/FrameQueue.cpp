#include "FrameQueue.h"
#include <memory.h>

FrameQueue::FrameQueue(int32_t max_length, int32_t max_size)
{
	InitLocker();

	mRealMaxLength = max_length + 1;
	mMaxLength = max_length;
	mRealMaxSize = max_size + 4;
	mMaxSize = max_size;

	mBuffer = new uint8_t *[mRealMaxLength];
	for (int32_t i = 0; i < mRealMaxLength; i++)
		mBuffer[i] = new uint8_t[mRealMaxSize];

	mHeader = 0;
	mTail = 0;
}

FrameQueue::~FrameQueue()
{
	Lock();
	for (int32_t i = 0; i < mRealMaxLength; i++)
		delete[] mBuffer[i];
	delete[] mBuffer;
	Unlock();

	ReleaseLocker();
}

bool FrameQueue::Push(const uint8_t * data_in, int32_t in_len, bool over_write)
{
	if (data_in == NULL || in_len <= 0)
		return false;

	if (IsFull()) {
		if (over_write)
			Pop(NULL, NULL);
		else
			return false;
	}
	in_len = in_len > mMaxSize ? mMaxSize : in_len;

	Lock();
	memcpy(mBuffer[mTail], &in_len, sizeof(int32_t));
	memcpy(mBuffer[mTail] + 4, data_in, in_len);
	mTail = (mTail + 1) % mRealMaxLength;
	Unlock();

	return true;
}

bool FrameQueue::Pop(uint8_t * data_out, int32_t * out_len)
{
	int32_t len;
	if (IsEmpty())
		return false;

	Lock();
	len = *((int32_t *)(mBuffer[mHeader]));
	if (out_len)
		*out_len = len;
	if (data_out)
		memcpy(data_out, mBuffer[mHeader] + 4, len);
	mHeader = (mHeader + 1) % mRealMaxLength;
	Unlock();

	return true;
}

bool FrameQueue::IsFull()
{
	return (Size() == mMaxLength) ? true : false;
}

bool FrameQueue::IsEmpty()
{
	return (Size() == 0) ? true : false;
}

int32_t FrameQueue::Size()
{
	int32_t size = 0;
	Lock();
	size = (mTail - mHeader + mRealMaxLength) % mRealMaxLength;
	Unlock();
	return size;
}