#ifndef _NAVI_PACK_PROTOCOL_PRIVATE_H_
#define _NAVI_PACK_PROTOCOL_PRIVATE_H_
#include "SDKProtocol.h"

#pragma pack(push, 1) 

typedef struct UnifiedSensor_S
{
	SdkProtocolHeader header;
	UnifiedSensorInfo sensorInfo;
}UnifiedSensor;

#pragma pack(pop)

#endif