/************************************************************
 * Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
 * FileName: file_dir.cpp
 * Author: Inmotion
 * Version:
 * Date: 2016/03/04
 * Description: 文件夹处理类
 ************************************************************/
#include <tools/file_dir.h>
#include "tools/LogFile.h"
#include "stdio.h"
#include <algorithm>
#include <sstream>
#include <string>
#if NAVIPACK_WIN
#include <windows.h>
#include <io.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#endif
#include "tools/systemenc/md5.h"

using namespace std;
#if NAVIPACK_WIN
char divideChar = '\\';
#else
char divideChar = '/';

#endif

#if NAVIPACK_WIN
void getFiles(string path, string exd, vector<string>& files)
{
	if (!FileDirectoryExists(path))
		CreateFileDirectory(path);

	long   hFile = 0;
	struct _finddata_t fileinfo;
	string pathName, exdName;

	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("\\").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					//files.push_back(pathName.assign(path).append("\\").append(fileinfo.name))
					files.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
#endif


void get_extension(char *file_name, char *name, char *extension)
{
	int i = 0, length;
	length = strlen(file_name);
	while (file_name[i])
	{
		if (file_name[i] == '.')
			break;
		else
			name[i] = file_name[i];
		i++;
	}
	if (i < length)
		strcpy(extension, file_name + i + 1);
	else
		strcpy(extension, "\0");
}

/*---------------------------------------------------------------
basic function
---------------------------------------------------------------*/
bool  FileDirectoryExists(string path)
{
#if NAVIPACK_WIN
	return 0 == _access(path.c_str(), 0x00);
#else
	return 0 == access(path.c_str(), 0x00);
#endif
}

bool  CreateFileDirectory(string dir_name)
{
#if NAVIPACK_WIN
	::CreateDirectory(dir_name.c_str(), NULL);
#else
	mkdir(dir_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	return true;
}


u64 GetFileNameByTime()
{
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//LOGD("%4d-%02d-%02d %02d:%02d:%02d\n", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon,timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	char cur_time_ch[100];
	sprintf(cur_time_ch, "%04u-%02u-%02u_%02uh%02um%02us", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon,
		timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);


	u64 num;
	num = 1900 + timeinfo->tm_year;
	num = num * 100 + timeinfo->tm_mon + 1;
	num = num * 100 + timeinfo->tm_mday;
	num = num * 100 + timeinfo->tm_hour;
	num = num * 100 + timeinfo->tm_min;
	num = num * 100 + timeinfo->tm_sec;

	LOGD("  %s  %lld", cur_time_ch, num);
	return num;
}

#define MAX_FILE_NUM_EXIST 10
void DeleteExtraFiles(std::string file_dir,std::vector<u64> &name_list, std::string &ex_str)
{
	char divide_ch_;
#if NAVIPACK_WIN
	divide_ch_ = '\\';
#else
	divide_ch_ = '/';
#endif
	//读取列表
	vector<u64> name_time_list;
	readFileList(file_dir.c_str(), name_time_list, ex_str);

	//排序
	sort(name_time_list.begin(), name_time_list.end());

	//删除多余文件
	int size = name_time_list.size();
	if (size > MAX_FILE_NUM_EXIST)
	{
		for (int i = 0; i < size-MAX_FILE_NUM_EXIST; i++)
		{
			stringstream ss;
			string name_num;
			ss << name_time_list[i];
			ss >> name_num;
			string file_name = file_dir + divide_ch_ + name_num + "." + ex_str;
			/* delete the file */
			if (remove(file_name.c_str()) == 0)
				LOGD("Removed %s.\n", file_name.c_str());
			else
				perror("remove");
		}
	}

	for (int i = 0; i < MAX_FILE_NUM_EXIST; i++)
	{
		if (name_time_list.size()>0)
		{
			u64 num = name_time_list.back();
			name_time_list.pop_back();
			name_list.push_back(num);
		}
		else
		{
			break;
		}
	}
}


std::string GetProjectDirectroy()
{


	char divide_ch_;
	int n;
#if NAVIPACK_WIN
		divide_ch_ = '\\';
#else
		divide_ch_ = '/';
#endif
	char *file_path_getcwd;
#if NAVIPACK_WIN
	file_path_getcwd = new char[MAX_PATH];
	n = GetModuleFileName(NULL, file_path_getcwd, MAX_PATH);
	//LOGD("AppPath: %s", file_path_getcwd);
#else
#define FILEPATH_MAX (1024)
	file_path_getcwd = new char[FILEPATH_MAX];
	//获得可执行文件当前路径
	n = readlink("/proc/self/exe", file_path_getcwd, FILEPATH_MAX);
#endif

	while (file_path_getcwd[n] != divide_ch_)
		n--;
	file_path_getcwd[n + 1] = 0;

	int len = strlen(file_path_getcwd);
	string project_directroy_ = string(file_path_getcwd);
	if (file_path_getcwd[len - 1] != divide_ch_)
		project_directroy_ += divide_ch_;

	if (file_path_getcwd)
		delete[] file_path_getcwd;
	return project_directroy_;
}

#if NAVIPACK_WIN
bool readFileList(const char *basePath, vector<u64> &name_time_list, std::string &extension_str)
{
	//对返回值进行清零
	name_time_list.clear();
	extension_str = "";

	vector<string> files;
	//得到文件夹下面所有文件
	getFiles(basePath, "", files);
	int size = files.size();
	if (size == 0)
		return false;

	LOGD("\n[File List]:\n");
	char file_name[40];
	for (int i = 0; i < size; i++)
	{
		strcpy(file_name, files[i].c_str());
		char name[40] = { "" }, ex[40] = { "" };
		get_extension(file_name, name, ex);

		LOGD("%s\n",file_name);

		u64 name_num = atoll(name);
		name_time_list.push_back(name_num);
		extension_str = string(ex);
	}
	LOGD("\n");
	return true;
}
#else
bool readFileList(const char *basePath, vector<u64> &name_time_list, std::string &extension_str)
{
	//对返回值进行清零
	name_time_list.clear();
	extension_str = "";

	//得到文件夹下面所有文件
	DIR *dir;
	struct dirent *ptr;
	if ((dir = opendir(basePath)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}
	
	LOGD("\n[File List]:\n");
	char file_name[40];
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
		if (ptr->d_type == 8)    ///file
		{
			strcpy(file_name,ptr->d_name);
			char name[40] = { "" }, ex[40] = { "" };
			get_extension(file_name, name, ex);

			LOGD("%s\n",file_name);

			u64 name_num = atoll(name);
			name_time_list.push_back(name_num);
			extension_str = string(ex);
		}
	}

	LOGD("\n");
	closedir(dir);
	return true;
}
#endif
char* GetRecordDataDir()
{
	const int pathLenMax = 256;
	char* dir = new char[pathLenMax];
	char *workDir = GetSelfExeCutaleDir();
	memset(dir, 0, pathLenMax);
	memcpy(dir, workDir, strlen(workDir));

	dir[strlen(workDir)] = divideChar;

	char file[] = "RawData";

	memcpy(dir + strlen(workDir) + 1, file, sizeof(file));
	dir[strlen(workDir) + 1 + strlen(file)] = divideChar;

	delete[] workDir;
	if (!FileDirectoryExists(dir))
	{
		CreateFileDirectory(dir);
	}
	return dir;
}

char* GetTempFiles()
{
	const int pathLenMax = 256;
	char* dir = new char[pathLenMax];
	char *workDir = GetSelfExeCutaleDir();
	memset(dir, 0, pathLenMax);
	memcpy(dir, workDir, strlen(workDir));

	dir[strlen(workDir)] = divideChar;

	char file[] = "TempFiles";

	memcpy(dir + strlen(workDir) + 1, file, sizeof(file));

	delete[] workDir;
	if (!FileDirectoryExists(dir))
	{
		CreateFileDirectory(dir);
	}
	return dir;
}

char* getMapDir()
{
	const int pathLenMax = 256;
	char* dir = new char[pathLenMax];
	char *workDir = GetSelfExeCutaleDir();
	memset(dir, 0, pathLenMax);
	memcpy(dir, workDir, strlen(workDir));

	dir[strlen(workDir)] = divideChar;

	char file[] = "shelly_map";

	memcpy(dir + strlen(workDir) + 1, file, sizeof(file));

	delete[] workDir;
	if (!FileDirectoryExists(dir))
	{
		CreateFileDirectory(dir);
	}
	return dir;
}

#ifdef NAVIPACK_WIN
char* GetSelfExeCutaleDir()
{
	char *file_path_getcwd = new char[MAX_PATH];
	int n = GetModuleFileName(NULL, file_path_getcwd, MAX_PATH);
	while (file_path_getcwd[n] != '\\')
		n--;
	file_path_getcwd[n] = 0;
	return file_path_getcwd;
}
int SaveFiles(const char* path, const char* fileName, const char* data, uint32_t datalLen)
{
	return -1;
}
#else
char* GetSelfExeCutaleDir()
{
	int rval;
	char link_target[1024];
	char* last_slash;
	size_t result_length;
	char* result;
	rval = readlink("/proc/self/exe", link_target, sizeof(link_target));
	if (rval == -1)
		abort();
	else
		link_target[rval] = '\0';
	last_slash = strrchr(link_target, '/');
	if (last_slash == NULL || last_slash == link_target)
		abort();
	result_length = last_slash - link_target;
	result = new char[result_length + 1];
	strncpy(result, link_target, result_length);
	result[result_length] = '\0';
	return result;
}

int SaveFiles(const char* path, const char* fileName, const char* data, uint32_t datalLen)
{
	if (!FileDirectoryExists(path))
	{
		if (!CreateFileDirectory(path))
		{
			return -1;
		}
	}

	char* file = new char[strlen(path)+strlen(fileName)+sizeof(divideChar)];
	memcpy(file, path, strlen(path));
	file[strlen(path)] = divideChar;
	memcpy(file + strlen(path) + sizeof(divideChar), fileName, strlen(fileName));

	FILE * pFile;
	u32 lSize;

	pFile = fopen(file, "wb");
	if (pFile == NULL)
	{
		LOGE("-->open File(%s) error", file);
		return -2;
	}

	size_t result = 0;
	while (result < datalLen) {
		result += fwrite(data + result, 1, datalLen-result, pFile);
	}

	LOGD("fwrite SUCCESS %d!", datalLen);
	fclose(pFile);
	delete[] file;

	return 0;
}
#endif
//
////获取所有的文件名  
//void GetAllFiles(string path, vector<string>& files)
//{
//
//	long   hFile = 0;
//	//文件信息    
//	struct _finddata_t fileinfo;//用来存储文件信息的结构体    
//	string p;
//	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  //第一次查找  
//	{
//		do
//		{
//			if ((fileinfo.attrib &  _A_SUBDIR))  //如果查找到的是文件夹  
//			{
//				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //进入文件夹查找  
//				{
//					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
//				}
//			}
//			else //如果查找到的不是是文件夹   
//			{
//				files.push_back(p.assign(fileinfo.name));  //将文件路径保存，也可以只保存文件名:    p.assign(path).append("\\").append(fileinfo.name)  
//			}
//
//		} while (_findnext(hFile, &fileinfo) == 0);
//
//		_findclose(hFile); //结束查找  
//	}
//
//}
//
//获取特定格式的文件名 
#ifdef NAVIPACK_WIN
int readFileList(char *basePath, vector<string>& files)
{
	return -1;
}
#else
int readFileList(char *basePath, vector<string>& files)
{
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir(basePath)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
		else if (ptr->d_type == 8)    ///file
		{
			//printf("d_name:%s/%s\n", basePath, ptr->d_name);
			string fileName(ptr->d_name);
			files.push_back(fileName);
		}
		
#if(0)
		else if (ptr->d_type == 10)    ///link file
			printf("d_name:%s/%s\n", basePath, ptr->d_name);
		else if (ptr->d_type == 4)    ///dir
		{
			memset(base, '\0', sizeof(base));
			strcpy(base, basePath);
			strcat(base, "/");
			strcat(base, ptr->d_name);
			readFileList(base);
		}
#endif
	}
	closedir(dir);
	return 1;
}
#endif

char* LoadFileBuf(const char *fileName)
{
	FILE *fp = NULL;
	long size = 0;
	char *buf = NULL;
	if ((fp = fopen(fileName, "rb")) == NULL)
		return NULL;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);	//这两句用于获取文件的长度
	rewind(fp);			//指向开头
	buf = new char[size + 1];
	buf[size] = '\0';
	if (fread(buf, size, 1, fp) < 1)
	{
		free(buf);
		buf = NULL;
	}
	fclose(fp);
	return buf;
}