/************************************************************
 * Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
 * FileName: CFrameList.cpp
 * Author: Inmotion
 * Version:
 * Date: 2016/03/04
 * Description: 通讯缓冲区类
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CFrameList.h"
#include <iostream>
#include "tools/LogFile.h"
#define MAX_FRAME_BUF 1024*1024*10

//using namespace shelly;

CFrameList::CFrameList()
{
	m_pMemory = NULL;
	m_FrameSize=m_FrameNum=0;
	ReadPos=WritePos=0;
	m_nCurrentFrameNum=0;
}
CFrameList::~CFrameList()
{
	if (m_pMemory)
	{
		delete[] m_pMemory;
		m_pMemory = NULL;
	}
}

int CFrameList::Init(int FrameNum, int FrameSize, const char *name)
{
	if (FrameNum<3)
	{
		LOGD("error:init FrameNum<3\n");
		return -1;
	}

	//LOGD("FrameSize    :  %d\n",FrameSize);


	if (FrameSize>MAX_FRAME_BUF)
	{
		LOGD("error:init FrameSize>MAX_FRAME_BUF\n");
		return -2;
	}
	//m_pMemory = (char*)malloc(FrameSize*FrameNum);
	m_pMemory = new char[(FrameSize+4)*FrameNum]; //4 is data len
	//if (m_pMemory)
	//	LOGD("Init m_pMemory: %d*%d (%s),addr: %p\n",FrameSize,FrameNum,name,m_pMemory);
	//else
	//	LOGD("Error malloc %d*%d\n",FrameSize,FrameNum);
	m_FrameNum = FrameNum;
	m_FrameSize = FrameSize;

	ReadPos = 0;
	WritePos = 0;
//	LOGD("m_FrameNum %d m_FrameSize %d\n",m_FrameNum,m_FrameSize);
	//m_nCurrentFrameNum = 0;
	return 0;
}

bool CFrameList::ClearFrame()
{
	ReadPos=WritePos=0;
	return true;
}

//读最新的一帧
bool CFrameList::ReadNewFrame(char *pFrame)
{
	if (ReadPos==WritePos)
		return false;

	if (WritePos==0)
	{
		ReadPos=m_FrameNum-1;
		memcpy(pFrame, &m_pMemory[ReadPos*m_FrameSize], m_FrameSize);
		ReadPos = 0;
	}
	else
	{
		ReadPos = WritePos-1;
		memcpy(pFrame, &m_pMemory[ReadPos*m_FrameSize], m_FrameSize);
		ReadPos++;
	}
	return true;
}

bool CFrameList::HasFrame()//测试是否有数据
{
	if (ReadPos==WritePos)
		return false;

	if (ReadPos==m_FrameNum-1)
	{
		if (0 == WritePos)
			return false;
		return true;
	}
	else
	{
		if (ReadPos+1 == WritePos)
			return false;
		return true;
	}
}

int CFrameList::ReadFrame(char *pFrame)//正常的读一帧
{
	//CCriticalSectionLocker locker(&framelist_cs_);
    framelist_cs_.Enter();

	//LOGD("ReadFrame %d %d %d\n", ReadPos,WritePos,m_FrameNum);
    if (ReadPos == WritePos)
    {
        framelist_cs_.Leave();
        return 0;
    }


	int len=0;
	memcpy(&len, &m_pMemory[ReadPos*m_FrameSize], 4);
	memcpy(pFrame, &m_pMemory[ReadPos*m_FrameSize+4], len);

	if (ReadPos==m_FrameNum-1)
	{
		//if (0 == WritePos)
		//	return false;
		ReadPos = 0;
	}
	else
	{
		ReadPos++;
	}

    framelist_cs_.Leave();

	return len;
}

/*
char * CFrameList::ReadFramePtr()//正常的读一帧
{
	//CCriticalSectionLocker locker(&framelist_cs_);
    framelist_cs_.Enter();

	char *p=NULL;
	//LOGD("ReadFrame %d %d %d\n", ReadPos,WritePos,m_FrameNum);
	if (ReadPos == WritePos)
	{
        framelist_cs_.Leave();
		return p;
	}
	else
	{
		p = &m_pMemory[ReadPos*m_FrameSize];

        framelist_cs_.Leave();
		return p;
	}
}*/


void CFrameList::RealseFramePtr()
{
	framelist_cs_.Enter();
	
	ReadPos = 0;
	WritePos = 0;
	
    framelist_cs_.Leave();
}


bool CFrameList::WriteFrame(const char *pFrame, int len)
{
	framelist_cs_.Enter();

	if (len>m_FrameSize-4 || len<=0)
	{
		LOGD("error, len=%d), m_FrameSize=%d, WriteFrame failure\n", len, m_FrameSize);

        framelist_cs_.Leave();
		return false;
	}
	if (WritePos == m_FrameNum-1) //最底部
	{
		if (0 == ReadPos)//下一个是读指针时，不再写
		{
            framelist_cs_.Leave();
			return false;
		}
		memcpy(&m_pMemory[WritePos*m_FrameSize], &len, 4);
		memcpy(&m_pMemory[WritePos*m_FrameSize+4], pFrame, len);
		WritePos = 0;
	}
	else
	{
		if (WritePos+1 == ReadPos)//下一个是读指针时，不再写
		{
            framelist_cs_.Leave();
			return false;
		}
		memcpy(&m_pMemory[WritePos*m_FrameSize], &len, 4);
		memcpy(&m_pMemory[WritePos*m_FrameSize+4], pFrame, len);
		WritePos++;
		//LOGD("WritePos:%d\n",WritePos);
	}
	//LOGD("WriteFrame\n");
    framelist_cs_.Leave();

	return true;
}

bool CFrameList::ClearQueue()
{
	ReadPos = 0;
	WritePos = 0;
	return true;
}