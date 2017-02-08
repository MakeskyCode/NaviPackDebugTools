#include "FifoBuffer.h"
#include "string.h"


FifoBuffer::FifoBuffer(int maxsize) {
	mHeader = mTail = 0;
	mMaxSize = maxsize;
	mRealMaxSize = maxsize + 1;
	mBuffer = new unsigned char[mRealMaxSize];

	InitLocker();
}

FifoBuffer::~FifoBuffer() {
	delete mBuffer;
	ReleaseLocker();
}

int FifoBuffer::push(const unsigned char * in_buf, int len, int over_write) {	
	Lock();
	if (len < 0)
	{
		return -1;
	}
	int left = mMaxSize - unsafe_size();
	//push more than maxsize,save newest data
	if (len > mMaxSize) {
		in_buf += len - mMaxSize;
		len = mMaxSize;		
	}

	if (left < len) {
		if (over_write)//drop older data
			unsafe_pop((unsigned char *)0, len - left);
		else {
			Unlock();
			return -1;
		}			
	}
	//copy data
	int n = mRealMaxSize - mTail;
	if (n >= len) {//do not need wrap
		memcpy(mBuffer + mTail, in_buf, len);
		mTail += len;
	}else {
		memcpy(mBuffer + mTail, in_buf, n);
		memcpy(mBuffer, in_buf + n, len - n);
		mTail = len - n;
	}
	Unlock();
	return len;
}

int FifoBuffer::unsafe_pop(unsigned char *out_buf, int len) {
	if (len > unsafe_size())
		len = unsafe_size();
	if (len == 0)
		return 0;
	int n = mRealMaxSize - mHeader;
	if (n >= len) {
		if (out_buf)
			memcpy(out_buf, mBuffer + mHeader, len);
		mHeader += len;
	}
	else {
		if (out_buf) {
			memcpy(out_buf, mBuffer + mHeader, n);
			memcpy(out_buf + n, mBuffer, len - n);
		}
		mHeader = len - n;
	}
	return len;
}

int FifoBuffer::unsafe_size() {
	return (mTail - mHeader + mRealMaxSize) % mRealMaxSize;
}

int FifoBuffer::pop(unsigned char *out_buf, int len) {
	int ret;
	Lock();
	ret = unsafe_pop(out_buf, len);
	Unlock();
	return ret;
}

int FifoBuffer::size()
{
	Lock();
	int n = unsafe_size();
	Unlock();
	return n;
}

int FifoBuffer::full() {
	Lock();
	int r = (unsafe_size() == mMaxSize);
	Unlock();
	return r;
}

int FifoBuffer::empty() {
	Lock();
	int r = (mTail == mHeader);
	Unlock();
	return r;
}
