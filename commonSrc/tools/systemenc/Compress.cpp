#include "Compress.h"
int RunLenghEncoding(void* dataIn, uint32_t inLen, void* dataOut, uint8_t oneMaxLen)
{
	uint8_t* In = (uint8_t*)dataIn;
	uint8_t* Out = (uint8_t*)dataOut;
	int tol = 1;
	int len = 0;

	int i;
	for (i = 1; i < inLen; i++)
	{
		if (In[i] == In[i - 1])
		{
			tol++;
		}
		else
		{
			while (tol > oneMaxLen)
			{
				Out[len] = In[i - 1];
				len++;
				Out[len] = oneMaxLen;
				len++;
				tol -= oneMaxLen;
			}
			Out[len] = In[i - 1];
			len++;
			Out[len] = tol;
			len++;
			tol = 1;
		}
	}

	while (tol > oneMaxLen)
	{
		Out[len] = In[inLen - 1];
		len++;
		Out[len] = oneMaxLen;
		len++;
		tol -= oneMaxLen;
	}
	Out[len] = In[inLen - 1];
	len++;
	Out[len] = tol;
	len++;

	return len;
}


int RunLenghDecoding(const void* dataIn, uint32_t inLen, void* dataOut, uint32_t maxOutLen)
{
	uint8_t* pIn = (uint8_t*)dataIn;
	uint8_t* pOut = (uint8_t*)dataOut;
	uint32_t decodeLen = 0;
	for (int i = 0; i < inLen; i += 2)
	{
		uint8_t size = pIn[i+1];
		while (size--)
		{
			pOut[decodeLen++] = pIn[i];
		}
	}
	return decodeLen;
}