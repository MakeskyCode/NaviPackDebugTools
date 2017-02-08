
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "inttypes.h"

#include <stdio.h>
#include <time.h>

typedef enum InterfaceID
{
	LIDAR_INTERFACE_ID,
	MCU_INTERFACE_ID,
	USB_HOST_INTERFACE_ID,
	NETWORK_INTERFACE_ID,
	CAMERA_INTERFACE_ID,
	MPU6500_NETWORK_ID,
	UNIFIED_SENDOR_INTERFACE_ID,
	MAX_INTERFACE_ID,
}InterfaceID;


enum ProtocolType
{
	PT_BUFFER,
	PT_SERAL_PACKAGE,
//	PT_SERAL_PACKAGE_AT_ONCE,
};

typedef void(*HardwareInterfaceCallBack)(int32_t id, void *param, const uint8_t *buffer, int32_t len);

typedef struct
{
	int id;
	HardwareInterfaceCallBack func;
	void *param;
}CallBackInfo;

#define MAX_CALL_BACK_NUM 16

class Protocol
{
public:
	Protocol(int32_t interface_id, uint32_t max_package_size);
	virtual ~Protocol();

	static Protocol * CreateProtocol(int32_t interface_id, ProtocolType type, uint32_t max_pakage_size = 4096);
	
	virtual int PushBuffer(const uint8_t * points, int32_t len) = 0;
	virtual ProtocolType GetProtocolType() = 0;
	virtual const uint8_t * PackageBuffer(const uint8_t *points, int32_t in_len, int32_t *out_len) = 0;

	int AddCallBack(HardwareInterfaceCallBack func, void * param);
	int RemoveCallBack(int id);

	public:

	int32_t mMaxPackageSize;
	int32_t mPackageSize;
	uint8_t *mRxBuffer;
	uint8_t *mTxBuffer;
	uint8_t *mRxZipBuffer;
	uint8_t *mTxZipBuffer;
	int32_t mInterfaceId;

	int32_t mCallBackInfoArraySize;

	CallBackInfo mCallBackInfoArray[MAX_CALL_BACK_NUM];	
};


#endif
