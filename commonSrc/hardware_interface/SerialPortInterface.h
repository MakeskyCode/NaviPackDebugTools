
#ifndef __SERIAL_PORT_INTERFACE_H__
#define __SERIAL_PORT_INTERFACE_H__

#include "HardwareInterface.h"

#include "tools/windowsdef.h"

#ifdef NAVIPACK_WIN
#include "windows.h"
#include "tchar.h"
#else

#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

class SerialPortInterface : public HardwareInterface
{
public:
	SerialPortInterface(InterfaceID id,uint32_t maxTxBufferSize);
	virtual ~SerialPortInterface();

	// Inherited via HardwareInterface
	// file_name is the serial port path(\\\\.\\COM? or /dev/tty? or /dev/acm? or /dev/usbtty?),
	// param1 is port num, witch is not used.
	// param2 is baud rate
	virtual int Open(const char *file_name, uint32_t param1, uint32_t param2) ;
	virtual int Close() ;
	virtual int ReOpen() ;
	InterfaceType GetInterfaceType() { return SERIAL_PORT; }

private:
	void InitSerialPortParam();

	virtual int32_t ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;
	virtual int32_t WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;

	uint32_t mPortNo;
	uint32_t mBaud;

	HANDLE mComHandle;
	DCB mDCB;

#ifdef NAVIPACK_WIN
	OVERLAPPED mOverlappedSend;
	OVERLAPPED mOverlappedRecv;
	COMMTIMEOUTS m_co;
#else

#endif

};


#endif // !__SERIAL_PORT_INTERFACE_H__