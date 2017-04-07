#include "TCPServerInterface.h"

uint32_t TCPServerInterface::mInitFlag = 0;


void TCPServerInterface::InitTCPServerParam()
{
#if NAVIPACK_WIN
	if (!mInitFlag)
	{
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 0);
		::WSAStartup(sockVersion, &wsaData);
	}
#endif
	mInitFlag++;

	mIsOpened = 0;
	mListeningFlag = 0;
	mListenPort = 0;
	mConnected = 0;

	mServerSocket = INVALID_SOCKET;
	mClientSocket = INVALID_SOCKET;
}



TCPServerInterface::TCPServerInterface(InterfaceID id, uint32_t maxTxBufferSize) : HardwareInterface(id,maxTxBufferSize)
{
	InitTCPServerParam();
}

TCPServerInterface::~TCPServerInterface()
{
	Close();

	mInitFlag--;

#if NAVIPACK_WIN
	if (!mInitFlag)
		::WSACleanup();
#endif
}

int TCPServerInterface::Open(const char * file_name, uint32_t param1, uint32_t param2)
{
	sockaddr_in serveraddr;
	mListenPort = param1;
	memset(mFileName,0,sizeof(mFileName));
	strcpy(mFileName, file_name);
	LOG("%p TCPServerInterface::Open %s %d\n",this, file_name, param1);
	if (mListeningFlag || mIsOpened)
		Close();

	//Create a Tcp Server
	mServerSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mServerSocket <= 0)
		return -1;

	int socketoptvalue = 1;
	setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&socketoptvalue, sizeof(int));
	socketoptvalue = 128 * 1024;
	setsockopt(mServerSocket, SOL_SOCKET, SO_RCVBUF, (const char*)(&socketoptvalue), sizeof(int));
	setsockopt(mServerSocket, SOL_SOCKET, SO_SNDBUF, (const char*)&socketoptvalue, sizeof(int));

#ifdef NAVIPACK_WIN
	int socket_timeout = 3;
#else
	struct timeval socket_timeout = {3,0 };//3s
#endif

	setsockopt(mServerSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&socket_timeout), sizeof(socket_timeout));
	setsockopt(mServerSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)(&socket_timeout), sizeof(socket_timeout));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(mListenPort);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(mServerSocket, (sockaddr *)&serveraddr, sizeof(serveraddr)) != 0)
		return -2;

	//start listen, maximum length of the queue of pending connections is 1  
	if (listen(mServerSocket, 10) != 0)
		return -3;

	//Create a Tcp listen thread to wait for connecting 
	char buf[256];
	memset(buf, 0, sizeof(buf));
	sprintf(buf,"%s_%d_listener",file_name,param1);
	mListenThread.Create(buf,ListenThreadLoop, this);
	mListeningFlag = 1;
	mListenThread.Start();

	mTxThread.Start();
	mRxThread.Start();
	return 0;
}

int TCPServerInterface::Close()
{
	/*
#if NAVIPACK_WIN
	if (mClientSocket != INVALID_SOCKET)
		closesocket(mClientSocket);

	if (mServerSocket != INVALID_SOCKET)
		closesocket(mServerSocket);
#else
	if (mClientSocket != INVALID_SOCKET)
		close(mClientSocket);

	if (mServerSocket != INVALID_SOCKET)
		close(mServerSocket);
#endif
	*/
	CLOSE(mClientSocket);
	CLOSE(mServerSocket);
	mServerSocket = INVALID_SOCKET;
	mClientSocket = INVALID_SOCKET;

	mIsOpened = 0;
	mListeningFlag = 0;

	mListenThread.Exit();
	return 0;
}

int TCPServerInterface::ReOpen()
{
	Close();
	if (mListenPort > 0)
		return Open(NULL, mListenPort, 0);
	return 0;
}

int32_t TCPServerInterface::ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{

	//if (mIsOpened)

	int32_t readed = 0;
	if (mClientSocket <= 0)
		return 0;
	//readed = recv(mClientSocket, (char *)byBuf, dwLen, 0);
	readed = BlockReceive(mClientSocket, (char *)byBuf, dwLen, 1);
	//LOG("TCPServerInterface::ReadBufferFromInterface %d", readed);
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
	//LOGD("TCPServerInterface::ReadBufferFromInterface %d", readed);
	return readed;

}

int32_t TCPServerInterface::WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut)
{
	if (mIsOpened)
	{
		int32_t writed = 0;
		if (mClientSocket == INVALID_SOCKET)
			return -1;
		writed = send(mClientSocket, (char *)byBuf, dwLen, 0);
		if (writed <= 0)
		{
#ifdef NAVIPACK_WIN
			LOGD("send return %d, mClientSocket=%d, client disconnect %d\n", writed, mClientSocket,GetLastError());
#endif
			mConnected = 0;
			CLOSE(mClientSocket);
			mClientSocket = INVALID_SOCKET;
			
		}
		return writed;
	}
	return 0;
}

int TCPServerInterface::ListenThreadLoop(void * param)
{
	SOCKET_LEN len = sizeof(sockaddr_in);

	TCPServerInterface *tcpsi = (TCPServerInterface *)param;

	int sock = 0;
	//if (tcpsi->mConnected == 0)
	//{
		sock = accept(tcpsi->mServerSocket, (sockaddr *)&tcpsi->mClientAddr, &len);
//	    LOGD("old --> %d  new --> %d\n", tcpsi->mClientSocket,sock);
		if (sock <= 0)
		{
//			LOGD("error accpet sock=%d\n", sock);
			Sleep(100);
			return 0;//这里返回-1 会导致线程退出
		}
		//close logic
		if (tcpsi->mClientSocket > 0 && tcpsi->mClientSocket != sock)
		{
			LOGD("CLOSE tcpsi->mClientSocket %d\n", tcpsi->mClientSocket);
			CLOSE(tcpsi->mClientSocket);
			tcpsi->mClientSocket = 0;
		}

		//accept new conn
		tcpsi->mClientSocket = sock;

		int remotePartPort = ntohs(tcpsi->mClientAddr.sin_port);
		LOGD("\n\n %s [CTCPServer::accept] ---->  Conection accepted from %s:%u, client sock=%d \n\n", tcpsi->mFileName,
			inet_ntoa(tcpsi->mClientAddr.sin_addr), remotePartPort, tcpsi->mClientSocket);
		tcpsi->mIsOpened = 1;
		
		tcpsi->mListeningFlag = 0;
		tcpsi->mConnected = 1;
	//}

	Sleep(100);
	return 1;
}
