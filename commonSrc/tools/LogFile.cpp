
#include "LogFile.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include "tools/CriticalSection.h"
#ifdef USE_WEB_SERVER
#include <WsLogServer.h>
#endif

#ifdef NAVIPACK_WIN
#include <windows.h>
#else
#define NONE         "\033[m" 
#define RED          "\033[0;32;31m" 
#define LIGHT_RED    "\033[1;31m" 
#define GREEN        "\033[0;32;32m" 
#define LIGHT_GREEN  "\033[1;32m" 
#define BLUE         "\033[0;32;34m" 
#define LIGHT_BLUE   "\033[1;34m" 
#define DARY_GRAY    "\033[1;30m" 
#define CYAN         "\033[0;36m" 
#define LIGHT_CYAN   "\033[1;36m" 
#define PURPLE       "\033[0;35m" 
#define LIGHT_PURPLE "\033[1;35m" 
#define BROWN        "\033[0;33m" 
#define YELLOW       "\033[1;33m" 
#define LIGHT_GRAY   "\033[0;37m" 
#define WHITE        "\033[1;37m"
#endif

#define PRINT_LEVEL L_INFO

CriticalSection LogoutCS;

void printc(const char* pszContent, ...)
{
	char buf[8192];
	va_list args;
	va_start(args, pszContent);
	vsprintf(buf, pszContent, args);
	va_end(args);
	LogoutCS.Enter();
	printf("%s\n", buf);
	LogoutCS.Leave();
}

void println(LogLevel log_level, long long uuid, long long userid, const char* pszContent, ...)
{
	return;
	char buf[1024];
	const char *header[] = { "[DEBUG] ","[INFO] ","[WARNING] ","[ERROR] ","[FATAL] " };
#ifdef NAVIPACK_WIN
	unsigned short color = log_level >= L_WARNING ? FOREGROUND_RED : FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
#endif
	if ((log_level >= PRINT_LEVEL) && (log_level < L_LOG_LEVEL_LAST))
	{
		va_list args;
		va_start(args, pszContent);
		vsprintf(buf, pszContent, args);
		va_end(args);
		if (buf[0])
		{
			LogoutCS.Enter();
#ifdef NAVIPACK_WIN			
			HANDLE console_hwnd = GetStdHandle(STD_OUTPUT_HANDLE);//实例化句柄
			SetConsoleTextAttribute(console_hwnd, color | FOREGROUND_INTENSITY);
#else
			if (log_level >= L_WARNING)
				printf(RED);
#endif
			printf("%s", header[log_level]);

#ifdef USE_WEB_SERVER
			if (WsLogServer::Instance()) {
				WsLogServer::Instance()->Log(log_level, buf);
			}
#else

#endif
			printf(" --> %s\n", buf);
			LogoutCS.Leave();
		}
	}
}

void PrintBuf(const void *pPos, unsigned int MaxLength, const char *name)
{
	
	LogoutCS.Enter();
	if (name && strlen(name) > 0)
		printf("%s", name);

	unsigned char cTmp;
	unsigned char *pTmp = (unsigned char*)pPos;
	unsigned int i;
	for (i = 0; i < MaxLength; i++)
	{
		if (i % 16 == 0)
			printf("\n");

		cTmp = *pTmp;
		printf("%02x ", cTmp);
		pTmp++;
	}
	printf("\n");
	LogoutCS.Leave();
}


