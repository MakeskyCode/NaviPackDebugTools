#include "SerialPortInterface.h"

void SerialPortInterface::InitSerialPortParam()
{
	mComHandle = INVALID_HANDLE_VALUE;
#if NAVIPACK_WIN
	memset(&mOverlappedSend, 0, sizeof(OVERLAPPED));
	memset(&mOverlappedRecv, 0, sizeof(OVERLAPPED));
	mOverlappedSend.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	mOverlappedRecv.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
}



SerialPortInterface::SerialPortInterface(InterfaceID id, uint32_t maxTxBufferSize) :HardwareInterface(id, maxTxBufferSize)
{
	InitSerialPortParam();
}


SerialPortInterface::~SerialPortInterface()
{
#if NAVIPACK_WIN
	if (mOverlappedRecv.hEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(mOverlappedRecv.hEvent);
		mOverlappedRecv.hEvent = INVALID_HANDLE_VALUE;
	}
	if (mOverlappedSend.hEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(mOverlappedSend.hEvent);
		mOverlappedSend.hEvent = INVALID_HANDLE_VALUE;
	}
#endif
}

#ifdef NAVIPACK_WIN
int SerialPortInterface::Open(const char *file_name, uint32_t param1, uint32_t param2)
{
	uint32_t port_no = param1;
	uint32_t baud_rate = param2;

	strcpy_s(mFileName, file_name);

#ifdef UNICODE
	int needlen = MultiByteToWideChar(936, 0, file_name, -1, 0, 0);
	TCHAR *file_name_t = new TCHAR[needlen];
	if (MultiByteToWideChar(936, 0, file_name, -1, file_name_t, needlen) != needlen)
	{
		delete[]file_name_t;
		return -1;
	}	
#else
	const TCHAR *file_name_t = file_name;
#endif

	if (mIsOpened)
	{
		if ((port_no == mPortNo) && (baud_rate == mDCB.BaudRate))
			return 0;
		else
			Close();
	}

	mPortNo = port_no;
	mBaud = baud_rate;
	mComHandle = ::CreateFile(file_name_t,
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL,
		OPEN_EXISTING,
       // FILE_ATTRIBUTE_NORMAL,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

#ifdef UNICODE
	delete[] file_name_t;
#endif

	if (mComHandle == INVALID_HANDLE_VALUE)
	{
		CloseHandle(mComHandle);
		return -1;
	}

	PurgeComm(mComHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	if (SetupComm(mComHandle, 4096, 4096) == 0)
	{
		CloseHandle(mComHandle);
		return -1;
	}
	mDCB.DCBlength = sizeof(DCB);
	if (GetCommState(mComHandle, &mDCB) == 0)
	{
		CloseHandle(mComHandle);
		return -1;
	}
	else
	{
		mDCB.BaudRate = baud_rate;
		mDCB.ByteSize = 8;
		mDCB.Parity = NOPARITY;
		mDCB.StopBits = ONESTOPBIT;
		if (SetCommState(mComHandle, &mDCB) == 0)
		{
			CloseHandle(mComHandle);
			return -1;
		}
	}

	Sleep(60);

	COMMTIMEOUTS TimeOuts;

	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;

	TimeOuts.WriteTotalTimeoutMultiplier = 0;
	TimeOuts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(mComHandle, &TimeOuts);


	EscapeCommFunction(mComHandle, SETDTR);
	mIsOpened = 1;

	mTxThread.Start();
	mRxThread.Start();
//	mRxDealThread.Start();
	return 0;
}

int SerialPortInterface::Close()
{
	mRxThread.Exit();
	mTxThread.Exit();
//	mRxDealThread.Exit();

	if (mComHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(mComHandle);		
		mComHandle = INVALID_HANDLE_VALUE;
	}
	mIsOpened = FALSE;
	return TRUE;
}

int32_t SerialPortInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
    uint32_t readed = 0;
    uint32_t res;

    ResetEvent(mOverlappedRecv.hEvent);

    res = ReadFile(mComHandle, byBuf, dwLen, (LPDWORD)&readed, &mOverlappedRecv);
    if (res == FALSE)
    {
        mSysErrCode = GetLastError();
        if (mSysErrCode == ERROR_IO_PENDING)
        {
            WaitForSingleObject(mOverlappedRecv.hEvent, INFINITE);
            GetOverlappedResult(mComHandle, &mOverlappedRecv, (LPDWORD)&readed, FALSE);
        }
    }

    return readed;
}

int32_t SerialPortInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
    uint32_t writed = 0;
    ResetEvent(mOverlappedSend.hEvent);
    uint32_t res = WriteFile(mComHandle, byBuf, dwLen, (LPDWORD)&writed, &mOverlappedSend);
    if (!res)
    {
		//LOGE("~~~~~~~~~~~~~~~~~WriteFile error~~~~~~~~~~~~~~~~~~~");
        mSysErrCode = GetLastError();
        if (mSysErrCode == ERROR_IO_PENDING)
        {
            WaitForSingleObject(mOverlappedSend.hEvent, dwTimeOut);
            GetOverlappedResult(mComHandle, &mOverlappedSend, (LPDWORD)&writed, FALSE);
            return writed;
        }
        else
            return 0;
    }
    return writed;
}
/*
int32_t SerialPortInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
    uint32_t readed = 0;
    uint32_t res = ReadFile(mComHandle, byBuf, dwLen, (LPDWORD)&readed, NULL);
    return res ? readed : 0;
}

int32_t SerialPortInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{

    uint32_t writed = 0;
    uint32_t res = WriteFile(mComHandle, byBuf, dwLen, (LPDWORD)&writed, NULL);
    return res ? writed : 0;
}
*/
#else

int SerialPortInterface::Open(const char *file_name, uint32_t param1, uint32_t param2)
{
	uint32_t port_no = param1;
	uint32_t baud_rate = param2;

	strcpy(mFileName, file_name);

	if (mIsOpened)
	{
		if (baud_rate == mBaud)
			return 0;
		else
			Close();
	}

	mPortNo = port_no;
	mBaud = baud_rate;

	struct termios newtio, oldtio;

	mComHandle = open(mFileName, O_RDWR | O_NOCTTY | O_NDELAY);
	
	//LOGDD("handle:%d\n",mComHandle);
	LOG("Port: %s , mComHandle:%d!\n", file_name, mComHandle);

	if (mComHandle == -1)
		return -1;

	if (fcntl(mComHandle, F_SETFL, 0)<0)//设置阻塞
		return -2;

	//if (isatty(STDIN_FILENO) == 0)
	//	return -3;
	if (tcgetattr(mComHandle, &oldtio) != 0) {
		return -4;
	}

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;   //enable read and write
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;          //8 data bit
	newtio.c_cflag &= ~PARENB;      //disable parity
	newtio.c_cflag &= ~CSTOPB;      //1 stop bit

	cfsetispeed(&newtio, mBaud);
	cfsetospeed(&newtio, mBaud);

	newtio.c_cc[VTIME] =1;     //while reading data count less than VMIN, after 100ms can be read
	newtio.c_cc[VMIN] = 24;
	tcflush(mComHandle, TCIFLUSH);
	
	if ((tcsetattr(mComHandle, TCSANOW, &newtio)) != 0)
		return -5;

	// play with DTR
	int iFlags;
	// turn on DTR
	iFlags = TIOCM_DTR;
	ioctl(mComHandle, TIOCMBIS, &iFlags);

	LOG("open port_name:%s  success!\n",mFileName);
	mIsOpened = 1;

	mTxThread.Start();
	mRxThread.Start();
	return 0;
}

int SerialPortInterface::Close()
{
	void* status;

	mRxThread.Exit();
	mTxThread.Exit();
	//close serial communication
	if (mComHandle != 0)
	{
		close(mComHandle);
		mComHandle = 0;
	}

	mIsOpened = 0;

	return 0;
}


int32_t SerialPortInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	uint32_t readed = 0;
#ifdef NAVIPACK_WIN
	readed = read(mComHandle, byBuf, dwLen);
#else
	struct timeval tv = { 0, 10 * 1000 };
	fd_set readfd;
	int rc;
	FD_ZERO(&readfd);
	FD_SET(mComHandle, &readfd);
	rc = select(mComHandle + 1, &readfd, NULL, NULL, &tv);
	if (rc > 0)
	{
		readed = read(mComHandle, byBuf, dwLen);
	}
	else
	{
		Sleep(1);
	}

#endif

	//PrintBuf(byBuf,16,"ReadBufferFromInterface");
	return readed;
}

int32_t SerialPortInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	unsigned long writed=0;
	int needWrite;
	//if (mPortNo == 3)
	//	LOGD("%s SerialPortInterface --> WriteBufferToInterface %d", mFileName, dwLen);
	do {
		needWrite = dwLen - writed;
		if (needWrite > 8 * 1024)
		{
			needWrite = 8 * 1024;
		}
		int ans = write(mComHandle, byBuf + writed, needWrite);

		if (ans == -1)
		{
			//tx_bytes_ += nw;
			LOGD("write(com_handle_) wrong!\n");
			return writed;
		}
		writed += ans;
//		LOGD("write(com_handle_) %d!\n", ans);
	}while (writed < dwLen);
//	LOGD("SerialPortInterface --> WriteBufferToInterface %d", writed);
	return writed;
}

#endif

int SerialPortInterface::ReOpen()
{
	Close();
	return Open(mFileName, mPortNo, mBaud);
}