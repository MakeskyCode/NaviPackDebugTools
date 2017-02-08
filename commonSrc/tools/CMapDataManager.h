/************************************************************
* Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
* FileName: CFileName.h
* Author: Inmotion
* Version:
* Date: 2016/03/04
* Description:  文件名生成
************************************************************/
#ifndef CMapDataManager_H_
#define CMapDataManager_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>

#ifdef NAVIPACK_WIN
#include <windows.h>
#include <io.h>
#include <time.h>  
#else
#include <unistd.h>  //包含了Linux C 中的函数getcwd()
#include <sys/stat.h> 
#include <string.h>
#endif

#include "tools/CParamIniFile.h"

//地图管理

enum ESensorIndex
{
	LIDAR_SENSOR_MAP = 0,
	RGBD_SENSOR_MAP,
	COLLISION_SENSOR_MAP,
	GROUND_SENSOR_MAP,
	ULTRASONIC_SENSOR_MAP,
	NAVIGATION_MAP,
};


enum EFileType
{
	FILE_RAWLOG=0,
	FILE_SIMPLE_MAP,
	FILE_EVENT_RECORD,
};

namespace shelly
{

	struct TMapSensorBits
	{
		unsigned char lidar_map : 1;
		unsigned char rgbd_map : 1;
		unsigned char collision_map : 1;
		unsigned char ground_map : 1;
		unsigned char ultrasonic_map : 1;
		unsigned char reserves : 3;
	};

	union TMapSensorInfo
	{
		unsigned char data;
		TMapSensorBits  bits;
		TMapSensorInfo()
		{
			data = 0;
		}
	};

	struct TMapManagerAction
	{
		unsigned char map_file_num;
		TMapSensorInfo map_save_info;
		unsigned char current_file_num;

		TMapManagerAction()
		{
			map_save_info.data = 0;
		}
	};


#define MAX_MAP_FILEDIR_NUM	10
	struct TMapDataStructure
	{
		unsigned char cur_file_index;

		TMapSensorInfo map_required;
		TMapSensorInfo simplemap_exist;
		TMapSensorInfo rawlog_exist;

		TMapDataStructure()
		{
			cur_file_index = 0;
			map_required.data = 0;
			simplemap_exist.data = 0;
			rawlog_exist.data = 0;
		}
	};

#define  MAX_MAP_DIR_NUM 10
	class  CMapDataManager
	{
	public:
		CMapDataManager();
		~CMapDataManager();

	private:

		char divide_ch_;
		std::string parent_dir_;
		std::string map_file_name_;
		std::string current_map_file_dir_;

		std::string cur_map_file_name_prefix_;

		std::string map_suffix_[8];

		bool verbse;

		bool all_map_file_ready_;

	public:
		std::string GetCurrentMapDir(){ return current_map_file_dir_; };
		std::string GetCurMapFileNamePrefix(ESensorIndex sensor_index);
		void Init(TMapSensorInfo info, std::string dir_name = "shelly_map");
		void ChangeMapDir(int num);
		bool GetMapFileReady();

		CParamIniFile<TMapDataStructure> map_data_structure_ini_file_;

		std::pair<std::string, std::string> map_data_list_;
		std::string map_name_prefix_;
		bool ReadMapDataFileList();
		void ValidateCurrentFileName();

		TMapDataStructure *map_ini_data_;

		std::string CreateFileNameByTime(ESensorIndex sensor_type, std::string file_type);
		
	};

	std::string GetFileTypeName(EFileType file_type);
}
#endif //FILE_NAME_H_