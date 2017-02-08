/*******************************************************
*  '\n' = 0x0a
*  windows，以0x0d，0x0a为回车符。
*  linux，以0x0a为回车符。
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "UnixStd.h"
#include "CfgTxtFile.h"


int ReadString(const char *FileName, const char *Key, char *pValue)
{
	pValue[0] = 0;
	
	FILE *f = fopen(FileName, "rb");
	if (!f)
		return -1;
	char buf[2048];
	
	int n = fread(buf, 1, 1024, f);
	fseek(f, 0, SEEK_SET);
	fclose(f);
	if (n>0 && n<2048)
		buf[n] = 0;
	//LOGD("config file len: %d\n", n);
	
	char *p = strstr(buf, Key); 
	if (NULL == p)
		return -1;	
	
	char *pEnd = p;
	int i = 0;
	for (i=0; i<n; i++)
	{
		if (pEnd[i]==0 || pEnd[i]=='\n' || pEnd[i]=='\r' || pEnd[i]==0x0A)
		{
			pEnd[i] = 0;
			break;
		}
	}
	
	pEnd = p;
	for (i=0;i<n;i++)
	{
		if (pEnd[i]=='=')
		{
			pEnd[i] = 0;
			//strcpy(pKey, p);
			strcpy(pValue, &pEnd[i+1]);
			break;
		}
	}
	
	return 0;
}
	
	
int WriteString(const char *FileName, const char *Key, const char *Value)
{
	char all[10*1024];
	char buf[10*1024];
	FILE *f = fopen(FileName, "rb");

	if (!f)
	{
		f = fopen(FileName, "wb");
		if (f)
		{
			sprintf(buf, "%s=%s", Key, Value);
			fwrite(buf, 1, strlen(buf), f);
			fclose(f);
		}		
		return 0;
	}
	else
	{	
		int n = fread(buf, 1, 10*1024, f);
		fclose(f);	
		buf[n] = 0;
		char *p = strstr(buf, Key); 
		if (NULL == p)
		{
			if (0==buf[0])
				sprintf(all, "%s=%s\n", Key, Value);
			else
				sprintf(all, "%s\n%s=%s\n", buf, Key, Value);

			f = fopen(FileName, "wb");
			fwrite(all, 1, strlen(all), f);
			fclose(f);
			return 0;	
		}
		
		char *pThree = NULL;
		int i = 0;
		char *pEnd = p;
		for (i=0; i<=n; i++)
		{
			if (pEnd[i]==0 || pEnd[i]=='\n' || pEnd[i]=='\r' || pEnd[i]==0x0A)
			{
				if (pEnd[i]==0)
				{
					pThree = NULL;
					break;
				}
				else
				{
					pThree = &pEnd[i+1];
					pEnd[i] = 0;
					break;
				}
			}
		}
		char line[256];
		strcpy(line, p);
		*p = 0; 
		char *pSep=strstr(line, "=");//pSep is =
		if (NULL == pSep)
			return -2;//in line, no '=' find
		*pSep = 0;
		
		if (0 == buf[0])
		{
			if (pThree)
				sprintf(all, "%s=%s\n%s", line, Value, pThree);
			else
				sprintf(all, "%s=%s\n", line, Value);
		}
		else
		{
			if (pThree)
				sprintf(all, "%s%s=%s\n%s", buf, line, Value, pThree);
			else
				sprintf(all, "%s%s=%s\n", buf, line, Value);
		}
		
	}
	
	f = fopen(FileName, "wb");
	fwrite(all, 1, strlen(all), f);
	fclose(f);
	return 0;
}


int ReadInt(const char *FileName, const char *Key, int *pValue)
{
	char sValue[64];
	sValue[0] = 0;
	ReadString(FileName, Key, sValue);
	if (0==sValue[0])
		return -1;

	*pValue = atoi(sValue);
	return 0;
}

int WriteInt(const char *FileName, const char *Key, int Value)
{   
    char sValue[64];
	sprintf(sValue, "%d", Value);
	WriteString(FileName, Key, sValue);
	return 0;
}

int ReadLongLong(const char *FileName, const char *Key, long long *pValue)
{
	char sValue[64];
	sValue[0] = 0;
	ReadString(FileName, Key, sValue);
	if (0==sValue[0])
		return -1;

	*pValue = atoll(sValue);
	return 0;
}

int WriteLongLong(const char *FileName, const char *Key, long long pValue)
{
	char sValue[64];
	sprintf(sValue, "%lld", pValue);
	WriteString(FileName, Key, sValue);
	
	return 0;
}

