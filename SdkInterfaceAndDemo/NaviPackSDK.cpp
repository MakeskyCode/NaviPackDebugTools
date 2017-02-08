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

//������ľ��
NAVIPACK_API int DLLCALL Create(ConnectType conType)
{
	printf("Create conType = %d\n", conType);
	int pID = -1;
	int ret = CNavipackInterface::CreateInstance(pID, conType);
	return pID;
}

//�����豸
NAVIPACK_API int DLLCALL Open(int id, const char* name, int param) //ip��ַ���˿ں�
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

/// ���ôӷ���˴�����Ϣ���ݵĻص�
/// @param[in] id NaviPack����ID
/// @param[in] deviceMsgCb �豸��Ϣ�ص�
/// @return  ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
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

//����Ŀ���
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

//��ȡ��ǰ·��
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

//�����ٶ�
NAVIPACK_API int DLLCALL SetSpeed(int id, int v, int w)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetSpeed(v, w);
}

//�س�
NAVIPACK_API int DLLCALL AutoCharge(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->AutoCharge();
}

//���ûس�����
NAVIPACK_API int DLLCALL SetChargerPosition(int id, int position_x, int position_y)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}

	return CNavipackInterface::navi_interface[id]->SetChargerPosition(position_x, position_y);
}

// ���ûس�����
// NAVIPACK_API int DLLCALL SetChargeThreshold(int id, u8 threshold)
// {
// 	return 0;
// }

//��ͼ����
//mappingMode�Զ����ֶ�
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

/// ���û�ȡ��ǰʹ�õĵ�ͼ
/// @param[in] id NaviPack����ID
/// @note �����ӳɹ�������Ҫ���±�����ͼʱ�����Ե��øú�����֪ͨNaviPack���ϴ���ͼ
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
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

//��ȡ״̬�Ĵ�������
NAVIPACK_API int DLLCALL GetStatus(int id, AlgStatusRegister* status)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->ReadStatusRegister(*status);
}

//��ʼ��λ
NAVIPACK_API int DLLCALL InitLocation(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->InitLocation();
}

//�ͷ���Դ�����پ��
NAVIPACK_API void DLLCALL Destroy(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return;
	}
	CNavipackInterface::deleteInstance(id);

}


/// ����WIFI����
/// @param[in] id NaviPack����ID
/// @param[in] ssid WIFI SSID
/// @param[in] password WIFI ����
/// @note ������SSID��WIFI֮��ϵͳ����������TCP/UDP�����������е�Ӳ����Ҫ����USB WIFI �����ſ���ʹ��WIFI���ӣ���
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
NAVIPACK_API int DLLCALL SetWiFiParam(int id, const  char * ssid, const  char * password)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SetWiFiParam(ssid, password);
}

/// ���������е��豸
/// @param[in] id NaviPack����ID
/// @param[out] device_list �豸�����б����ڱ����豸�������ƺͲ�����������������ӣ������Ƹ�ʽΪip:port,��192.168.0.2:7896������Ǵ��ڣ���Ϊport:bandrate���� /dev/ttyACM0:115200�����豸����֮�䣬����;������
/// @param[in] timeout ��ʱֵ
/// @note device_list���ڴ����û����𴴽������٣�����ڴ��СΪ256�������������ӵ��豸��˵�������ܣ���ͨ������UDP�㲥��ʵ�ֵģ���ˣ���Ҫ���������豸�������豸��ͬһ�����Ρ����ڴ������ӵ��豸��˵����Android/linuxϵͳ���棬������/dev/ttyACM��ͷ�������豸�����������Ӻ�ͨѶ����windowsϵͳ���棬��������������ϵͳ���ڣ����������Ӻ�ͨѶ��
/// @return ����ֵС��0����ʾʧ�ܣ����ڵ����㣬��ʾ�������ĵ�NaviPack�豸��������
NAVIPACK_API int DLLCALL SearchDevice(int id, char *device_list, int timeout)
{
	return -1;
}

/// ���ͨѶ�Ƿ�����
/// @return ����ֵС��0����ʾʧ�ܣ�����0��ʾ�ɹ�
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


/// �ֶ�����IMU�Ľ���
/// @param[in] id		NaviPack����ID
/// @return ����ֵС��0����ʾʧ�ܣ�����0��ʾ�ɹ�
NAVIPACK_API int DLLCALL ImuCalibrate(int id)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->ImuCalibrate();
}


/// �����Լ��Ĵ��������ݵ�NaviPack
/// @param[in] id				NaviPack����ID
/// @param[in] sensorData		���������ݵ�ͳһ��ʽ
/// @return ����ֵС��0����ʾʧ�ܣ�����0��ʾ�ɹ�
NAVIPACK_API int DLLCALL SendUnifiedSensor(int id, UnifiedSensorInfo sensorData)
{
	if (id >= MAX_INTERFACE || CNavipackInterface::navi_interface[id] == NULL)
	{
		return -1;
	}
	return CNavipackInterface::navi_interface[id]->SendUnifiedSensor(id,sensorData);
}