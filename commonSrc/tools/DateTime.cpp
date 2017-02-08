/************************************************************
 * Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
 * FileName: datetime.cpp
 * Author: Inmotion
 * Version:
 * Date: 2016/03/04
 ************************************************************/

#include "DateTime.h"

#ifndef NAVIPACK_WIN
#include <sys/time.h>   
#endif


#ifdef NAVIPACK_WIN
LARGE_INTEGER mCpuFrequency = { 0 };
#endif

int64_t GetCurrentTick()
{
#ifdef NAVIPACK_WIN
	if (mCpuFrequency.QuadPart == 0)
		QueryPerformanceFrequency(&mCpuFrequency);

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return 1000000 * time.QuadPart / mCpuFrequency.QuadPart;
#else
	struct timeval time;
	gettimeofday(&time, 0);
	int64_t v = time.tv_sec;
	v *= 1000000;
	v += time.tv_usec;
	return v;
#endif
}

TimeInterval::TimeInterval()
{
	mMaxInterval = mMinInterval = mLastInterval;
	for (int i = 0; i < MAX_AVERAGE_COUNT; i++)
		mLastIntervals[i] = 0;
	mIndex = 0;
}


TimeInterval::~TimeInterval()
{

}

void TimeInterval::Tic()
{
	mLastTime = GetCurrentTick();
}

int64_t TimeInterval::Tac()
{
	return Update();
}

int64_t TimeInterval::Update()
{
	int64_t ct = GetCurrentTick();
	mLastInterval = ct - mLastTime;
	mLastTime = ct;
	mLastIntervals[mIndex++] = mLastInterval;
	if (mIndex >= MAX_AVERAGE_COUNT)
		mIndex = 0;
	if (mLastInterval > mMaxInterval)
		mMaxInterval = mLastInterval;
	if (mLastInterval < mMinInterval)
		mMinInterval = mLastInterval;
	return mLastInterval;
}

int64_t TimeInterval::Update(int64_t last_time)
{
	int64_t ct = GetCurrentTick();
	mLastTime = last_time;
	mLastInterval = ct - mLastTime;
	mLastTime = ct;
	mLastIntervals[mIndex++] = mLastInterval;
	if (mIndex >= MAX_AVERAGE_COUNT)
		mIndex = 0;
	if (mLastInterval > mMaxInterval)
		mMaxInterval = mLastInterval;
	if (mLastInterval < mMinInterval)
		mMinInterval = mLastInterval;
	return mLastInterval;
}

int64_t TimeInterval::GetMaxInterval()
{
	return mMaxInterval;
}

int64_t TimeInterval::GetMinInterval()
{
	return mMinInterval;
}

int64_t TimeInterval::GetLastInterval()
{
	return mLastInterval;
}

int64_t TimeInterval::GetAverageInterval()
{
	int64_t sum = 0;
	for (int i = 0; i < MAX_AVERAGE_COUNT; i++)
		sum += mLastIntervals[i];
	return sum / MAX_AVERAGE_COUNT;
}


/*---------------------------------------------------------------
					TimeDifference
  ---------------------------------------------------------------*/
double TimeDifference(const TimeStamp &t1, const TimeStamp &t2)
{
	return ((double)((int64_t)(t2 - t1))) / 10000000.0;

}
