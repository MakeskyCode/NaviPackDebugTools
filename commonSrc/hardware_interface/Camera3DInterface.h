
#ifndef __CAMERA_3D_INTERFACE_H__
#define __CAMERA_3D_INTERFACE_H__

#include <stdio.h>  
#include "HardwareInterface.h"
#include "depthCamera/Camera.h"

#ifdef NAVIPACK_WIN
#include "windows.h"
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif


//只支持1个客户端连接  本接口只支持读取乐行的深度相机
class Camera3DInterface : public HardwareInterface
{
public:

	Camera3DInterface(InterfaceID id, uint32_t maxTxBufferSize);
	virtual ~Camera3DInterface();

	//filename is not used, params1 is port, param2 is not used
	virtual int Open(const char * file_name, uint32_t param1, uint32_t param2) ;
	virtual int Close() ;
	virtual int ReOpen() ;
	InterfaceType GetInterfaceType() { return TCP_SERVER; }

	
private:
	void InitCamera3DParam();
	Camera mCamera;
	uint8_t* mFrameBuf;
	int32_t mFrameWidth;
	int32_t mFrameHeight;
	int32_t mFrameLen;
	virtual int32_t ReadBufferFromInterface(uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;
	virtual int32_t WriteBufferToInterface(const uint8_t * byBuf, int32_t dwLen, int32_t dwTimeOut = -1) ;

	virtual int RxFun();


};

#endif