/************************************************************
 * Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
 * FileName: CFrameList.h
 * Author: Inmotion
 * Version:
 * Date: 2016/03/04
 * Description: 通讯缓冲区类
 ************************************************************/
#ifndef _NET_FRAME_LIST_H
#define _NET_FRAME_LIST_H
#include "tools/CriticalSection.h"

typedef struct FramHeader
{
	unsigned int type;
	unsigned int data_len;
}FramHeader;

class CFrameList
{
public:
	CFrameList();
	~CFrameList();
public:
	int	 Init(int FrameNum, int FrameSize, const char *name);
	bool ClearFrame();
	bool ReadNewFrame(char *pFrame);
	int	 ReadFrame(char *pFrame);
	bool WriteFrame(const char *pFrame, int len);
	bool HasFrame();
	int	 GetFrameSize(){ return m_FrameSize; }
	bool ClearQueue();


	//char*ReadFramePtr();
	void RealseFramePtr();
private:
	int m_FrameSize;
	int m_FrameNum;
	int ReadPos;
	int WritePos;
	int m_nCurrentFrameNum;
	char *m_pMemory;


	CriticalSection    framelist_cs_;
};


#endif

