#include  "Event.h"

#include <new>  
#include <iostream> // for std::cerr  


#if !NAVIPACK_WIN
#include <sys/time.h>
#include <errno.h>
#include <cstdlib> // for std::exit()  

#endif
#ifdef NAVIPACK_WIN

Event::Event(uint32_t manual_reset, uint32_t init_state)
{
	mHandle = CreateEvent(NULL, manual_reset, init_state, NULL);
}
//
//Event::Event()
//{
//	mHandle = CreateEvent(NULL, 0, 0, NULL);
//}

Event::~Event()
{
	if (mHandle != INVALID_EVENT_VALUE)
		CloseHandle(mHandle);
}

void Event::Set()
{
	SetEvent(mHandle);
}

void Event::Reset()
{
	ResetEvent(mHandle);
}

int Event::Wait(uint32_t timeout)
{
	DWORD rc = WaitForSingleObject(mHandle, timeout);
	if (rc == WAIT_TIMEOUT)
		return 1;
	if (rc == WAIT_OBJECT_0)
		return 0;
	else
		return -1;
}

#else

Event::Event(uint32_t manual_reset, uint32_t init_state)
{
	mHandle= new(std::nothrow) event_t;
	mHandle->state = init_state;
	mHandle->manual_reset = manual_reset;
	pthread_mutex_init(&mHandle->mutex, NULL);
	pthread_cond_init(&mHandle->cond, NULL);
}

Event::~Event()
{
	if (mHandle != INVALID_EVENT_VALUE)
	{
		pthread_cond_destroy(&mHandle->cond);
		pthread_mutex_destroy(&mHandle->mutex);
		delete mHandle;
	}
}

void Event::Set()
{
	pthread_mutex_lock(&mHandle->mutex);

	mHandle->state = true;

	if (mHandle->manual_reset)
		pthread_cond_broadcast(&mHandle->cond);
	else
		pthread_cond_signal(&mHandle->cond);

	pthread_mutex_unlock(&mHandle->mutex);
}

void Event::Reset()
{
	pthread_mutex_lock(&mHandle->mutex);

	mHandle->state = false;

	pthread_mutex_unlock(&mHandle->mutex);
}

int Event::Wait(uint32_t timeout)
{
	int rc = 0;
	struct timespec abstime;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	abstime.tv_sec = tv.tv_sec + timeout / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (timeout % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	pthread_mutex_lock(&mHandle->mutex);

	while (!mHandle->state)
	{
		rc = pthread_cond_timedwait(&mHandle->cond, &mHandle->mutex, &abstime);
		if (rc)
		{
			if (rc == ETIMEDOUT) break;
			pthread_mutex_unlock(&mHandle->mutex);
			return -1;
		}
	}

	if (rc == 0 && !mHandle->manual_reset)
		mHandle->state = false;

	pthread_mutex_unlock(&mHandle->mutex);

	if (rc == ETIMEDOUT)
		return 1;

	return 0;
}
#endif