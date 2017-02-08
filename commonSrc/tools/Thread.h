#ifndef __THREAD_H__
#define __THREAD_H__

#ifdef NAVIPACK_WIN
#include "windows.h"
#else
#include <pthread.h>
#include <semaphore.h>
#endif
#include <string.h>
#include <time.h>
#include "tools/LogFile.h"
#include "tools/DateTime.h"

typedef int (*ThreadCallback)(void *param);

#define MAX_THREAD_NUM	128

class Thread
{
public:
	Thread();
	~Thread();

	int Create(const char * name,ThreadCallback callback, void *param);

#if NAVIPACK_WIN
	HANDLE semSuspend;
#else
	sem_t semSuspend;
#endif

	bool mSuspendflag;

	int Exit(int timeout = -1);
	int Start();

	void Suspend();
	void Resume();

	int HasExited() { return mHasExited; }

	static int GetThreadCnt() { return mThreadCnt; }
	static Thread** GetThreadList() { return mThreadList; }
	static void DisplayThreadsTimeData();

	void GetRunTime(int64_t *last, int64_t *avg, int64_t *max, int64_t *min);

private:
	int mHasExited;
	bool mRunningFlag;
	bool mExitFlag;
	char mThreadName[128];

	TimeInterval mTimeInterval;

	//线程监控
	static Thread *mThreadList[MAX_THREAD_NUM];
	static int mThreadCnt;

	void *mUserParam;
	ThreadCallback mCallBack;
	void WorkThreadLoop();
	
#ifdef NAVIPACK_WIN
	HANDLE mThreadHandle;	
#else
	pthread_t mThreadHandle;
#endif

	static void *WorkThread(void *pParams)
	{
		Thread *work_thread = (Thread *)pParams;
		work_thread->WorkThreadLoop();
		return 0;
	}
};

#endif