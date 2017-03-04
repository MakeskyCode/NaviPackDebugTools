/************************************************************
 * Copyright (C), 2013-2016, Inmotion Tech. Co., Ltd.
 * FileName: file_dir.h
 * Author: Inmotion
 * Version:
 * Date: 2016/03/04
 * Description: 文件夹处理类
 ************************************************************/
#ifndef _FILE_DIR_H
#define _FILE_DIR_H

#include <string>
#include <vector>
#include "tools/windowsdef.h"
#include "tools/error_code.h"
#include <inttypes.h>
//#include "tools//CSerialMessage.h"
#ifdef NAVIPACK_WIN
#include <windows.h>
#include <io.h>
#include <time.h>  
#else
#include <unistd.h>  //包含了Linux C 中的函数getcwd()
#include <sys/stat.h> 
#include <string.h>
#endif
using namespace std;
extern char divideChar;

bool FileDirectoryExists(std::string path);
bool CreateFileDirectory(std::string file_directory);

void get_extension(char *file_name, char *name, char *extension);
#if NAVIPACK_WIN
void getFiles(std::string path, std::string exd, std::vector<std::string>& files);
#endif

u64 GetFileNameByTime();

bool readFileList(const char *basePath, std::vector<u64> &name_time_list,std::string &extension_str);
void DeleteExtraFiles(std::string file_dir, std::vector<u64> &name_time_list, std::string &extension_str);

std::string GetProjectDirectroy();
char* GetRecordDataDir();
char* GetSelfExeCutaleDir();
char* GetTempFiles();
int SaveFiles(const char* path,const char* fileName,const char* data,uint32_t datalLen);
char* getMapDir();
//void GetAllFiles(string path, vector<string>& files);
int readFileList(char *basePath,vector<string>& files);
char* LoadFileBuf(const char *fileName);
char* LoadBinaryFileBuf_(const char *fileName, int& file_len);

#endif
