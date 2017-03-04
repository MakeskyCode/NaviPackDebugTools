#include  "tools/Thread.h"

#include "tools/windowsdef.h"
#include "tools/LogFile.h"
using namespace std;

//std::list<Thread *> Thread::mThreadList;
Thread* Thread::mThreadList[MAX_THREAD_NUM];
int Thread::mThreadCnt = 0;

Thread::Thread()
{
	mHasExited = 0;
	mRunningFlag = 0;
	mCallBack = NULL;
	mThreadHandle = INVALID_HANDLE_VALUE;

	mSuspendflag = false;
	mExitFlag = false;

#if NAVIPACK_WIN
	semSuspend = CreateSemaphore(NULL, 0, 1, NULL);
#else
	sem_init(&semSuspend, 0, 0);
#endif
}

Thread::~Thread()
{
	Exit();
}

int Thread::Create(const char * name,ThreadCallback callback, void *param)
{
	strcpy(mThreadName, name);

	mCallBack = callback;
	mUserParam = param;

	int pos = mThreadCnt++;
	if (pos < MAX_THREAD_NUM)
	{
		mThreadList[pos] = this;
	}
	//LOGD("Thread [%s] Created, Total Thread Num:%d", mThreadName, mThreadCnt);

#if NAVIPACK_WIN
	mThreadHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkThread, this, NULL, NULL);
#else
	int err = pthread_create(&mThreadHandle, NULL, WorkThread, (void*)this);
#endif
	return 0;
}

int Thread::Exit(int timeout /* = -1 */)
{
	if (mThreadHandle != INVALID_HANDLE_VALUE)
	{
		mExitFlag = true;
		Resume();
		//等待线程退出
#if NAVIPACK_WIN
		WaitForSingleObject(mThreadHandle, timeout);
		CloseHandle(mThreadHandle);
#else
		void *status;
	//	pthread_join(mThreadHandle, &status);
	//	pthread_exit(mThreadHandle, &status);
#endif
		mThreadHandle = INVALID_HANDLE_VALUE;
		return 0;
	}
	return -1;
}

int Thread::Start()
{
	//LOGD("[%s] Thread::Start", mThreadName);
	mExitFlag = 0;//gk add 2016/07/30 退出标记置为1
	if (mThreadHandle == INVALID_HANDLE_VALUE)
	{
		if (mCallBack != NULL)
            Create(mThreadName, mCallBack, mUserParam);
		else
			return -1;
	}
	Resume();
	return 0;
}

void Thread::Suspend()
{
	mSuspendflag = true;
	while (1)
	{
		if (!mRunningFlag)
		{
			break;
		}
		Sleep(1);
	}
	//LOGD("Thread Suspend success!");
}

void Thread::Resume()
{
#if NAVIPACK_WIN
	ReleaseSemaphore(semSuspend, 1, NULL);
#else
	sem_post(&semSuspend);
#endif
}

void Thread::WorkThreadLoop()
{
	//LOGD( "Thread [%s] Started", mThreadName);

	if (strcmp(mThreadName, "mWorkerThread IdleCommander") == 0)
	{
		LOGD("Idle Commander: mRunningFlag:%d\n", mRunningFlag);
	}

	if (mCallBack)
	{
		while (1)
		{
			mRunningFlag=false;
#if NAVIPACK_WIN
			WaitForSingleObject(semSuspend, INFINITE);
#else
			sem_wait(&semSuspend);
#endif
			if (mExitFlag)
			{
				return;
			}
			mRunningFlag = true;

			while (!mSuspendflag)
			{
				if (mExitFlag)
				{
					//LOGD("[%s] mExitFlag is true , WorkThreadLoop exit", mThreadName);
					return;
				}
				
				mTimeInterval.Tic();
				if (-1 == mCallBack(mUserParam))
				{
					mExitFlag = true;
					//LOGD("[%s] mCallBack return -1 ,WorkThreadLoop exit", mThreadName);
					return;
				}
				mTimeInterval.Tac();

				Sleep(1);
			}
		}
	}
}

void Thread::GetRunTime(int64_t *last, int64_t *avg, int64_t *max, int64_t *min)
{
	if (last) *last = mTimeInterval.GetLastInterval();
	if (avg) *avg = mTimeInterval.GetAverageInterval();
	if (max) *max = mTimeInterval.GetMaxInterval();
	if (min) *min = mTimeInterval.GetMinInterval();
}

void Thread::DisplayThreadsTimeData()
{
	LOGD( "\n//---------------- Threads Time Interval (us) ----------------//");
	for (int i = 0; i < mThreadCnt; i++)
	{
		int64_t last, avg, max, min;
		mThreadList[i]->GetRunTime(&last, &avg, &max, &min);
		LOGD( "%s", mThreadList[i]->mThreadName);
		LOGD( "last = %lld, avg = %lld, max = %lld, min = %lld", last, avg, max, min);
	}
	LOGD( "\n\n");
}