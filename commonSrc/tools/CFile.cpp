#include <stdio.h>
#include <cstdlib>
#include "CFile.h"
#include "file_dir.h"
#include "LogFile.h"
#ifndef NULL
#define NULL 0
#endif

#if NAVIPACK_WIN
char g_divideChar = '\\';
#else
char g_divideChar = '/';
#endif	

const char* tempFileName = "tempFile";

CFile::CFile()
{
	m_pFile = NULL;
	memset(mFileName,0,sizeof(mFileName));
	memset(mFileDirName, 0, sizeof(mFileDirName));
	memset(mFileFullName, 0, sizeof(mFileFullName));
}

CFile::~CFile()
{

}

//打开文件
int CFile::Open(const char* fileDir, const char* fileName, OpenMode mode)
{
	Close(0, 0);
	LOGD("Open file %s / %s", fileDir, fileName);
	if (m_pFile != NULL)
	{
		LOGE("CFile::Open filed file is now using");
		return -1;
	}
	if (!FileDirectoryExists(fileDir))
	{
		LOGE("FileDirectory is not Exists! try to create it!!");
		if (!CreateFileDirectory(fileDir))
		{
			LOGE("Create FileDirectory failed!!");
			return -2;
		}
		else
		{
			LOGE("Create FileDirectory success!!");
		}
	}
	else {
		LOGE("FileDirectory is Exists!");
	}

	memcpy(mFileDirName,fileDir,strlen(fileDir));
	mFileDirName[strlen(fileDir)] = '\0';
	LOGE("mFileDirName = %s", mFileDirName);

	if (mode == WRITE)
	{
		if (strlen(fileName) < sizeof(mFileName)) 
		{
			memcpy(mFileName, fileName, strlen(fileName));
			mFileName[strlen(fileName)] = '\0';
		}
		else 
		{
			LOGE("fileName is too long ");
			return -5;
		}
		char* file = new char[strlen(fileDir) + strlen(tempFileName) + sizeof(g_divideChar)+1];
		memset(file, 0, (strlen(fileDir) + strlen(tempFileName) + sizeof(g_divideChar))+1);
		memcpy(file, fileDir, strlen(fileDir));
		file[strlen(fileDir)] = divideChar;
		//*(file + strlen(fileDir)) = divideChar;
		memcpy(file + strlen(fileDir) + sizeof(divideChar), tempFileName, strlen(tempFileName));
		//*(file + strlen(fileDir) + sizeof(divideChar) + strlen(tempFileName)) = '\0';
		m_pFile = fopen(file, "wb");
		if (m_pFile == NULL)
		{
			LOGE("-->open File(%s) error", file);
			return -3;
		}

		return 0;
	}
	return -4;
}
//追加内容
int CFile::AddBuf(char *buf, int len)
{
	//LOGE("CFile::AddBuf:");
	//LOGE("AddBuf %s %d", mFileName, len);
	if (m_pFile == NULL)
	{
		LOGE("m_pFile is null");
		return -1;
	}
	int writeLen = 0;
	while (writeLen < len)
	{
		//LOGD("writeLen = %d %d %p", writeLen,len,buf);
		writeLen += fwrite(buf + writeLen, 1, len - writeLen, m_pFile);
	}

	mWriteLen += writeLen;
	return writeLen;

}
//关闭文件
int CFile::Close(int needSave,int isNeedMove)
{
	LOGD("Close %s needSave = %d", mFileName, needSave);
	int isNeedSave = needSave;
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	else
	{
		return -1;
	}


	if (!isNeedMove)
	{
		LOGI("CFile::Close(int needSave,int isNeedMove)");
		return 0;
	}

	mWriteLen = 0;
	mNeedLen = 0;
	char cmd[256];
	memset(cmd,0,sizeof(cmd));
#ifdef NAVIPACK_WIN
	sprintf(cmd,"move %s%c%s %s%c%s",mFileDirName,divideChar,tempFileName,mFileDirName, divideChar,mFileName);
#else
	sprintf(cmd, "mv %s%c%s %s%c%s", mFileDirName, divideChar, tempFileName, mFileDirName, divideChar, mFileName);
#endif
	LOGD("cmd:%s",cmd);
	sprintf(mFileFullName, "%s%c%s",mFileDirName, divideChar, mFileName);

	if (isNeedSave == 1)
	{
		system(cmd);
	}

	//why clean mFileName? --ldw
	//memset(mFileName,0,sizeof(mFileName));
	return 0;
}

char* CFile::getFileName()
{
	return mFileName;
}

char* CFile::getFullName()
{
	/*
	char tmpFullName[512];
	memset(tmpFullName, 0, sizeof(tmpFullName));
	sprintf(tmpFullName, "%s%c%s", mFileDirName, divideChar, mFileName);
	*/
	return mFileFullName;
}
