
#ifndef __EVENT_H__
#define __EVENT_H__

#if NAVIPACK_WIN
#include <Windows.h>
#include "inttypes.h"

#define INVALID_EVENT_VALUE INVALID_HANDLE_VALUE

#else
#include <pthread.h>
#include "inttypes.h"
typedef struct
{
	bool state;
	bool manual_reset;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}event_t;
#define INVALID_EVENT_VALUE ((event_t*)(-1))
#endif

#define IMMEDIATELY_RETUREN 0
#define INFINITE_WAIT 0xFFFFFFFF

class Event
{
public:
	//Event();
	Event(uint32_t manual_reset=0, uint32_t init_state=0);
	~Event();

	void Set();
	void Reset();
	int Wait(uint32_t timeout);

private:

	void *mData;

#ifdef NAVIPACK_WIN
	HANDLE mHandle;
#else
	event_t *mHandle;
#endif
	
};

#endif