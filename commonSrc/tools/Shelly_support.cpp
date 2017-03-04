//#include "stdafx.h"
#include <vector>
#include <string>
#include <cstdarg>
#include "tools/Shelly_support.h"
// #include "serial_communication/compress/CFileInputStream.h"
// #include "serial_communication/compress/CFileOutputStream.h"

#include "tools/file_dir.h"
#ifdef SHELLY_OS_WINDOWS
	#include <conio.h>
	#include <windows.h>
	#include <process.h>
	#include <tlhelp32.h>
	#include <sys/utime.h>
	#include <io.h>
	#include <direct.h>
#else
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <termios.h>
	#include <unistd.h>
	#include <sys/select.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/time.h>
	#include <time.h>
	#include <unistd.h>
	#include <utime.h>
	#include <errno.h>
	#include <signal.h>

#define uint64_t unsigned long long
#endif

using namespace std;
using namespace shelly;




// A sprintf-like function for std::string 
string shelly::format(const char *fmt, ...)
{
	if (!fmt) return string("");

	int   result = -1, length = 1024;
	vector<char> buffer;
	while (result == -1)
	{
		buffer.resize(length + 10);

		va_list args;  // This must be done WITHIN the loop
		va_start(args, fmt);
		result = vsnprintf(&buffer[0], length, fmt, args);
		va_end(args);

		// Truncated?
		if (result >= length) result = -1;
		length *= 2;
	}
	string s(&buffer[0]);
	return s;
}
//
//unsigned __int64 shelly::time_tToTimestamp(const double &t)
//{
//	return (unsigned __int64)(t*10000000.0) + ((unsigned __int64)116444736 * 1000000000);
//}

//unsigned __int64  shelly::GetCurrentTick()
//{
//#ifdef SHELLY_OS_WINDOWS
//	FILETIME		t;
//	GetSystemTimeAsFileTime(&t);
//	return (((unsigned __int64)t.dwHighDateTime) << 32) | ((unsigned __int64)t.dwLowDateTime);
//#else
//	timespec  tim;
//	clock_gettime(CLOCK_REALTIME, &tim);
//	return time_tToTimestamp(tim.tv_sec) + tim.tv_nsec / 100;
//#endif
//}

bool shelly::vectorToBinaryFile(const vector<char> &vec, const std::string &fileName)
{
	try
	{
// 		CFileOutputStream	of(fileName);
// 		if (!vec.empty())
// 			of.WriteBuffer(&vec[0], sizeof(vec[0])*vec.size());
		return true;
	}
	catch (...) { return false; }
}

/*---------------------------------------------------------------
getFileSize
---------------------------------------------------------------*/
unsigned __int64 shelly::getFileSize(const std::string &fileName)
{
#if defined(NAVIPACK_WIN)
	// Visual Studio:
	struct __stat64 filStat;
	std::wstring b(fileName.begin(), fileName.end());
	if (_wstat64(b.c_str(), &filStat))
		return unsigned __int64(-1);
	else	return unsigned __int64(filStat.st_size);
#else
	// The rest of the world:
	struct stat filStat;
	if (stat(fileName.c_str(), &filStat))
		return (uint64_t)(-1);
	else	return (uint64_t)(filStat.st_size);
#endif
}

/*---------------------------------------------------------------
getTempFileName
---------------------------------------------------------------*/
std::string shelly::getTempFileName()
{
	if (!FileDirectoryExists("data_tmp"))
	{
		CreateFileDirectory("data_tmp");
	}
#ifdef SHELLY_OS_WINDOWS
	FILETIME		tt;
	GetSystemTimeAsFileTime(&tt);
	const UINT uniq = static_cast<UINT>(tt.dwLowDateTime);
	char	TMP_PATH[MAX_PATH];
	char	tmpPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tmpPath);
	GetTempFileNameA(tmpPath, "shelly", uniq, TMP_PATH);
	return std::string("data_tmp/shelly_tempXXXXXX");
#else
	char tmp[] = "data_tmp/shelly_tempXXXXXX";
	int fd;
	fd = mkstemp(tmp);
	assert(fd >= 0);
	close(fd);
	return std::string(tmp);
#endif
}

/*---------------------------------------------------------------
getTempFileName
---------------------------------------------------------------*/
std::string shelly::getTempFileNameD()
{
	if (!FileDirectoryExists("data_tmp"))
	{
		CreateFileDirectory("data_tmp");
	}
#ifdef SHELLY_OS_WINDOWS
	FILETIME		tt;
	GetSystemTimeAsFileTime(&tt);
	const UINT uniq = static_cast<UINT>(tt.dwLowDateTime);
	char	TMP_PATH[MAX_PATH];
	char	tmpPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tmpPath);
	GetTempFileNameA(tmpPath, "shellyd", uniq, TMP_PATH);
	return std::string("data_tmp/shelly_tempXXXXXX");
#else
	char tmp[] = "data_tmp/shelly_tempXXXXXX";
	int fd;
	fd = mkstemp(tmp);
	assert(fd >= 0);
	close(fd);
	return std::string(tmp);
#endif
}