#pragma once
#include "tools/SDKProtocol.h"

#include <string>
#include "tools/CriticalSection.h"
#include "hardware_interface/TCPClientInterface.h"
#include "hardware_interface/SerialPortInterface.h"
#include "errordeal/ErrorEvent.h"
#include <inttypes.h>
#include "tools/CFile.h"
#define MAX_INTERFACE 1




class CNavipackInterface
{
public:
	static CNavipackInterface* navi_interface[MAX_INTERFACE];

	static int CreateInstance(int& id,ConnectType connectType);
	static void deleteInstance(int id);

	~CNavipackInterface();
	int SetGetNaviPackVersion();
	int Connect(const char* name, int param);

	//�Ͽ�����
	int DisConnect();
	//��������
	int ReConnect();

	// ��ȡ����Ĳ���
	int GetCarrierParam(CarrierParam *param);
	// �����������
	int SetCarrierParam(CarrierParam *param);
	// ��ȡNaviPack�Ĳ��� ������NaviPackϵͳ��һЩ������Ϣ�������Ƿ��Զ����µ�ͼ��դ���С��һЩ������
	int GetNaviPackParam(NaviPackParam *param);

	/// ��������Ĳ���
	/// @note NaviPackParam ������NaviPackϵͳ��һЩ������Ϣ�������Ƿ��Զ����µ�ͼ��դ���С��һЩ������
	int SetNaviPackParam(NaviPackParam *param);

	/// ����Ŀ����б� ����һ�����ö��Ŀ��㣬������ɺ�NaviPack�������Զ�����״̬��
	int SetTarget(int posX[], int posY[], int num,int phi);


	/// ��ȡ��ǰ�Ѿ����õ�·����
	int GetCurrentPath(int posX[], int posY[], int* num);

	/// ������������������֮���ת��
	int PixelToPosition(int pixel_x, int pixel_y, int *position_x, int *position_y);

	/// ������������������֮���ת��
	int PositionToPixel(int position_x, int position_y, int *pixel_x, int *pixel_y);

	/// �����豸�������ٶ�v�����ٶ�w�˶���
	int SetSpeed(int v, int w);

	/// ����NaviPack�����Զ��س�״̬
	int AutoCharge();

	/// ���ó��׮��λ��
	int SetChargerPosition(int position_x, int position_y);

	/// ���뽨ͼģʽ
	int StartMapping(int mode);

	/// �˳���ͼģʽ
	int StopMapping(int save_flag);

	int EnableMapUpdate(bool bUpdate_flag);
	/// ��ȡNaviPack�еĵ�ͼ�б�
	/// @param[in] id NaviPack����ID
	/// @param[out] id_buffer ���ڴ�ŵ�ͼID�Ļ��������û��������Ϊ8����
	/// @note �ú�������ȡϵͳ�б�������еĵ�ͼID��
	/// @return ����ֵС��0����ʾʧ�ܣ����ڻ�����㣬��ʾ��ǰϵͳ�еĵ�ͼ������
	int GetMapList(int *id_buffer);

	/// ָ��NaviPack����ָ���ĵ�ͼ
	/// @param[in] id NaviPack����ID
	/// @param[in] map_id ָ����ͼ��ID��0~7
	/// @note �󲿷����ͼ��صĺ�������Ҫ�����ͼ�󣬲ſ��Թ�����
	/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
	int LoadMap(int map_id);

	/// ���û�ȡ��ǰʹ�õĵ�ͼ
	/// @param[in] id NaviPack����ID
	/// @note �����ӳɹ�������Ҫ���±�����ͼʱ�����Ե��øú�����֪ͨNaviPack���ϴ���ͼ
	/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
	int SetGetCurrentMap();

	/// ���뱾�صĵ�ͼ�ļ���NaviPack
	/// @param[in] id NaviPack����ID
	/// @param[in] local_map_path ���ص�ͼ�ļ�·��
	/// @param[in] map_id ���ñ��ص�ͼ��Ӧ��NaviPack�ϵ�ͼ��ID
	/// @note �ú�������������λ�����صĵ�ͼ�ļ���NaviPack����������NaviPack����ĵ�ͼID��
	/// ���ó�map_idֵ���󲿷����ͼ��صĺ�������Ҫ�����ͼ�󣬲ſ��Թ�����
	/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
	int LoadLocalMap(const char* local_map_path, int map_id);

	/// ���浱ǰ��NaviPack�����еĵ�ͼ����λ������
	/// @param[in] id NaviPack����ID
	/// @param[in] local_map_path ���ص�ͼ�ļ�·��
	/// @param[in] picture_flag �����ڱ����ͼ�ļ���ͬʱ���Ƿ񽫲�ͬͼ������ݣ�תΪ��Ӧ��bmpͼƬ�ļ���1 ��ʾ���棬0��ʾ������
	/// @note �ú�������ǰNaviPack�����еĵ�ͼ�����浽��λ�����أ��ļ���չ��Ϊ.npmap��ͬʱ�����picture_flag����Ϊ1��
	/// �佫����⽫�����״�����ͼ���Լ�����������ͼ�㱣��Ϊbmp��ʽ��ͼƬ�������û��鿴��
	/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
	int SaveMapToLocal(const char * local_map_path, int picture_flag);


	int GetMapLayer(AlgMapData& mapdata, int mapType);

	/// ����NaviPack��ʹ�õĵ�ͼͼ��
	/// @param[in] id NaviPack����ID
	/// @param[in] map_data MapData�ṹ�壬���ڱ����ͼ����
	/// @param[in] map_type ��ͬ�ĵ�ͼ���͡������Ǽ����״�ͼ�㡢������ͼ�㡢��ײͼ��ȣ��Զ���ͼ�㣬���ͼ���
	/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
	int SetMapLayer(AlgMapData *map_data, int map_type);

	int GetSensorData(AlgSensorData& data, int sensorType);

	int SaveCurrentMapToList(int mapId);

	int ReadStatusRegister(AlgStatusRegister& status);

	int InitLocation();
	int StopInitLocation();

	int UpdateNaviPackFile(const char * fileName);

	int SetSelfStream(char* buf, int bufLen);

	int SetSaveMap(const char* filePath, const char* fileName);

	int SendFile(int type, const char* filePath, const char* fileName);

	int SetWiFiParam(const  char * ssid, const  char * password);

	int SetChangeNaviPackMode(int mode);

	int ImuCalibrate();

	int UpdateLidarFirmware();
	int DoCleanTask();
	int OptimizeMap(bool enable_flag);


	int SendUnifiedSensor(int id, UnifiedSensorInfo sensorData);

	void SetCallbacks(DeviceMsgCallBack deviceMsgCb, RobotMsgCallBack errMsgCallBack,
		MapPackageCallBack mapPackCb, LidarPackageCallBack lidarPackCb);
	static void RxDataCallBack(int32_t id, void *param, const uint8_t *data, int32_t len);


private:

	DeviceMsgCallBack mDeviceMsgCallBack;
	RobotMsgCallBack mErrorMsgCallBack;
	MapPackageCallBack mMapPackageCallBack;
	LidarPackageCallBack mLidarPackageCb;

	static bool isInterfaceInit;
	ConnectType mConnectType;
	CNavipackInterface(ConnectType type);
	AlgControlRegister* mControlRegister;
	AlgStatusRegister* mStatusRegister;
	AlgTargetPos* mSetTargetPos;
	AlgTargetPos* mPlanedPathPos;
	AlgTargetPos* mLastPlanedPathPos;
	AlgSensorData* mRealLidarData;
	AlgSensorData* mRealUltrasonData;
	AlgSensorData* mRealCollisionData;
	AlgSensorData* mRealGroundData;

	AlgMapData* mMap;
	AlgMapData* mLidarMap;
	AlgMapData* mUltrasonMap;
	AlgMapData* mCollisionMap;
	AlgMapData* mgroundMap;
	CarrierParam* mCarrierParam;
	MapListInfo mMapListInfo;

	CriticalSection lidar_cs;
	CriticalSection Map_cs;
	CriticalSection Path_cs;
	CriticalSection StatusRegister_cs;
	CriticalSection SetTargetPos_cs;
	CriticalSection pathPos_cs;
	CriticalSection carrierParam_cs;

	AlgControlRegister algControlRegister;


	bool isConnect;

	HardwareInterface *mHwInterface;

	static int navi_interfaceidex;

	void GetAlgMapDataFromRxData(AlgMapData* mapData, const uint8_t* data,uint32_t len);
	void GetAlgMapDataFromRxDataLz4(AlgMapData* mapData, const uint8_t* data, uint32_t len);

	CFile mSaveMapFile;

	int SendFile(u8 type,const char * fileName);
};


