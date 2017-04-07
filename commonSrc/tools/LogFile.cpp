
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

#define PRINT_LEVEL 0

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
	//return;
	char buf[1024];
	const char *header[] = { "[INFO] ","[WARNING] ","[ERROR] ","[DEBUG] ","[FATAL] " };

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
			switch (log_level)
			{
			case L_DEBUG:
				SetConsoleTextAttribute(console_hwnd, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE );
				break;
			case L_WARNING:
				SetConsoleTextAttribute(console_hwnd, FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			case L_ERROR:
				SetConsoleTextAttribute(console_hwnd, FOREGROUND_RED | FOREGROUND_INTENSITY);
				break;
			default:
				SetConsoleTextAttribute(console_hwnd, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
				break;
			}
#else
			switch (log_level)
			{
			case L_DEBUG:
				printf(LIGHT_GRAY);
				break;
			case L_WARNING:
				printf(LIGHT_BLUE);
				break;
			case L_ERROR:
				printf(RED);
				break;
			default:
				printf(LIGHT_BLUE);
				break;
			}
#endif
			if (log_level != L_INFO)
			{
		//		printf("%s", header[log_level]);
		//		printf(" --> %s\n", buf);
				//static FILE* file = fopen("log.txt", "w");
				//fprintf(file, "%s --> %s\n",header[log_level], buf);
				//fflush(file);
			}

#ifdef USE_WEB_SERVER
			if (WsLogServer::Instance()) {
				if (log_level != L_DEBUG)//INFO
				{
					WsLogServer::Instance()->Log(log_level, buf);
				}
			}
#endif
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


