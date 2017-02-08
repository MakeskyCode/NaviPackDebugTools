/************************************************************
* Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
* FileName: DateTime.h
* Author: Inmotion
* Version:
* Date: 2016/03/04
* Description: 系统时间类
************************************************************/
#ifndef  CPARAM_INIT_FILE_H
#define  CPARAM_INIT_FILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "tools/LogFile.h"
#include "tools/file_dir.h"

template <class T>
class CParamIniFile
{
public:

	T data;
	std::ifstream m_fin;
	std::ofstream m_fout;
	std::string file_name_;

	CParamIniFile()
	{
	}

	void InitFileName(std::string file_name)
	{
		file_name_ = file_name;
		//如果文件不存在
		if (!FileDirectoryExists(file_name_))
		{
			WriteParam();
		}
		else
		{
			ReadParam();
		}
	};

	bool WriteParam()
	{
		m_fout.open(file_name_.c_str(), std::ios_base::out | std::ios_base::binary);
		if (m_fout.is_open())
		{
			m_fout.write((char *)&data, sizeof(T));
			m_fout.close();
			return true;
		}
		else
		{
			LOGD("Can not open the output file %s\n", file_name_.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	}

	bool ReadParam()
	{
		m_fin.open(file_name_.c_str(), std::ios_base::in | std::ios_base::binary);
		if (m_fin.is_open())
		{
			m_fin.read((char *)&data, sizeof(T));
			m_fin.close();
			return true;
		}
		else
		{
			LOGD("Can not open the output file \n");
			exit(EXIT_FAILURE);
			return false;
		}
	}
};


#endif
