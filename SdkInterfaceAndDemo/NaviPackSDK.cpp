#include "NaviPackSDK.h"
#include "NavipackInterface.h"


#define SDK_WISION	(MAIN_VER<<24 | CHILD_VER << 16 | COMPILE_VER)
NAVIPACK_API int DLLCALL GetSDKVersion()
{
	return SDK_WISION;
}

NAVIPACK_API int DLLCALL SetGetNaviPackVersion(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SetGetNaviPackVersion();
}

//返回类的句柄
NAVIPACK_API int DLLCALL Create(ConnectType conType)
{
	printf("Create conType = %d\n", conType);
	int pID = -1;
	int ret = CNavipackInterface::CreateInstance(pID, conType);
	return pID;
}

//链接设备
NAVIPACK_API int DLLCALL Open(int id, const char* name, int param) //ip地址，端口号
{
	printf("Open %d %s %d\n", id, name, param);
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	int ret = CNavipackInterface::navi_interface[id]->Connect(name, param);
	return ret;
}

NAVIPACK_API int DLLCALL Reopen(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	int ret = CNavipackInterface::navi_interface[id]->ReConnect();
	return ret;

}

NAVIPACK_API int DLLCALL Close(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	int ret = CNavipackInterface::navi_interface[id]->DisConnect();
	return ret;
}

/// 设置从服务端传来消息数据的回调
/// @param[in] id NaviPack对象ID
/// @param[in] deviceMsgCb 设备消息回调
/// @return  返回值小于0，表示失败，等于0 表示成功
NAVIPACK_API int DLLCALL SetCallback(int id, DeviceMsgCallBack deviceMsgCb, ErrorMsgCallBack errMsgCb, MapPackageCallBack mapPackCb, LidarPackageCallBack lidarPackCb)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	CNavipackInterface::navi_interface[id]->SetCallbacks(deviceMsgCb, errMsgCb, mapPackCb, lidarPackCb);
	return 0;
}

NAVIPACK_API int DLLCALL GetCarrierParam(int id, CarrierParam *param)
{
	return 0;
}

NAVIPACK_API int DLLCALL SetCarrierParam(int id, CarrierParam *param)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SetCarrierParam(param);

}

NAVIPACK_API int DLLCALL GetNaviPackParam(int id, NaviPackParam *param)
{
	return 0;

}

NAVIPACK_API int DLLCALL SetNaviPackParam(int id, NaviPackParam *param)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SetNaviPackParam(param);

}

//设置目标点
NAVIPACK_API int DLLCALL SetTargets(int id, int position_x[], int position_y[], int num, int phi)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	if (num >= MAX_TARGET_LIST_SIZE)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SetTarget(position_x, position_y, num, phi);
}

//获取当前路径
NAVIPACK_API int DLLCALL GetCurrentPath(int id, int position_x[], int position_y[], int* num)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->GetCurrentPath(position_x, position_y, num);
}

NAVIPACK_API int DLLCALL PixelToPosition(int id, int pixel_x, int pixel_y, int *position_x, int *position_y)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->PixelToPosition(pixel_x, pixel_y, position_x, position_y);

}

NAVIPACK_API int DLLCALL PositionToPixel(int id, int position_x, int position_y, int *pixel_x, int *pixel_y)
{
	return 0;
}

//设置速度
NAVIPACK_API int DLLCALL SetSpeed(int id, int v, int w)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetSpeed(v, w);
}

//回充
NAVIPACK_API int DLLCALL AutoCharge(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->AutoCharge();
}

//设置回充坐标
NAVIPACK_API int DLLCALL SetChargerPosition(int id, int position_x, int position_y)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetChargerPosition(position_x, position_y);
}

// 设置回充门限
// NAVIPACK_API int DLLCALL SetChargeThreshold(int id, u8 threshold)
// {
// 	return 0;
// }

//建图命令
//mappingMode自动或手动
NAVIPACK_API int DLLCALL StartMapping(int id, int mappingMode)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->StartMapping(mappingMode);
}

NAVIPACK_API int DLLCALL StopMapping(int id, int save_flag)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->StopMapping(save_flag);
}

NAVIPACK_API int DLLCALL EnableMapUpdate(int id, bool bUpdate_flag)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->EnableMapUpdate(bUpdate_flag);
}
NAVIPACK_API int DLLCALL GetMapList(int id, int *id_buffer)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->GetMapList(id_buffer);
}

NAVIPACK_API int DLLCALL SaveCurrentMapToList(int id, int mapId)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SaveCurrentMapToList(mapId);

}

NAVIPACK_API int DLLCALL LoadMap(int id, int map_id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->LoadMap(map_id);
}

/// 设置获取当前使用的地图
/// @param[in] id NaviPack对象ID
/// @note 当连接成功或者需要更新本机地图时，可以调用该函数来通知NaviPack来上传地图
/// @return 返回值小于0，表示失败，等于0 表示成功
NAVIPACK_API int DLLCALL SetGetCurrentMap(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetGetCurrentMap();
}

NAVIPACK_API int DLLCALL LoadLocalMap(int id, const char* local_map_path, int map_id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->LoadLocalMap(local_map_path, map_id);
}

NAVIPACK_API int DLLCALL SaveMapToLocal(int id, const char * local_map_path, int picture_flag)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SaveMapToLocal(local_map_path, picture_flag);
}

NAVIPACK_API int DLLCALL GetMapLayer(int id, AlgMapData *map_data, int map_type)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->GetMapLayer(*map_data, map_type);
}

NAVIPACK_API int DLLCALL SetMapLayer(int id, AlgMapData *map_data, int map_type)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetMapLayer(map_data, map_type);
}

NAVIPACK_API int DLLCALL BitmapToMapLayer(unsigned char *bitmap, AlgMapData **map)
{
	return -1;
}

NAVIPACK_API int DLLCALL BmpFileToMapLayer(const char * file_path, AlgMapData **map)
{
	return -1;
}

NAVIPACK_API int DLLCALL MapLayerToBmpFile(AlgMapData *map, char * file_path)
{
	return -1;
}

NAVIPACK_API int DLLCALL ReadLocalMaplayer(const char* fileName, AlgMapData **map)
{
	return -1;
}

NAVIPACK_API int DLLCALL GetSensorData(int id, AlgSensorData *sensor_data, int sensorType)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->GetSensorData(*sensor_data, sensorType);
}

//读取状态寄存器数据
NAVIPACK_API int DLLCALL GetStatus(int id, AlgStatusRegister* status)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->ReadStatusRegister(*status);
}

//初始定位
NAVIPACK_API int DLLCALL InitLocation(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->InitLocation();
}

//释放资源，销毁句柄
NAVIPACK_API void DLLCALL Destroy(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return;
	}
	CNavipackInterface::deleteInstance(id);

}


/// 设置WIFI参数
/// @param[in] id NaviPack对象ID
/// @param[in] ssid WIFI SSID
/// @param[in] password WIFI 密码
/// @note 设置了SSID和WIFI之后，系统将主动建立TCP/UDP服务器（现有的硬件需要插入USB WIFI 网卡才可以使用WIFI连接）。
/// @return 返回值小于0，表示失败，等于0 表示成功
NAVIPACK_API int DLLCALL SetWiFiParam(int id, const  char * ssid, const  char * password)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SetWiFiParam(ssid, password);
}

/// 搜索网络中的设备
/// @param[in] id NaviPack对象ID
/// @param[out] device_list 设备名称列表，用于保存设备连接名称和参数，如果是网络连接，则名称格式为ip:port,如192.168.0.2:7896，如果是串口，则为port:bandrate，如 /dev/ttyACM0:115200。各设备名称之间，采用;隔开。
/// @param[in] timeout 超时值
/// @note device_list的内存由用户负责创建和销毁，最大内存大小为256。对于网络连接的设备来说，本功能，是通过发送UDP广播来实现的，因此，需要被搜索的设备和搜索设备在同一个网段。对于串口连接的设备来说，在Android/linux系统里面，将搜索/dev/ttyACM开头的所有设备，并尝试连接和通讯，在windows系统里面，将主动搜索所有系统串口，并尝试连接和通讯。
/// @return 返回值小于0，表示失败，大于等于零，表示搜索到的的NaviPack设备的数量。
NAVIPACK_API int DLLCALL SearchDevice(int id, char *device_list, int timeout)
{
	return -1;
}

/// 检查通讯是否正常
/// @return 返回值小于0，表示失败，等于0表示成功
NAVIPACK_API int DLLCALL CheckConnection()
{
	return -1;
}

NAVIPACK_API int DLLCALL UpdateNaviPackFile(int id, const char* fileName)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->UpdateNaviPackFile(fileName);
}

NAVIPACK_API int DLLCALL SetSelfStream(int id, char* buf, int bufLen)
{
	//SdkProtocolHeader* header = (SdkProtocolHeader*)buf;
	//float *a = (float*)(buf + sizeof(SdkProtocolHeader));
	//LOGE("SetSelfStream %d %d %d %d %f", header->deviceAddr, header->functionCode, header->startAddr, header->len,(float)(*a));

	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetSelfStream(buf, bufLen);
}

NAVIPACK_API int DLLCALL SetSaveMap(int id, const char* filePath, const char* fileName)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetSaveMap(filePath, fileName);
}

NAVIPACK_API int DLLCALL SendFile(int id, int type, const char* filePath, const char* fileName)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SendFile(type, filePath, fileName);
}


NAVIPACK_API int DLLCALL SetChangeNaviPackMode(int id, int mode)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SetChangeNaviPackMode(mode);
}


/// 手动进行IMU的矫正
/// @param[in] id		NaviPack对象ID
/// @return 返回值小于0，表示失败，等于0表示成功
NAVIPACK_API int DLLCALL ImuCalibrate(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->ImuCalibrate();
}


/// 发送自己的传感器数据到NaviPack
/// @param[in] id				NaviPack对象ID
/// @param[in] sensorData		传感器数据的统一格式
/// @return 返回值小于0，表示失败，等于0表示成功
NAVIPACK_API int DLLCALL SendUnifiedSensor(int id, UnifiedSensorInfo sensorData)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SendUnifiedSensor(id,sensorData);
}