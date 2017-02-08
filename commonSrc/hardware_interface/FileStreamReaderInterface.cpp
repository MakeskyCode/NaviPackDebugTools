#include "FileStreamReaderInterface.h"
#include "DataReviewInterface.h"
#include "tools/LogFile.h"

FileStreamReaderInterface::FileStreamReaderInterface()
{
	mFile = NULL;
	mBuffer = new uint8_t[1024 * 100];
	mFifoBuffer = new FifoBuffer(1204 * 128);
	mLastFileTimeStemp = 0;
	//mRxThread.Start();
	memset(mFileName,0,sizeof(mFileName));
}

FileStreamReaderInterface::~FileStreamReaderInterface()
{
	delete[] mBuffer;
	delete mFifoBuffer;
}

int FileStreamReaderInterface::Open(const char * file_name, uint32_t param1, uint32_t param2)
{
	Close();


	mFile = fopen(file_name, "rb");
	if (mFile == NULL)
		return -1;

	const char *head = "InterfaceStream";
	char* buf = new char[strlen(head)];
	fread(buf, strlen(head), 1, mFile);

	if (memcmp(buf, head, strlen(head)) != 0)
	{
		fclose(mFile);
		mFile = NULL;
		return -2;
	}
	else 
	{
		//��ȡ��һ�ε�ʱ���
		fread(&mFirstFileTimeStemp, sizeof(mFirstFileTimeStemp), 1, mFile);
		LOGD("%s mFirstFileTimeStemp = %lld",mFileName, mFirstFileTimeStemp);

		mReadThread.Create("File Read Thread", ReadThreadLoop, this);
		strcpy(mFileName, file_name);

		mReadThread.Start();
	}

	return 0;
}

int FileStreamReaderInterface::Close()
{
	if (mFile)
	{
		mReadThread.Exit();
		fclose(mFile);
		mFile = NULL;
	}
	return 0;
}

int FileStreamReaderInterface::ReOpen()
{
	Close();
	return Open(mFileName, 0, 0);
}

int FileStreamReaderInterface::AddReviewCallback(int id, void *param)
{
	int canUseIndex = -1;
	for (int i = 0; i < MAX_DATA_REVIEW_NUM; i++)
	{
		if ((mDataReview[i]) == NULL)
		{
			if (canUseIndex < 0) 
			{
				canUseIndex = i;
			}
		}
		else
		{
			if (mDataReview[i]->mId == id)
			{
				delete mDataReview[i];
				mDataReview[i] = new DataReview(id, param);
				return 0;
			}
		}
	}

	if (canUseIndex >= 0)
	{
		mDataReview[canUseIndex] = new DataReview(id, param);
		return 0;
	}

	return -1;


}

void FileStreamReaderInterface::RemoveReviewCallback(int id)
{
	for (int i = 0; i < MAX_DATA_REVIEW_NUM; i++)
	{
		if ((mDataReview[i]) && mDataReview[i]->mId == id)
		{
				delete mDataReview[i];
				mDataReview[i] = NULL;
		}
	}
}

int32_t FileStreamReaderInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	if (WAIT_OBJECT_0 == mRxEvent.Wait(dwTimeOut))
	{
		int len = mFifoBuffer->pop(byBuf, dwLen);
//		LOGD("FileStreamReaderInterface::ReadBufferFromInterface %d",len);
		return len;
	}
	return 0;
}

int32_t FileStreamReaderInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	return 0;
}

int FileStreamReaderInterface::ReadThreadLoop(void * param)
{

	FileStreamReaderInterface *fsri = (FileStreamReaderInterface *)param;
	static uint64_t readCount = 0;
	if (fsri->mLastFileTimeStemp == 0) //��һ��
	{
		int id,len;
		int n = fsri->ReadSection(&fsri->mNextFileTimeStemp,&id, &len, fsri->mBuffer);
		if (n > 0) {

			Sleep(2000);
			LOGD("-->>>>>>>>>>>>>start review data !!<<<<<<<<------------------");

			//��ȡ֮��Ӧ���ַ������Ե��߳�
			for (int i = 0; i < MAX_DATA_REVIEW_NUM; i++)
			{
				if (fsri->mDataReview[i] && fsri->mDataReview[i]->mId == id)
				{
					((DataReviewInterface*)(fsri->mDataReview[i]->mDataCallback))->DataReviewCallback(fsri->mBuffer, len);
					break;
				}
			}

			readCount += len;
			fsri->mLastFileTimeStemp = fsri->mFirstFileTimeStemp;
			fsri->mLastRealTimeStemp = now();
			
		}
	}
	else
	{
		TimeStamp td1 = fsri->mNextFileTimeStemp - fsri->mLastFileTimeStemp;//¼�����ݵ�ʱ���
		TimeStamp t = now();
		TimeStamp td2 = t - fsri->mLastRealTimeStemp;						//��ǰϵͳ��ʱ���


		if (td2 >= td1)
		{
			int id, len;
			fsri->mLastFileTimeStemp = fsri->mNextFileTimeStemp;
			int n = fsri->ReadSection(&fsri->mNextFileTimeStemp,&id, &len, fsri->mBuffer);
			if (n > 0)
			{

				readCount += len;
//				LOGD("%lld %lldread section %d %d %lld",td2,td1,id,len, readCount);
				//��ȡ֮��Ӧ���ַ������Ե��߳�
				for (int i = 0; i < MAX_DATA_REVIEW_NUM; i++)
				{
					if (fsri->mDataReview[i] && fsri->mDataReview[i]->mId == id)
					{
					
						((DataReviewInterface*)(fsri->mDataReview[i]->mDataCallback))->DataReviewCallback(fsri->mBuffer, len);
						break;
					}
				}
				fsri->mLastRealTimeStemp = t;
			}
		}
	}

	//2ms��ʱ
	Sleep(2);
	return 0;
}

int FileStreamReaderInterface::ReadSection(int64_t *next_time_stemp, int *id,int *len, uint8_t *data)
{
	int n;

	//��ȡID
	int secId;
	n = fread(&secId,1,sizeof(secId), mFile);
	if (n != sizeof(secId))
	{
		return -1;
	}
	
	if (id)
	{
		*id = secId;
	}

	int64_t t;
	//��ȡ���ݳ���
	int32_t secLen;
	n = fread(&secLen, 1,sizeof(secLen),mFile);
	if (n != sizeof(secLen))
		return -1;
	//��ȡ����
	n = fread(data, 1, secLen, mFile);
	if (n != secLen)
		return -1;

	//��ȡʱ���
	n = fread(&t, 1, 8, mFile);
	if (n != 8)
	{
		if (next_time_stemp)
			*next_time_stemp = 0;
		return secLen;
	}

	if (next_time_stemp)
		*next_time_stemp = t;
	if (len)
		*len = secLen;
	return secLen;
}
