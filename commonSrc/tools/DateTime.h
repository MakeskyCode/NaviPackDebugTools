
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

	//��ʼ��ʱ
	void Tic(); //ʵ����������mLastTimeΪ��ǰʱ��
				
	//�����ϴ�Start������ʱ��ʵ���ϵ��õ���Update();
	int64_t Tac();

	//���µ�ǰʱ�䣬���ϴθ��µ�ʱ����������ʱ�䡣
	//�������ϴε���Update֮���ʱ����
	int64_t Update();

	//����last_time�͵�ǰʱ��֮��ļ�����������ڲ���mLastTime
	int64_t Update(int64_t last_time);

	//��ȡ���ʱ����
	int64_t GetMaxInterval();
	//��ȡ��Сʱ����
	int64_t GetMinInterval();
	//��ȡ���µ�ʱ����
	int64_t GetLastInterval();
	//��ȡƽ��ʱ����
	int64_t GetAverageInterval();
private:
	int64_t mMaxInterval, mMinInterval, mLastInterval;
	int64_t mLastIntervals[MAX_AVERAGE_COUNT];
	int64_t mLastTime;
	int32_t mIndex;
};

//��õ�ǰ��ʱ���
int64_t GetCurrentTick();

typedef int64_t TimeStamp;

inline TimeStamp now() {
	return GetCurrentTick();
}

double  TimeDifference(const TimeStamp &t_first, const TimeStamp &t_later);


#endif
