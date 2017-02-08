#ifndef  __CRITICAL_SECTION_H
#define  __CRITICAL_SECTION_H

#ifdef NAVIPACK_WIN
#include <windows.h>
#else
#include <pthread.h>
#endif

class CriticalSection
{
public:
	CriticalSection();
	~CriticalSection();

	void Enter();
	void Leave();

private:
#ifdef NAVIPACK_WIN
	CRITICAL_SECTION mCS;
#else
	pthread_mutex_t mCS;
#endif
};

#endif