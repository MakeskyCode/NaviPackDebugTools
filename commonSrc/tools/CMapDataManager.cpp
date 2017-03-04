/************************************************************
* Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
* FileName: CFileName.cpp
* Author: Inmotion
* Version:
* Date: 2016/03/04
* Description: 文件名生成
************************************************************/
#include "CMapDataManager.h"
#include "tools/file_dir.h"
#include "tools/LogFile.h"
using namespace std;
using namespace shelly;

const string sensor_suffix[8] =
{
	".lidar_",
	".rgbd_",
	".collision_",
	".ground_",
	".ultrasonic_",
	".navigation_",
};

const string file_type_name[8] =
{
	"rawlog",
	"simplemap",
	"event",
};

CMapDataManager::CMapDataManager()
{
#if NAVIPACK_WIN
	divide_ch_ = '\\';
#else
	divide_ch_ = '/';
#endif

	verbse = true;
	all_map_file_ready_ = false;

	for (int i = 0; i < 8; i++)
	{
		map_suffix_[i] = sensor_suffix[i];
	}
}

CMapDataManager::~CMapDataManager()
{
	map_data_structure_ini_file_.WriteParam();
}

void CMapDataManager::ChangeMapDir(int num)
{
	if (num < 0) return;
	map_ini_data_->cur_file_index = num;
	map_data_structure_ini_file_.WriteParam();

	ValidateCurrentFileName();
}

void CMapDataManager::Init(TMapSensorInfo info, std::string dir_name)
{
	parent_dir_ = GetProjectDirectroy()+dir_name;

	//创建或者打开配置文件
	if (!FileDirectoryExists(parent_dir_))
	{
		CreateFileDirectory(parent_dir_);
	}

	string ini_file = GetProjectDirectroy() + "map_data_structure_ini_file.ini";
	map_data_structure_ini_file_.InitFileName(ini_file);
	map_ini_data_ = &map_data_structure_ini_file_.data;
	map_ini_data_->map_required = info;

	map_name_prefix_ = "shelly";
}

void CMapDataManager::ValidateCurrentFileName()
{
	char dir_tmp[50];

	sprintf(dir_tmp, "%s_%d",
		map_name_prefix_.c_str(),
		map_ini_data_->cur_file_index);


	map_file_name_ = string(dir_tmp);
	current_map_file_dir_ = parent_dir_ + divide_ch_ + map_file_name_;

	if (verbse) LOGD("map_file_name_:%s sub_dir_:%s\n", map_file_name_.c_str(), current_map_file_dir_.c_str());

	if (!FileDirectoryExists(current_map_file_dir_))
	{
		CreateFileDirectory(current_map_file_dir_);
	}
	cur_map_file_name_prefix_ = current_map_file_dir_ + divide_ch_ + map_file_name_;
}


bool CMapDataManager::ReadMapDataFileList()
{
	ValidateCurrentFileName();

	all_map_file_ready_ = true;
	map_ini_data_->rawlog_exist.data = 0;
	map_ini_data_->simplemap_exist.data = 0;

	for (unsigned char i = 0; i < 8; i++)
	{
		unsigned char bit_flag = (0x01 << i);
		string file_name;
		file_name = cur_map_file_name_prefix_ + sensor_suffix[i] + "simplemap";
		if (!FileDirectoryExists(file_name))
		{
			//LOGW("%s do not exist!\n", file_name.c_str());
			if (map_ini_data_->map_required.data&bit_flag)
			{
				all_map_file_ready_ = false;
			}
		}
		else
		{
			map_ini_data_->simplemap_exist.data |= bit_flag;
			LOGD("%s is ready!\n", file_name.c_str());
		}

	}

	return all_map_file_ready_;
}

bool CMapDataManager::GetMapFileReady()
{
	return all_map_file_ready_;
}

string CMapDataManager::GetCurMapFileNamePrefix(ESensorIndex sensor_index)
{
	return (cur_map_file_name_prefix_ + sensor_suffix[sensor_index]);
}

/*---------------------------------------------------------------
CreateFileName
---------------------------------------------------------------*/
string CMapDataManager::CreateFileNameByTime(ESensorIndex sensor_type, string file_type)
{
	//根据文件类型构建文件夹
	string sub_dir = current_map_file_dir_ + divide_ch_+file_type;
	if (!FileDirectoryExists(sub_dir))
	{
		LOGD("file do not exist!\n");
		CreateFileDirectory(sub_dir);
	}

	//利用时间构建文件名称
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char cur_time_ch[100];
	sprintf(cur_time_ch, "%04u%02u%02u%02u%02u%02u", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon,
		timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	string file_name = sub_dir + divide_ch_ + string(cur_time_ch) + sensor_suffix[sensor_type] + file_type;
	return file_name;
}

string shelly::GetFileTypeName(EFileType file_type)
{
	return string(file_type_name[file_type]);
}
