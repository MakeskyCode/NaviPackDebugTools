#ifndef __BUFFER_PROTOCOL_H__
#define __BUFFER_PROTOCOL_H__

#include "Protocol.h"

class BufferProtocol : public Protocol
{
public:
	BufferProtocol(int32_t interface_id, uint32_t max_package_size);
	~BufferProtocol();

	// Inherited via Protocol
	virtual int PushBuffer(const uint8_t * points, int32_t len) ;
	virtual ProtocolType GetProtocolType() { return PT_BUFFER; };
	virtual const uint8_t * PackageBuffer(const uint8_t * points, int32_t in_len, int32_t * out_len) ;

};

#endif // !__BUFFER_PROTOCOL_H__