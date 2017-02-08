
#ifndef __SERIAL_PACKAGE_PROTOCOL__
#define __SERIAL_PACKAGE_PROTOCOL__

#include "Protocol.h"

class SerialPackageProtocol : public Protocol
{
public:
	SerialPackageProtocol(int32_t interface_id, uint32_t maxPackageSize);
	~SerialPackageProtocol();

	virtual ProtocolType GetProtocolType() { return PT_SERAL_PACKAGE; }

	// Inherited via Protocol
	virtual int PushBuffer(const uint8_t * points, int32_t len) ;
	virtual const uint8_t * PackageBuffer(const uint8_t * points, int32_t in_len, int32_t * out_len) ;

private:
	uint8_t mLastByte;
	uint32_t mBeginFlag;
	uint32_t mCtrlFlag;	
	int32_t mRevOffset;
	uint8_t mCheckSum;

	uint8_t mFrameHead, mFrameTail, mFrameCtrl;

	virtual int PushByte(uint8_t points);
};


#endif
