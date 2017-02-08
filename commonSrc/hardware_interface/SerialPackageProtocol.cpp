#include "SerialPackageProtocol.h"
#include "tools/windowsdef.h"
#include "tools/SDKProtocol.h"
//#include "tools/compress/zip.h"
#include "string.h"
#include "tools/LogFile.h"


SerialPackageProtocol::SerialPackageProtocol(int32_t interface_id, uint32_t maxPackageSize):Protocol(interface_id, maxPackageSize)
{
	mLastByte = 0;
	mBeginFlag = 0;
	mCtrlFlag = 0;
	mRevOffset = 0;
	mCheckSum = 0;

	mFrameHead = 0xAA;
	mFrameTail = 0x55;
	mFrameCtrl = 0xA5;
}


SerialPackageProtocol::~SerialPackageProtocol()
{

}

int SerialPackageProtocol::PushByte(uint8_t points)
{
	if (((points == mFrameHead) && (mLastByte == mFrameHead)) || (mRevOffset > mMaxPackageSize))
	{
		//RESET
		mRevOffset = 0;
		mBeginFlag = 1;
		mLastByte = points;
		return 0;
	}
	if ((points == mFrameTail) && (mLastByte == mFrameTail) && mBeginFlag)
	{
		mRevOffset--;
		mPackageSize = mRevOffset - 1;
		mCheckSum -= mFrameTail;
		mCheckSum -= mRxBuffer[mPackageSize];
		mLastByte = points;
		mBeginFlag = 0;
		if (mCheckSum == mRxBuffer[mPackageSize])
		{

			mCheckSum = 0;
			return 1;
		}
		else 
		{
			printf("mCheckSum error!! %d\n",mPackageSize);
		}
		mCheckSum = 0;
		return 0;
	}
	mLastByte = points;
	if (mBeginFlag)
	{
		if (mCtrlFlag)
		{
			mRxBuffer[mRevOffset++] = points;
			mCheckSum += points;
			mCtrlFlag = 0;
			mLastByte = mFrameCtrl;
		}
		else if (points == mFrameCtrl)
		{
			mCtrlFlag = 1;
		}
		else
		{
			mRxBuffer[mRevOffset++] = points;
			mCheckSum += points;
		}
	}
	return 0;
}

const uint8_t * SerialPackageProtocol::PackageBuffer(const uint8_t * buf, int32_t in_len, int32_t * out_len)
{

	int32_t i;
	uint8_t *pBuf;
	uint8_t check_sum = 0;

	if (buf == NULL)
		return NULL;

	pBuf = mTxBuffer;

	*pBuf++ = mFrameHead;
	*pBuf++ = mFrameHead;

	for (i = 0; i < in_len; i++)
	{
		if ((buf[i] == mFrameCtrl) || (buf[i] == mFrameHead) || (buf[i] == mFrameTail))
			*pBuf++ = mFrameCtrl;

		*pBuf++ = buf[i];
		check_sum += buf[i];


		if ((pBuf - mTxBuffer) > (mMaxPackageSize - 4))
			return NULL;
	}

	if ((check_sum == mFrameCtrl) || (check_sum == mFrameHead) || (check_sum == mFrameTail))
		*pBuf++ = mFrameCtrl;

	*pBuf++ = check_sum;

	*pBuf++ = mFrameTail;
	*pBuf++ = mFrameTail;

	*out_len = pBuf - mTxBuffer;

	return mTxBuffer;
}

int SerialPackageProtocol::PushBuffer(const uint8_t * points, int32_t len)
{

	//LOGW("SerialPackageProtocol push buffer mCallBackInfoArraySize %d %d\n", mCallBackInfoArraySize, len);
	//LOGW("PushBuffer %x %x %x %x %x %x %x %x %x %x %x %x", points[0], points[1], points[2], points[3], points[4],
	//	 points[5], points[6], points[7], points[8], points[9], points[10], points[11]);
	for (int i = 0; i < len; i++)
	{
		if (PushByte(points[i]))
		{
			//解包成功，判断是否需要进行解压缩
			//SdkProtocolHeader *head = (SdkProtocolHeader*)mRxBuffer;
			//if (head->startAddr == ALG_DATA_ADDR_LIDAR_MAP)//(head->compress)
			//{
			//	int decompress_buffer_len = 0;
			//	int head_len = sizeof(SdkProtocolHeader);
			//	bool ret = shelly::decompress_gz_data_static((char*)mRxBuffer + head_len, mPackageSize - head_len, (char*)mRxZipBuffer, &decompress_buffer_len);
			//	if (ret)
			//	{
			//		memcpy(mRxBuffer+head_len,mRxZipBuffer,decompress_buffer_len);
			//		mPackageSize = head_len + decompress_buffer_len;
			//	}
			//	else
			//	{
			//		LOGD("[ERROR]decompress failed!\n");
			//		continue;
			//	}
			//}

			//LOGW("push buffer mCallBackInfoArraySize %d\n", mCallBackInfoArraySize);
			for (int j = 0; j < mCallBackInfoArraySize; j++)
			{
				if (mCallBackInfoArray[j].func)
				{
					//LOGD("mCallBackInfoArray[j].func:%x\n", (uint32_t)mCallBackInfoArray[j].func);
					//
					//LOGD("mRxBuffer\n");
					//for (int i = 0; i < mPackageSize; i++)
					//LOGD("%x", mRxBuffer[i]);
					//LOGD("\n\n");

					//防止在回调函数中进行长时间操作
					float time1 = (float)(clock()*1000 / CLOCKS_PER_SEC);
					mCallBackInfoArray[j].func(mInterfaceId, mCallBackInfoArray[j].param, mRxBuffer, mPackageSize);
					float time2 = (float)(clock() * 1000 / CLOCKS_PER_SEC);
					if (time2 - time1 > 10.0f)//如果时间大于10ms
					{
						LOG("cost too much time in callback function! using time = %.2f\n", time2 - time1);
					}
				}
			}	
		}
	}
	return 0;
}