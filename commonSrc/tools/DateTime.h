
#ifndef  __DATE_TIME_H__
#define  __DATE_TIME_H__

#include <inttypes.h>

#ifdef NAVIPACK_WIN
#include <Windows.h>
#endif // NAVIPACK_WIN

#define INVALID_TIMESTAMP 0
#define MAX_AVERAGE_COUNT 16

class TimeInterval
{
public:
	TimeInterval();
	~TimeInterval();

	//开始计时
	void Tic(); //实际上设置了mLastTime为当前时间
				
	//返回上次Start以来的时间差，实际上调用的是Update();
	int64_t Tac();

	//更新当前时间，用上次更新的时间来计算间隔时间。
	//返回与上次调用Update之间的时间间隔
	int64_t Update();

	//计算last_time和当前时间之间的间隔，并更新内部的mLastTime
	int64_t Update(int64_t last_time);

	//获取最大时间间隔
	int64_t GetMaxInterval();
	//获取最小时间间隔
	int64_t GetMinInterval();
	//获取最新的时间间隔
	int64_t GetLastInterval();
	//获取平均时间间隔
	int64_t GetAverageInterval();
private:
	int64_t mMaxInterval, mMinInterval, mLastInterval;
	int64_t mLastIntervals[MAX_AVERAGE_COUNT];
	int64_t mLastTime;
	int32_t mIndex;
};

//获得当前的时间戳
int64_t GetCurrentTick();

typedef int64_t TimeStamp;

inline TimeStamp now() {
	return GetCurrentTick();
}

double  TimeDifference(const TimeStamp &t_first, const TimeStamp &t_later);


#endif
