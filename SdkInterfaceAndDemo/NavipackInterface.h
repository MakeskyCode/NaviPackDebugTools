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

	//断开链接
	int DisConnect();
	//重新链接
	int ReConnect();

	// 读取载体的参数
	int GetCarrierParam(CarrierParam *param);
	// 设置载体参数
	int SetCarrierParam(CarrierParam *param);
	// 读取NaviPack的参数 包含了NaviPack系统的一些参数信息，包含是否自动更新地图，栅格大小等一些参数。
	int GetNaviPackParam(NaviPackParam *param);

	/// 设置载体的参数
	/// @note NaviPackParam 包含了NaviPack系统的一些参数信息，包含是否自动更新地图，栅格大小等一些参数。
	int SetNaviPackParam(NaviPackParam *param);

	/// 设置目标点列表 可以一次设置多个目标点，设置完成后，NaviPack即进入自动导航状态，
	int SetTarget(int posX[], int posY[], int num,int phi);


	/// 获取当前已经设置的路径点
	int GetCurrentPath(int posX[], int posY[], int* num);

	/// 像素坐标与世界坐标之间的转换
	int PixelToPosition(int pixel_x, int pixel_y, int *position_x, int *position_y);

	/// 世界坐标与像素坐标之间的转换
	int PositionToPixel(int position_x, int position_y, int *pixel_x, int *pixel_y);

	/// 控制设备，以线速度v，角速度w运动。
	int SetSpeed(int v, int w);

	/// 控制NaviPack进入自动回充状态
	int AutoCharge();

	/// 设置充电桩的位置
	int SetChargerPosition(int position_x, int position_y);

	/// 进入建图模式
	int StartMapping(int mode);

	/// 退出建图模式
	int StopMapping(int save_flag);

	int EnableMapUpdate(bool bUpdate_flag);
	/// 读取NaviPack中的地图列表
	/// @param[in] id NaviPack对象ID
	/// @param[out] id_buffer 用于存放地图ID的缓冲区。该缓冲区最大为8个。
	/// @note 该函数将读取系统中保存的所有的地图ID。
	/// @return 返回值小于0，表示失败，大于或等于零，表示当前系统中的地图个数。
	int GetMapList(int *id_buffer);

	/// 指定NaviPack载入指定的地图
	/// @param[in] id NaviPack对象ID
	/// @param[in] map_id 指定地图的ID：0~7
	/// @note 大部分与地图相关的函数，需要载入地图后，才可以工作。
	/// @return 返回值小于0，表示失败，等于0 表示成功
	int LoadMap(int map_id);

	/// 设置获取当前使用的地图
	/// @param[in] id NaviPack对象ID
	/// @note 当连接成功或者需要更新本机地图时，可以调用该函数来通知NaviPack来上传地图
	/// @return 返回值小于0，表示失败，等于0 表示成功
	int SetGetCurrentMap();

	/// 载入本地的地图文件到NaviPack
	/// @param[in] id NaviPack对象ID
	/// @param[in] local_map_path 本地地图文件路径
	/// @param[in] map_id 设置本地地图对应的NaviPack上地图的ID
	/// @note 该函数用于下载上位机本地的地图文件到NaviPack，并将其在NaviPack里面的地图ID，
	/// 设置成map_id值。大部分与地图相关的函数，需要载入地图后，才可以工作。
	/// @return 返回值小于0，表示失败，等于0 表示成功
	int LoadLocalMap(const char* local_map_path, int map_id);

	/// 保存当前的NaviPack上运行的地图到上位机本地
	/// @param[in] id NaviPack对象ID
	/// @param[in] local_map_path 本地地图文件路径
	/// @param[in] picture_flag 决定在保存地图文件的同时，是否将不同图层的数据，转为相应地bmp图片文件。1 表示保存，0表示不保存
	/// @note 该函数将当前NaviPack上运行的地图，保存到上位机本地，文件扩展名为.npmap。同时，如果picture_flag设置为1，
	/// 其将会而外将激光雷达数据图层以及超声波数据图层保存为bmp格式的图片，方便用户查看。
	/// @return 返回值小于0，表示失败，等于0 表示成功
	int SaveMapToLocal(const char * local_map_path, int picture_flag);


	int GetMapLayer(AlgMapData& mapdata, int mapType);

	/// 更新NaviPack所使用的地图图层
	/// @param[in] id NaviPack对象ID
	/// @param[in] map_data MapData结构体，用于保存地图数据
	/// @param[in] map_type 不同的地图类型。可以是激光雷达图层、超声波图层、碰撞图层等，自定义图层，组合图层等
	/// @return 返回值小于0，表示失败，等于0 表示成功
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


