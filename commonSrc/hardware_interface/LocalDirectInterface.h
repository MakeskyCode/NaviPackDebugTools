
#ifndef __LOCAL_DIRECT_INTERFACE_H__
#define __LOCAL_DIRECT_INTERFACE_H__

#include <stdio.h>
#include "HardwareInterface.h"
#include "tools/windowsdef.h"

class LocalDirectInterface : public HardwareInterface
{
public:
	LocalDirectInterface(InterfaceID id,uint32_t maxBufferSize, uint32_t maxLocalBuffersize=32*1024);
	virtual ~LocalDirectInterface();

	virtual int Open(const char *file_name, uint32_t param1, uint32_t param2);
	virtual int Close();
	virtual int ReOpen();

	InterfaceType GetInterfaceType() { return LOCOL_DIRECT; }
private:
	virtual int32_t ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1);
	virtual int32_t WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1);
	int mParam1;
	int mParam2;
	FifoBuffer *mLocolBuf;
};


#endif