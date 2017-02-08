#ifndef LOGFILE__H
#define LOGFILE__H

typedef enum
{
	L_INFO=0,
	L_DEBUG,
	L_WARNING, 
	L_ERROR, 
	L_FATAL,
	L_LOG_LEVEL_LAST
}LogLevel;

#ifndef BUILD_ANDROID_APP
#define LOGI(...) println(L_INFO, 0, 0, __VA_ARGS__)
#define LOGD(...) println(L_DEBUG, 0, 0, __VA_ARGS__)
#define LOGW(...) println(L_WARNING, 0, 0, __VA_ARGS__)
#define LOGE(...) println(L_ERROR, 0, 0, __VA_ARGS__)
#define LOGF(...) println(L_FATAL, 0, 0, __VA_ARGS__)
#else
#include <jni.h>
#include <android/log.h>
#define LOG_TAG "NaviPackSdkJni"
#ifdef BUILD_ANDROID_DEBUG
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#else
#define LOGD(...)  
#define LOGI(...) 
#define LOGW(...)  
#define LOGE(...)  
#define LOGF(...)  
#endif
#endif
#define LOG(...) printc(__VA_ARGS__)

void printc(const char* pszContent, ...);
void println(LogLevel loglevel, long long uuid, long long userid, const char* pszContent, ...);
void PrintBuf(const void *pPos, unsigned int MaxLength, const char *name);
#endif	// __LOGFILE__H__