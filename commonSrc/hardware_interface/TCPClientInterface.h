
#ifndef __TCP_CLIENT_INTERFACE_H__
#define __TCP_CLIENT_INTERFACE_H__

#include "HardwareInterface.h"

#ifdef NAVIPACK_WIN
#include <winsock2.h>
#include "windows.h"
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

class TCPClientInterface : public HardwareInterface
{
public:
	TCPClientInterface(InterfaceID id, uint32_t maxTxBufferSize);
	virtual ~TCPClientInterface();

	//filename is not used, params1 is ip, params2 is port
	virtual int Open(const char * filename, uint32_t param1, uint32_t param2) ;
	virtual int Close() ;
	virtual int ReOpen() ;
	InterfaceType GetInterfaceType() { return TCP_CLIENT; }

private:
	static uint32_t mInitFlag;

	void InitTCPClientParam();
	int mSocket;

	int32_t mIP;
	int32_t mPort;

	virtual int32_t ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;
	virtual int32_t WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;

};

#endif