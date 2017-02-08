#include "TCPClientInterface.h"
#ifdef NAVIPACK_WIN
#include <winsock2.h>
#pragma comment (lib,"Ws2_32.lib")
#else
#include "tools/windowsdef.h"
#endif
#include <tools/CommonFunctions.h>

uint32_t TCPClientInterface::mInitFlag = 0;

void TCPClientInterface::InitTCPClientParam()
{
	mSocket = INVALID_SOCKET;

#if NAVIPACK_WIN
	if (!mInitFlag)
	{
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 0);
		::WSAStartup(sockVersion, &wsaData);
	}
#endif

	mInitFlag++;
}



TCPClientInterface::TCPClientInterface(InterfaceID id, uint32_t maxTxBufferSize) : HardwareInterface(id,maxTxBufferSize)
{
	InitTCPClientParam();
}

TCPClientInterface::~TCPClientInterface()
{
	Close();

	mInitFlag--;
#if NAVIPACK_WIN
	if(!mInitFlag)
		::WSACleanup();
#endif
}

int TCPClientInterface::Open(const char * filename, uint32_t param1, uint32_t param2)
{
	mIP = param1;
	mPort = param1;

	strcpy(mFileName, filename);

	mSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
		return -1;

	int socketoptvalue = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&socketoptvalue, sizeof(int));
	socketoptvalue = 128 * 1024;
	setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)(&socketoptvalue), sizeof(int));
	setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&socketoptvalue, sizeof(int));

	socketoptvalue = 3;
	setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&socketoptvalue), sizeof(int));
	setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)(&socketoptvalue), sizeof(int));


	sockaddr_in remote;
	remote.sin_addr.s_addr = inet_addr(mFileName);
	remote.sin_family = AF_INET;
	remote.sin_port = htons((u_short)mPort);
	
	if (-1 == ::connect(mSocket, (sockaddr *)&remote, sizeof(sockaddr)))
	{
#ifdef NAVIPACK_WIN
		LOGD("connect error %d \n", GetLastError());
#endif // NAVIPACK_WIN
		return -1;
	}

	LOGD("ok\n");

	mTxThread.Start();
	mRxThread.Start();
	mIsOpened = TRUE;
	return 0;
}

int TCPClientInterface::Close()
{
	mRxThread.Exit();

	if (mSocket != INVALID_SOCKET)
	{
#if NAVIPACK_WIN
		::closesocket(mSocket);
#else
		close(mSocket);
#endif
	}
	mSocket = INVALID_SOCKET;	
	
	mIsOpened = FALSE;
	return TRUE;
}


int32_t TCPClientInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	int32_t readed = 0;
	if (mSocket <= 0)
		return 0;
	//readed = recv(mClientSocket, (char *)byBuf, dwLen, 0);
	readed = BlockReceive(mSocket, (char *)byBuf, dwLen, 1);
	//LOG("TCPClientInterface::ReadBufferFromInterface %d",readed);
	if (readed <= 0)
	{
		/*
		mConnected = 0;
		CLOSE(mClientSocket);
		mClientSocket = INVALID_SOCKET;
		LOGD("recv return %d, mClientSocket=%d, client disconnect\n", readed, mClientSocket);
		*/
		return 0;
	}
	
	return readed;
}

int32_t TCPClientInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	int32_t writed = 0;
	writed = send(mSocket, (char *)byBuf, dwLen, 0);
	if (writed <= 0)
	{
		CLOSE(mSocket);
		mSocket = INVALID_SOCKET;
	}

	return writed;
}

int TCPClientInterface::ReOpen()
{
	Close();
	return Open(mFileName, mIP, mPort);
}