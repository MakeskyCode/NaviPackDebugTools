#include  "tools/CriticalSection.h"



CriticalSection::CriticalSection()
{
#ifdef NAVIPACK_WIN
	InitializeCriticalSection(&mCS);
#else
	pthread_mutex_init(&mCS, NULL);
	//mCS = PTHREAD_MUTEX_INITIALIZER;
#endif
}


CriticalSection::~CriticalSection()
{
#ifdef NAVIPACK_WIN
	DeleteCriticalSection(&mCS);
#else
	//
#endif
}

void CriticalSection::Enter()
{
#ifdef NAVIPACK_WIN
	EnterCriticalSection(&mCS);
#else
	pthread_mutex_lock(&mCS);
#endif
}

void CriticalSection::Leave()
{
#ifdef NAVIPACK_WIN
	LeaveCriticalSection(&mCS);
#else
	pthread_mutex_unlock(&mCS);
#endif
}
