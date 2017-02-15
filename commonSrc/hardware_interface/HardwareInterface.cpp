#include "HardwareInterface.h"
#include "tools/LogFile.h"

int32_t HardwareInterface::mIdCounter = MAX_INTERFACE_ID;

void HardwareInterface::initInterfaceParam(uint32_t MaxTxBufferSize)
{
	ResetCounters();

	for (int i = 0; i < MAX_PROTOCOL_NUM; i++)
		mProtocolArray[i] = NULL;

	//100K的发送缓冲区
	mTxBufferFifo = new FifoBuffer(MaxTxBufferSize);

	//mTxBufferFifo = new CFrameList();
	//mTxBufferFifo->Init(1, MAX_FRAME_SIZE, "mTxBufferFifo");

	char thread_name[128];
	sprintf(thread_name, "Hardware Interface:%d RxThread", mId);
	mRxThread.Create(thread_name, RxThreadLoop, this);
	sprintf(thread_name, "Hardware Interface:%d TxThread", mId);
	mTxThread.Create(thread_name, TxThreadLoop, this);

	mIsOpened = mSysErrCode = 0;
	mProtocolArraySize = 0;
	memset(mFileName, 0, sizeof(mFileName));

}

HardwareInterface::HardwareInterface()
{
	initInterfaceParam(128*1024);
	mId = mIdCounter++;
}

HardwareInterface::HardwareInterface(InterfaceID id,uint32_t maxBufferSize)
{
	initInterfaceParam(maxBufferSize);
	mId = id;
}


HardwareInterface::~HardwareInterface()
{
	mTxThread.Exit();
	mRxThread.Exit();
	delete mTxBufferFifo;
	for (int i = 0; i < MAX_PROTOCOL_NUM; i++)
		delete mProtocolArray[i];
}

void HardwareInterface::ResetCounters()
{
	mTotalRxBytes = mTotalTxBytes = 0;
}

int HardwareInterface::RxFun()
{
	uint8_t buf[2048];
	int32_t readed;
	if (GetInterfaceType() == SERIAL_PORT)
	{
		if (!mIsOpened) return 0;
	}

	readed = ReadBufferFromInterface(buf, 2048);
	if (readed > 0)
	{
		//if (mId == LIDAR_INTERFACE_ID)
		//{
		//	PrintBuf(buf, readed, "Receive Data:");
		//}
		/*else if (mId == MCU_INTERFACE_ID)
		{
			static int startTime1 = time(NULL);
			static int endTime1 = time(NULL);
			static int recvLen1 = 0;
			recvLen1 += readed;
			endTime1 = time(NULL);
			if (endTime1 - startTime1 > 0)
			{
				printf("MCU_INTERFACE_ID %d s , recv data %d \n", endTime1 - startTime1, recvLen1);
				recvLen1 = 0;
				startTime1 = endTime1;
			}
		}*/
		mTotalRxBytes += readed;

		mCS.Enter();


		for (int32_t i = 0; i < mProtocolArraySize; i++)
		{
			Protocol *ptl = mProtocolArray[i];
			ptl->PushBuffer(buf, readed);
		}

		mCS.Leave();

		return readed;
	}
	else
	{
		return 0;
	}
}

int HardwareInterface::RxThreadLoop(void * param)
{
	HardwareInterface *hif = (HardwareInterface *)param;
	int readed =  hif->RxFun();
	return readed;
	
}

int HardwareInterface::TxThreadLoop(void * param)
{
	HardwareInterface *hif = (HardwareInterface *)param;
	if (0 == hif->mTxEvent.Wait(100))
	{
		int32_t len;
		uint8_t txBuf[1024];
		do{

			len = hif->mTxBufferFifo->pop(txBuf, sizeof(txBuf));
			//len = hif->mTxBufferFifo->ReadFrame((char*)(hif->mTxBuf));
			//LOGD("--------->>>>>>>>>>>> %s pop data %d", hif->GetInterfaceTypeName(hif->GetInterfaceType()), len);
			if (len > 0)
			{
				/*if (hif->GetID() == NETWORK_INTERFACE_ID)
				{
					PrintBuf(txBuf,len,"ReadFrame:");
				}*/
				//if (hif->GetID() == MCU_INTERFACE_ID)
				{
					//PrintBuf(txBuf, len, "Send 2 Robot: ");
				}
				hif->WriteBufferToInterface(txBuf, len, 1000);
			}
		} while (len > 0);
	}
	return 0;
}



const char * HardwareInterface::GetInterfaceTypeName(InterfaceType it)
{
	switch (it)
	{
	case SERIAL_PORT: return "Serial Port Interface";
	case TCP_SERVER: return "TCP Server Interface";
	case TCP_CLIENT: return "TCP Client Interface";
	case UDP: return "UDP Interface";
	default: return "UNKOWN Interface";
	}
}

int HardwareInterface::AddRxCallBack(ProtocolType type, HardwareInterfaceCallBack func, void *param,uint32_t maxRxBufferSize)
{
	LOGD("%s AddRxCallBack", mFileName);
	int i;
	mCS.Enter();
	for(i = 0 ; i < mProtocolArraySize ; i ++)
	{
		if (mProtocolArray[i]->GetProtocolType() == type)
		{
			mCS.Leave();
			return mProtocolArray[i]->AddCallBack(func, param);
		}
	}

	if ((i == mProtocolArraySize) && (i < MAX_PROTOCOL_NUM))
	{
		mProtocolArray[i] = Protocol::CreateProtocol(mId, type, maxRxBufferSize);	//8M
		mProtocolArraySize++;
		mCS.Leave();
		return mProtocolArray[i]->AddCallBack(func, param);
	}

	mCS.Leave();
	return -1;
}

int HardwareInterface::RemoveRxCallBack(ProtocolType type,  int id)
{
	mCS.Enter();
	for (int i = 0; i < mProtocolArraySize; i++)
	{
		if (mProtocolArray[i]->GetProtocolType() == type)
		{
			mCS.Leave();
			return mProtocolArray[i]->RemoveCallBack(id);
		}
	}
	mCS.Leave();
	return -1;
}

int HardwareInterface::WriteBuf(uint8_t * points, int32_t len)
{
	mTotalTxBytes += len;
	int ret = mTxBufferFifo->push(points, len, 0);
	mTxEvent.Set();
	return ret;
}


int HardwareInterface::WriteData(ProtocolType type, uint8_t * buf, int32_t len)
{
	mTotalTxBytes += len;
	mCS.Enter();
	for(int i = 0 ; i < mProtocolArraySize ; i ++)
	{
		if (mProtocolArray[i]->GetProtocolType() == type)
		{
			int32_t pack_len = 0;
			const uint8_t * pack_buf = mProtocolArray[i]->PackageBuffer(buf, len, &pack_len);
			if (pack_buf)
			{
				mCS.Leave();
				int ret = 0;
				//if (PT_SERAL_PACKAGE_AT_ONCE == type)
				//{
				//	LOG("PT_SERAL_PACKAGE_AT_ONCE");
				//	ret = mTxBufferFifo->push(pack_buf, pack_len, 0);
				//	//WriteBufferToInterface(pack_buf, pack_len, 100);
				//}
				//else
				//{
					ret = mTxBufferFifo->push(pack_buf, pack_len, 0);
//				}
				 
				mTxEvent.Set();
				return ret;
			}
		}
	}

	mCS.Leave();

	return -1;
}

