
#ifndef __TCP_SERVER_INTERFACE_H__
#define __TCP_SERVER_INTERFACE_H__

#include <stdio.h>  
#include "HardwareInterface.h"
#include "tools/CommonFunctions.h"

#ifdef NAVIPACK_WIN
#include <winsock2.h>
#include "windows.h"
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif


//只支持1个客户端连接
class TCPServerInterface : public HardwareInterface
{
public:
	TCPServerInterface(InterfaceID id, uint32_t maxTxBufferSize);
	virtual ~TCPServerInterface();

	//filename is not used, params1 is port, param2 is not used
	virtual int Open(const char * file_name, uint32_t param1, uint32_t param2) ;
	virtual int Close() ;
	virtual int ReOpen() ;
	InterfaceType GetInterfaceType() { return TCP_SERVER; }

	
private:
	static uint32_t mInitFlag;

	void InitTCPServerParam();

	virtual int32_t ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;
	virtual int32_t WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;

	Thread mListenThread;
	static int ListenThreadLoop(void * param);

	sockaddr_in mClientAddr;
	int mServerSocket, mClientSocket;
	uint32_t mListeningFlag, mConnected;
	uint16_t mListenPort;
};

#endif