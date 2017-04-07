#include "NavipackInterface.h"
#include "string.h"
#include <stdio.h>
#include "tools/LogFile.h"
#include "tools/systemenc/Compress.h"
#include "errordeal/ErrorEvent.h"
#include "tools/systemenc/md5.h"
#include "tools/file_dir.h"

#include "tools/SDKProtocolPrivate.h"
#include "lz4/lz4.h"


#ifdef NAVIPACK_WIN
#include <io.h>
#endif


#define SERIAL_IF_NAME "\\\\.\\COM7"
#define SERIAL_IF_PARAM1 7//Port No
#define SERIAL_IF_PARAM2 115200//Baud Rate


int CNavipackInterface::navi_interfaceidex = 0;
CNavipackInterface* CNavipackInterface::navi_interface[MAX_INTERFACE];
bool CNavipackInterface::isInterfaceInit = false;
// CNavipackInterface* navi_interface;

//exist bug ! �����ӳ���8��֮�󼴱�ȫ���Ͽ�Ҳ�����ٽ������� 2016/07/26 gk
int CNavipackInterface::CreateInstance(int& id, ConnectType connectType)
{
	
	if (!isInterfaceInit)
	{
		LOGD("init navi_interface \n");
		for (int i = 0; i < MAX_INTERFACE; i++)
		{
			navi_interface[i] = NULL;
		}

		isInterfaceInit = true;
	}

	if (navi_interfaceidex >= MAX_INTERFACE)
	{
		id = -1;
		return -1;
	}

	for (int i = 0; i < MAX_INTERFACE; i++)
	{
		LOGD("navi_interface[%d] : %p \n", i, navi_interface[i]);
		if (NULL == navi_interface[i])
		{
			id = i;
			navi_interface[i] = new CNavipackInterface(connectType);
			navi_interfaceidex++;
			return 0;
		}
	}

	return -1;
}

//exist bug ! �����ӳ���8��֮�󼴱�ȫ���Ͽ�Ҳ�����ٽ������� 2016/07/26 gk
void CNavipackInterface::deleteInstance(int id)
{
	int delId = 0;
	LOGD("deleteInstance %d", delId);
	navi_interface[delId]->DisConnect();
	delete navi_interface[delId];
	navi_interface[delId] = NULL;
	navi_interfaceidex--;
}

CNavipackInterface::CNavipackInterface(ConnectType type)
{
	mConnectType = type;
	mControlRegister = new AlgControlRegister;
	mStatusRegister = new AlgStatusRegister;
	mSetTargetPos = new AlgTargetPos;
	mPlanedPathPos = new AlgTargetPos;
	mRealLidarData = new AlgSensorData;
	mRealLidarData->num = 0;
	mRealUltrasonData = new AlgSensorData;
	mRealUltrasonData->num = 0;
	mRealCollisionData = new AlgSensorData;
	mRealCollisionData->num = 0;
	mRealGroundData = new AlgSensorData;
	mRealGroundData->num = 0;
	mMapListInfo.mapListNum = 0;

	mMap = new AlgMapData;
	mLidarMap = new AlgMapData;
	mUltrasonMap = new AlgMapData;
	mCollisionMap = new AlgMapData;
	mgroundMap = new AlgMapData;
	mCarrierParam = new CarrierParam;

	mDeviceMsgCallBack = NULL;
	mErrorMsgCallBack = NULL;
	mMapPackageCallBack = NULL;
	mLidarPackageCb = NULL;


	isConnect = false;

	if (TCP_CONNECT == mConnectType)
	{
		mHwInterface = new TCPClientInterface(NETWORK_INTERFACE_ID,128*1024);
		mHwInterface->AddRxCallBack(PT_SERAL_PACKAGE, RxDataCallBack, this,1*1024*1024);
		LOG("---------->>>>>>>> TCPClientInterface AddRxCallBack ");
	}
	else if (SERIAL_CONNECT == mConnectType)
	{
		mHwInterface = new SerialPortInterface(USB_HOST_INTERFACE_ID,128*1024);
		mHwInterface->AddRxCallBack(PT_SERAL_PACKAGE, RxDataCallBack, this,1*1024*1024);
		LOG("---------->>>>>>>> SerialPortInterface AddRxCallBack");
	}


}

void CNavipackInterface::SetCallbacks(DeviceMsgCallBack deviceMsgCb, RobotMsgCallBack errMsgCallBack,
	MapPackageCallBack mapPackCb, LidarPackageCallBack lidarPackCb)
{
	mDeviceMsgCallBack = deviceMsgCb;
	mErrorMsgCallBack = errMsgCallBack;
	mMapPackageCallBack = mapPackCb;
	mLidarPackageCb = lidarPackCb;
}


CNavipackInterface::~CNavipackInterface()
{
	delete mControlRegister;
	delete mStatusRegister;
	delete mSetTargetPos;
	delete mPlanedPathPos;
	delete mRealLidarData;
	delete mRealUltrasonData;
	delete mRealCollisionData;
	delete mRealGroundData;
	delete mMap;
	delete mLidarMap;
	delete mUltrasonMap;
	delete mCollisionMap;
	delete mgroundMap;
	delete mHwInterface;
	delete mCarrierParam;
}

int CNavipackInterface::SetGetNaviPackVersion()
{
	SdkProtocolHeader header;
	header.deviceAddr = DEVICE_MSG;
	header.functionCode = GET_NAVIPACK_VERSION;
	int ret = -1;
	if (isConnect && mHwInterface != NULL)
	{
		ret = mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	}
	return ret;
}

int CNavipackInterface::Connect(const char* name, int param)
{
	// 	uint32_t ip = (127 << 6) + (0 << 4) + (0 << 2) + (1 << 0);
	char fileName[100];
	strcpy(fileName, name);

	LOGD("TCP/SERIAL name :%s  and param : %d\n", fileName, param);
	int ret;
	ret = mHwInterface->Open(name, param, param);
	if (0 == ret)
	{
		isConnect = true;
		//���͸��µ�ͼ��Ϣ
		SdkProtocolHeader header;
		header.deviceAddr = DEVICE_MSG;
		header.functionCode = UPDATE_MAP;
		mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));

		header.deviceAddr = DEVICE_MSG;
		header.functionCode = UPDATE_MAP_LIST;
		mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));

	}

	return ret;

}

int CNavipackInterface::DisConnect()
{
	if (mHwInterface != NULL)
	{
		mHwInterface->Close();
	}
	return 0;
}

int CNavipackInterface::ReConnect()
{
	int ret = -1;
	if (mHwInterface != NULL)
	{
		ret = mHwInterface->ReOpen();
	}
	return ret;
}

// ��ȡ����Ĳ���
int CNavipackInterface::GetCarrierParam(CarrierParam *param)
{
	return 0;
}

// ��������Ĳ���
int CNavipackInterface::SetCarrierParam(CarrierParam *param)
{
	return -1;
}

/// ��ȡNaviPack�Ĳ���
/// @param[in] id NaviPack����ID
/// @param[out] param ����ָ��
/// @note NaviPackParam ������NaviPackϵͳ��һЩ������Ϣ�������Ƿ��Զ����µ�ͼ��դ���С��һЩ������
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
int CNavipackInterface::GetNaviPackParam(NaviPackParam *param)
{
	return -1;
}

/// ��������Ĳ���
/// @note NaviPackParam ������NaviPackϵͳ��һЩ������Ϣ�������Ƿ��Զ����µ�ͼ��դ���С��һЩ������

int CNavipackInterface::SetNaviPackParam(NaviPackParam *param)
{
	return -1;
}

/// ������������������֮���ת��
int CNavipackInterface::PixelToPosition(int pixel_x, int pixel_y, int *position_x, int *position_y)
{
	//todo deven
	return -1;
}

/// ������������������֮���ת��
int CNavipackInterface::PositionToPixel(int position_x, int position_y, int *pixel_x, int *pixel_y)
{
	return -1;
}


/// ����NaviPack�����Զ��س�״̬
int CNavipackInterface::AutoCharge()
{
	SdkProtocolHeader head;
	head.deviceAddr = ALG_ADDRESS;
	head.functionCode = ALG_CONTROL_REG;
	head.startAddr = ALG_CR_ADDR_BACK_CHARGE;
	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&head, sizeof(SdkProtocolHeader));
	}
	return -1;
}

/// ���ó��׮��λ��
int CNavipackInterface::SetChargerPosition(int position_x, int position_y)
{
	char buf[sizeof(SdkProtocolHeader) + sizeof(IntPoint)];
	SdkProtocolHeader *head = (SdkProtocolHeader*)buf;
	head->deviceAddr = DEVICE_MSG;
	head->functionCode = SET_CHARGE_POSITION;

	IntPoint *point = (IntPoint*)&buf[sizeof(SdkProtocolHeader)];
	point->x = position_x;
	point->y = position_y;

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)buf, sizeof(buf));
	}
	return -1;
}

int CNavipackInterface::SetTarget(int posX[], int posY[], int num, int phi)
{
	SetTargetPos_cs.Enter();
	if (num > MAX_TARGET_LIST_SIZE) num = MAX_TARGET_LIST_SIZE;


	u8* buf = new u8[sizeof(AlgTargetPos) + sizeof(SdkProtocolHeader)];

	SdkProtocolHeader *head = (SdkProtocolHeader*)buf;
	head->deviceAddr = ALG_ADDRESS;
	head->functionCode = ALG_CONTROL_REG;
	head->startAddr = ALG_CR_ADDR_SET_TARGET;

	mSetTargetPos->nTargetPosNum = num;
	mSetTargetPos->phi = phi;
	memcpy(mSetTargetPos->PathPosX, posX, num * sizeof(s32));
	memcpy(mSetTargetPos->PathPosY, posY, num * sizeof(s32));

	memcpy(&buf[sizeof(SdkProtocolHeader)], mSetTargetPos, sizeof(AlgTargetPos));

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, sizeof(AlgTargetPos) + sizeof(SdkProtocolHeader));
	}
	delete[] buf;
	SetTargetPos_cs.Leave();
	return 0;
}

int CNavipackInterface::GetCurrentPath(int posX[], int posY[], int* num)
{
	pathPos_cs.Enter();
	for (int n = 0; n < mPlanedPathPos->nTargetPosNum; n++)
	{
		posX[n] = mPlanedPathPos->PathPosX[n];
		posY[n] = mPlanedPathPos->PathPosY[n];
	}
	*num = mPlanedPathPos->nTargetPosNum;

	pathPos_cs.Leave();
	return 0;
}

int CNavipackInterface::SetSpeed(int v, int w)
{
	mControlRegister->setLineVelocity = v;
	mControlRegister->setAngularVelocity = w;
	u8 *buf = new u8[sizeof(SdkProtocolHeader) + 3 * sizeof(int)];
	int data_len = 0;

	SdkProtocolHeader head;
	head.deviceAddr = ALG_ADDRESS;
	head.functionCode = ALG_CONTROL_REG;
	head.startAddr = ALG_CR_ADDR_SPEED_CONTROL;
	head.len = sizeof(s32) + sizeof(s32);
	memcpy(buf, (u8*)&head, sizeof(SdkProtocolHeader)); data_len += sizeof(SdkProtocolHeader);
	memcpy(buf + data_len, &v, sizeof(float)); data_len += sizeof(s32);
	memcpy(buf + data_len, &w, sizeof(float)); data_len += sizeof(s32);

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, data_len);
	}
	delete[] buf;
	return 0;
}

int CNavipackInterface::StartMapping(int mode)
{
	SdkProtocolHeader head;
	int data_len = 0;
	u8 *buf = new u8[sizeof(SdkProtocolHeader) + 2];
	head.deviceAddr = ALG_ADDRESS;
	if (mode == 0)
	{
		head.functionCode = ALG_BUILD_MAP_MANUAL;
	}
	else
	{
		head.functionCode = ALG_BUILD_MAP_AUTO;
	}
	head.startAddr = ALG_CR_ADDR_MAP_BUILDER;
	memcpy(buf, (u8*)&head, sizeof(SdkProtocolHeader)); data_len += sizeof(SdkProtocolHeader);

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, data_len);
	}
	delete[] buf;
	return 0;
}

int CNavipackInterface::StopMapping(int save_flag)
{
	mControlRegister->setDefaultMap = save_flag;
	SdkProtocolHeader head;
	int data_len = 0;
	u8 *buf = new u8[sizeof(SdkProtocolHeader) + sizeof(u8)];
	head.deviceAddr = ALG_ADDRESS;
	head.functionCode = ALG_SAVE_MAP;
	memcpy(buf, (u8*)&head, sizeof(SdkProtocolHeader)); data_len += sizeof(SdkProtocolHeader);
	memcpy(buf + data_len, (u8*)&mControlRegister->setDefaultMap, sizeof(u8)); data_len += sizeof(u8);


	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, data_len);
	}
	//if (tcpClientConnected)
	//{
	//	tcpClient->WriteData(PT_SERAL_PACKAGE,buf, data_len);
	//}
	//else if (serialPortConnected)
	//{
	//	serialPort->WriteData(PT_SERAL_PACKAGE,buf, data_len);
	//}
	delete[] buf;
	return 0;
}

int CNavipackInterface::EnableMapUpdate(bool bUpdate_flag)
{
	int nUpdate = (bUpdate_flag == true ? 1 : 0);
	SdkProtocolHeader head;
	int data_len = 0;
	u8 *buf = new u8[sizeof(SdkProtocolHeader) + sizeof(u8)];
	head.deviceAddr = ALG_ADDRESS;
	head.functionCode = ALG_ENABLE_MAP_UPDATE;
	memcpy(buf, (u8*)&head, sizeof(SdkProtocolHeader)); data_len += sizeof(SdkProtocolHeader);
	memcpy(buf + data_len, (u8*)&nUpdate, sizeof(u8)); data_len += sizeof(u8);

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, data_len);
	}
	delete[] buf;
	return 0;
}

/// ��ȡNaviPack�еĵ�ͼ�б�
/// @param[in] id NaviPack����ID
/// @param[out] id_buffer ���ڴ�ŵ�ͼID�Ļ��������û��������Ϊ8����
/// @note �ú�������ȡϵͳ�б�������еĵ�ͼID��
/// @return ����ֵС��0����ʾʧ�ܣ����ڻ�����㣬��ʾ��ǰϵͳ�еĵ�ͼ������
int CNavipackInterface::GetMapList(int *id_buffer)
{
	memcpy(id_buffer, mMapListInfo.mapListId , mMapListInfo.mapListNum * sizeof(int));
	return mMapListInfo.mapListNum;		//���ظ���
}

int CNavipackInterface::SaveCurrentMapToList(int mapId)
{
	//���͸��µ�ͼ��Ϣ
	SdkProtocolHeader header;
	header.deviceAddr = DEVICE_MSG;
	header.functionCode = SET_SAVE_CURRENT_MAP;
	header.startAddr = mapId;
	int ret = mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	if (ret > 0)
	{
		return 0;
	}
	return -1;
}

/// ָ��NaviPack����ָ���ĵ�ͼ
/// @param[in] id NaviPack����ID
/// @param[in] map_id ָ����ͼ��ID��0~7
/// @note �󲿷����ͼ��صĺ�������Ҫ�����ͼ�󣬲ſ��Թ�����
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
int CNavipackInterface::LoadMap(int map_id)
{
	SdkProtocolHeader header;
	header.deviceAddr = DEVICE_MSG;
	header.functionCode = SET_LOAD_MAP_FROME_LIST;
	header.startAddr = map_id;
	int ret = mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	if (ret > 0)
	{
		return 0;
	}
	return -1;
}

/// ���û�ȡ��ǰʹ�õĵ�ͼ
/// @param[in] id NaviPack����ID
/// @note �����ӳɹ�������Ҫ���±�����ͼʱ�����Ե��øú�����֪ͨNaviPack���ϴ���ͼ
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
int CNavipackInterface::SetGetCurrentMap()
{
	//���͸��µ�ͼ��Ϣ
	SdkProtocolHeader header;
	header.deviceAddr = DEVICE_MSG;
	header.functionCode = UPDATE_MAP;
	int ret = mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	if (ret > 0)
	{
		return 0;
	}
	return -1;
}

/// ���뱾�صĵ�ͼ�ļ���NaviPack
/// @param[in] id NaviPack����ID
/// @param[in] local_map_path ���ص�ͼ�ļ�·��
/// @param[in] map_id ���ñ��ص�ͼ��Ӧ��NaviPack�ϵ�ͼ��ID
/// @note �ú�������������λ�����صĵ�ͼ�ļ���NaviPack����������NaviPack����ĵ�ͼID��
/// ���ó�map_idֵ���󲿷����ͼ��صĺ�������Ҫ�����ͼ�󣬲ſ��Թ�����
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
int CNavipackInterface::LoadLocalMap(const char* local_map_path, int map_id)
{
	return SendFile(SEND_FILE_TYPE_MAP_PACKAGE, local_map_path);
}

/// ���浱ǰ��NaviPack�����еĵ�ͼ����λ������
/// @param[in] id NaviPack����ID
/// @param[in] local_map_path ���ص�ͼ�ļ�·��
/// @param[in] picture_flag �����ڱ����ͼ�ļ���ͬʱ���Ƿ񽫲�ͬͼ������ݣ�תΪ��Ӧ��bmpͼƬ�ļ���1 ��ʾ���棬0��ʾ������
/// @note �ú�������ǰNaviPack�����еĵ�ͼ�����浽��λ�����أ��ļ���չ��Ϊ.npmap��ͬʱ�����picture_flag����Ϊ1��
/// �佫����⽫�����״�����ͼ���Լ�����������ͼ�㱣��Ϊbmp��ʽ��ͼƬ�������û��鿴��
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
int CNavipackInterface::SaveMapToLocal(const char * local_map_path, int picture_flag)
{
	LOGD("CNavipackInterface::SaveMapToLocal local_map_path(%s)", local_map_path);

	char file[FILE_PATH_MAX_LEN];
	if (strlen(local_map_path) >= sizeof(file))
	{
		LOGD("Path Name too long");
		return -2;
	}

	memset(file, 0, sizeof(file));

	int data_len = sizeof(SdkProtocolHeader) + sizeof(file);
	int ret = -1;

	u8 *buffer = new u8[data_len];
	SdkProtocolHeader *head = (SdkProtocolHeader*)buffer;
	head->deviceAddr = DEVICE_MSG;
	head->functionCode = SET_NAVAPACK_UPLOAD_MAP;

	sprintf(file, "%s", local_map_path);

	memcpy(&buffer[sizeof(SdkProtocolHeader)], file, sizeof(file));

	LOGD("SetSaveMap file(%s) name(%s)", file, &buffer[sizeof(SdkProtocolHeader)]);

	if (isConnect) {
		ret = mHwInterface->WriteData(PT_SERAL_PACKAGE, buffer, data_len);
	}

	return ret;

}

static void GetMapLayerData(AlgMapData* mapDst, AlgMapData* mapSrc)
{
	memcpy((u8*)mapDst, (u8*)mapSrc, sizeof(AlgMapData) - sizeof(mapSrc->map));
	memcpy(mapDst->map, mapSrc->map, MAX_MAP_SIZE);
}

int CNavipackInterface::GetMapLayer(AlgMapData& mapdata, int mapType)
{

	int ret = 0;
	switch (mapType)
	{
	case TOTOAL_MAP:
		Map_cs.Enter();
		GetMapLayerData(&mapdata, mMap);
		Map_cs.Leave();
		break;
	case ULTRASON_MAP:
		GetMapLayerData(&mapdata, mUltrasonMap);
		break;
	case COLLISION_MAP:
		GetMapLayerData(&mapdata, mCollisionMap);
		break;
	case GROUND_MAP:
		GetMapLayerData(&mapdata, mgroundMap);

		break;
	case LIDAR_MAP:
		GetMapLayerData(&mapdata, mLidarMap);

	default:
		ret = -1;
		break;
	}
	return ret;
}


/// ����NaviPack��ʹ�õĵ�ͼͼ��
/// @param[in] id NaviPack����ID
/// @param[in] map_data MapData�ṹ�壬���ڱ����ͼ����
/// @param[in] map_type ��ͬ�ĵ�ͼ���͡������Ǽ����״�ͼ�㡢������ͼ�㡢��ײͼ��ȣ��Զ���ͼ�㣬���ͼ���
/// @return ����ֵС��0����ʾʧ�ܣ�����0 ��ʾ�ɹ�
int CNavipackInterface::SetMapLayer(AlgMapData *map_data, int map_type)
{
	u8 *buf = new u8[sizeof(AlgMapData) + sizeof(SdkProtocolHeader) + MAX_MAP_SIZE*2];
	SdkProtocolHeader* head = (SdkProtocolHeader*)buf;
	head->deviceAddr = ALG_ADDRESS;
	head->functionCode = ALG_DATA_READ;
	head->startAddr = ALG_DATA_ADDR_LIDAR_MAP;

	memcpy(buf+sizeof(SdkProtocolHeader),map_data,sizeof(AlgMapData));

	u32 encodeLen = RunLenghEncoding(map_data->map, map_data->height*map_data->width, buf+sizeof(AlgMapData) + sizeof(SdkProtocolHeader), 255);
	//LOGD("----------------->>>>>>>>>>>>>>>>>>>>>>%d RunLenghEncoding %d  %d ", sizeof(AlgMapData), map_data->height*map_data->width, encodeLen);

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, sizeof(AlgMapData) + sizeof(SdkProtocolHeader) + encodeLen);
	}

	delete[] buf;
	return -1;
}

int CNavipackInterface::GetSensorData(AlgSensorData& data, int sensorType)
{
	lidar_cs.Enter();
	memcpy((u8*)&data, (u8*)mRealLidarData, sizeof(AlgSensorData));
	lidar_cs.Leave();
	return 0;
}

int CNavipackInterface::ReadStatusRegister(AlgStatusRegister& status)
{
	StatusRegister_cs.Enter();
	memcpy((u8*)&status, (u8*)mStatusRegister, sizeof(AlgStatusRegister));
	StatusRegister_cs.Leave();
	return 0;
}

/*
��ʼ����λ
*/
int CNavipackInterface::InitLocation()
{
	SdkProtocolHeader head;
	int data_len = 0;
	u8 *buf = new u8[sizeof(SdkProtocolHeader) + sizeof(u8) + 2];
	head.deviceAddr = ALG_ADDRESS;
	head.functionCode = ALG_LOCATION_PF;
	head.startAddr = ALG_CR_ADDR_IDLE;

	memcpy(buf, (u8*)&head, sizeof(SdkProtocolHeader));
	data_len += sizeof(SdkProtocolHeader);

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, data_len);
	}
	delete[] buf;
	return 0;
}
//#define T_BUF_SIZE (128*1024)
#define T_BUF_SIZE (120*1024)
int  CNavipackInterface::SendFile(u8 type, const char * fileName)
{
	//step 1 check file exist
#ifdef NAVIPACK_WIN
	if (_access(fileName, 0) < 0)
#else
	if (access(fileName, 0) < 0)
#endif
	{
		LOGE("file not exist!");
		return -1;
	}

	MD5_CTX md5;
	char pmd5[32 + 1];
	pmd5[32] = '\0';

	//readFile
	FILE * pFile;
	u32 fileSize;
	size_t bytes;
	/* ��Ҫһ��byte��©�ض��������ļ���ֻ�ܲ��ö����Ʒ�ʽ�� */
	pFile = fopen(fileName, "rb");
	if (pFile == NULL)
	{
		LOGE("-->open File(%s) error", fileName);
		return -2;
	}

	/* ��ȡ�ļ���С */
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	rewind(pFile);

	/* �����ڴ�洢�����ļ� */
	int bufLen = T_BUF_SIZE + sizeof(SdkProtocolHeader) + sizeof(FileInfo);
	char* pBuffer = new char[bufLen];
	char* pBak = pBuffer;

	if (pBuffer == NULL)
	{
		LOGE("Memory error");
		fclose(pFile);
		return -3;
	}

	SdkProtocolHeader *head = (SdkProtocolHeader*)pBak;
	head->deviceAddr = DEVICE_MSG;
	head->functionCode = type;

	FileInfo *fileInfo = (FileInfo*)(pBak + sizeof(SdkProtocolHeader));
	memset(fileInfo->fileName, 0, sizeof(fileInfo->fileName));
	memset(fileInfo->md5, 0, sizeof(fileInfo->md5));
	fileInfo->fileLen = fileSize;

	int nameLen = strlen(fileName) - 1;
	while (nameLen > 0 && fileName[nameLen] != divideChar)
	{
		nameLen--;
	}
	char packName[128];
	memset(packName, 0, sizeof(packName));
	if (nameLen > 0) {
		nameLen++;
	}
	memcpy(packName, fileName + nameLen, strlen(fileName) - nameLen);
	memcpy(fileInfo->fileName, packName, strlen(packName));


	int partNum = 1;
	while (1)
	{
		bytes = fread(pBak + sizeof(SdkProtocolHeader) + sizeof(FileInfo), 1, T_BUF_SIZE, pFile);
		if (bytes > 0)
		{
			fileInfo->partNum = partNum++;
			fileInfo->partLen = bytes;
			md5.GetBufferMd5(pmd5, pBak + sizeof(SdkProtocolHeader) + sizeof(FileInfo), bytes);
			memcpy(fileInfo->md5, pmd5, sizeof(fileInfo->md5));
			//LOGE("%d,%d UpdateNaviPack packName--> %s md5:%s", bytes, fileInfo->partLen, packName, pmd5);
			if (isConnect)
			{
				int n = mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)pBak, sizeof(SdkProtocolHeader) + sizeof(FileInfo) + bytes);
				if (n != sizeof(SdkProtocolHeader) + sizeof(FileInfo) + bytes)
				{
					//LOGE("-------------->mHwInterface->WriteData = %d needWrite = %d  partNum=%d",n, sizeof(SdkProtocolHeader) + sizeof(FileInfo) + bytes, fileInfo->partNum);
				}
			}
		}
		if (bytes < T_BUF_SIZE) //complete
		{
			//LOGE("bytes < T_BUF_SIZE write over");
			fclose(pFile);
			pFile = NULL;
			break;
		}
		Sleep(300);

	}

	LOGD("writeData success %d!", bufLen);
	delete[] pBuffer;
	return 0;
}
//��������
int CNavipackInterface::UpdateNaviPackFile(const char * fileName)
{
	return SendFile(SET_UPDATE_FILE,fileName);
}

int CNavipackInterface::SetSelfStream(char* buf, int bufLen)
{

	int data_len = sizeof(SdkProtocolHeader) + bufLen;
	u8 *buffer = new u8[sizeof(SdkProtocolHeader) + bufLen];
	SdkProtocolHeader *head = (SdkProtocolHeader*)buffer;
	head->deviceAddr = MCU_ADDRESS;
	head->functionCode = MCU_USER_REG_WRITE;
	
	memcpy(&buffer[sizeof(SdkProtocolHeader)], buf, bufLen);
	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buffer, data_len);
	}
	delete[] buffer;
	return 0;
}

int CNavipackInterface::SetSaveMap(const char* filePath, const char* fileName)
{
	LOGD("CNavipackInterface::SetSaveMap path(%s):name(%s)",filePath,fileName);

	char file[FILE_PATH_MAX_LEN];
	if (strlen(filePath) + strlen(fileName) + sizeof(char) >= sizeof(file))
	{
		LOGD("Path Name too long");
		return -2;
	}

	memset(file,0,sizeof(file));

	int data_len = sizeof(SdkProtocolHeader)+sizeof(file);
	int ret = -1;

	u8 *buffer = new u8[data_len];
	SdkProtocolHeader *head = (SdkProtocolHeader*)buffer;
	head->deviceAddr = DEVICE_MSG;
	head->functionCode = SET_NAVAPACK_UPLOAD_MAP;

	sprintf(file, "%s/%s", filePath, fileName);

	memcpy(&buffer[sizeof(SdkProtocolHeader)], file, sizeof(file));

	LOGD("SetSaveMap file(%s) name(%s)", file,&buffer[sizeof(SdkProtocolHeader)]);

	if (isConnect) {
		ret = mHwInterface->WriteData(PT_SERAL_PACKAGE, buffer, data_len);
	}

	return ret;
	

}

int CNavipackInterface::SendFile(int type, const char* filePath, const char* fileName)
{
	std::string file(filePath);
#ifdef NAVIPACK_WIN
	file = file + '\\' + fileName;
#else
	file = file + '/'+ fileName;
#endif
	LOGI("SendFile %s", file.c_str());
	return SendFile((u8)type, file.c_str());
}

int CNavipackInterface::SetWiFiParam(const  char * ssid, const  char * password)
{
	uint8_t* buf = new uint8_t[sizeof(SdkProtocolHeader)+sizeof(WifiParam)];
	WifiParam* wifiParam = (WifiParam*)&buf[sizeof(SdkProtocolHeader)];
	if (strlen(ssid) > sizeof(wifiParam->ssid) || strlen(password) > sizeof(wifiParam->pwd))
	{
		delete[] buf;
		return -1;
	}

	memset(wifiParam->ssid, 0, sizeof(wifiParam->ssid));
	memcpy(wifiParam->ssid,ssid,strlen(ssid));
	memset(wifiParam->pwd,0,sizeof(wifiParam->pwd));
	memcpy(wifiParam->pwd, password, strlen(password));

	LOGD("SetWiFiParam %s %s", wifiParam->ssid, wifiParam->pwd);

	SdkProtocolHeader* head = (SdkProtocolHeader*)buf;
	head->deviceAddr = DEVICE_MSG;
	head->functionCode = SET_WIFI_PARAM;

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, sizeof(SdkProtocolHeader) + sizeof(WifiParam));
	}

	delete[] buf;
	return 0;
}

int CNavipackInterface::SetChangeNaviPackMode(int mode)
{
	SdkProtocolHeader header;
	header.deviceAddr = DEVICE_MSG;
	if (mode == MODE_NAVIGATION) {
		header.functionCode = CHANGE_LIDAR_TO_PACK_MODE;		//Ĭ��ģʽ
	}
	else
	{
		header.functionCode = CHANGE_LIDAR_TO_SENSOR_MODE;		//�����״�����ģʽ
	}
	
	if (isConnect) {
		return mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	}
	return -1;
}

int CNavipackInterface::ImuCalibrate()
{
	SdkProtocolHeader header;
	header.deviceAddr = ALG_ADDRESS;
	header.functionCode = ALG_CONTROL_REG;
	header.startAddr = ALG_CR_ADDR_IMU_CELIBRATE;
	header.len = 0;
	if (isConnect) {
		return mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	}
	return -1;
}


int CNavipackInterface::UpdateLidarFirmware()
{
	SdkProtocolHeader header;
	header.deviceAddr = ALG_ADDRESS;
	header.functionCode = ALG_CONTROL_REG;
	header.startAddr = ALG_CR_ADDR_LIDAR_FIRMWARE_UPDATE;
	header.len = 0;
	if (isConnect) {
		return mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	}
	return -1;
}

int CNavipackInterface::DoCleanTask()
{
	SdkProtocolHeader header;
	header.deviceAddr = ALG_ADDRESS;
	header.functionCode = ALG_CONTROL_REG;
	header.startAddr = ALG_CR_ADDR_CLEAN;
	header.len = 0;
	if (isConnect) {
		return mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&header, sizeof(SdkProtocolHeader));
	}
	return -1;
}

int CNavipackInterface::OptimizeMap(bool enable_flag)
{
	int nEnable = (enable_flag == true ? 1 : 0);
	SdkProtocolHeader head;
	int data_len = 0;
	u8 *buf = new u8[sizeof(SdkProtocolHeader) + sizeof(u8)];
	head.deviceAddr = ALG_ADDRESS;
	head.functionCode = ALG_ENABLE_OPTIMIZE;
	memcpy(buf, (u8*)&head, sizeof(SdkProtocolHeader)); data_len += sizeof(SdkProtocolHeader);
	memcpy(buf + data_len, (u8*)&nEnable, sizeof(u8)); data_len += sizeof(u8);

	if (isConnect) {
		mHwInterface->WriteData(PT_SERAL_PACKAGE, buf, data_len);
	}
	delete[] buf;
	return -1;
}

int CNavipackInterface::SendUnifiedSensor(int id, UnifiedSensorInfo sensorData)
{
	UnifiedSensor_S data;
	data.header.deviceAddr = ALG_ADDRESS;
	data.header.functionCode = ALG_SET_UNIFIED_SENSOR_DATA;
	data.header.startAddr = 0;
	data.header.len = sizeof(UnifiedSensorInfo);
	memcpy(&data.sensorInfo,&sensorData,sizeof(UnifiedSensorInfo));
	if (isConnect) {
		return mHwInterface->WriteData(PT_SERAL_PACKAGE, (uint8_t*)&data, data.header.len+sizeof(SdkProtocolHeader));
	}
	return -1;
}

void CNavipackInterface::GetAlgMapDataFromRxDataLz4(AlgMapData* mapData, const uint8_t* data, uint32_t len)
{
	memcpy((u8*)(mapData), &data[sizeof(SdkProtocolHeader)], (sizeof(AlgMapData) - sizeof(mapData->map)));
	int encDataLen = len - (sizeof(AlgMapData) + sizeof(SdkProtocolHeader));
	int decLen = LZ4_decompress_safe((char *)&data[sizeof(SdkProtocolHeader) + sizeof(AlgMapData)], (char *)mapData->map, encDataLen, MAX_MAP_SIZE);
//	if (deccLen == w*h)
//	{
//		return true;
//	}
//	return false;
	//int decLen = RunLenghDecoding(&data[sizeof(SdkProtocolHeader) + sizeof(AlgMapData)], encDataLen, mapData->map, MAX_MAP_SIZE);
	//LOG("%d RunLenghDecoding IN(%d) OUT(%d)", sizeof(AlgMapData), encDataLen, decLen);
}

void CNavipackInterface::GetAlgMapDataFromRxData(AlgMapData* mapData,const uint8_t* data, uint32_t len)
{
	memcpy((u8*)(mapData), &data[sizeof(SdkProtocolHeader)], (sizeof(AlgMapData) - sizeof(mapData->map)));
	//PrintBuf(data, sizeof(SdkProtocolHeader) + sizeof(AlgMapData), "algMapData:");
	int encDataLen = len - (sizeof(AlgMapData) + sizeof(SdkProtocolHeader));
	int decLen = RunLenghDecoding(&data[sizeof(SdkProtocolHeader) + sizeof(AlgMapData)], encDataLen, mapData->map, MAX_MAP_SIZE);
	//LOG("%d RunLenghDecoding IN(%d) OUT(%d)", sizeof(AlgMapData), encDataLen, decLen);
	//check...
}

void CNavipackInterface::RxDataCallBack(int32_t id, void *param, const uint8_t *data, int32_t len)
{

	CNavipackInterface * face = (CNavipackInterface *)param;
	if (sizeof(SdkProtocolHeader) <= len)
	{
		SdkProtocolHeader *header = (SdkProtocolHeader *)data;
		if (header->deviceAddr == ALG_ADDRESS)
		{
			switch (header->functionCode)
			{
			case ALG_DATA_READ:
			{

				switch (header->startAddr)
				{
				case ALG_DATA_ADDR_REAL_LIDAR_DATA:
				{
					face->lidar_cs.Enter();
					memcpy((u8*)face->mRealLidarData, (u8*)header + sizeof(SdkProtocolHeader), len - sizeof(SdkProtocolHeader));
					face->lidar_cs.Leave();
					if (face->mDeviceMsgCallBack) face->mDeviceMsgCallBack(id, UPDATE_SENSOR_DATA, ST_LIDAR2D, NULL);
					break;
				}
				case ALG_DATA_ADDR_LIDAR_MAP:
				{
					face->lidar_cs.Enter();
					face->GetAlgMapDataFromRxData(face->mLidarMap, data,len);
					if (face->mDeviceMsgCallBack) face->mDeviceMsgCallBack(id, UPDATE_MAP, LIDAR_MAP, NULL);
					face->lidar_cs.Leave();
					break;
				}
				case ALG_DATA_ADDR_LIDAR_MAP_LZ4:
				{
					face->lidar_cs.Enter();
					static long i = 0;
					face->GetAlgMapDataFromRxDataLz4(face->mLidarMap, data, len);
					if (face->mDeviceMsgCallBack) face->mDeviceMsgCallBack(id, UPDATE_MAP, LIDAR_MAP, NULL);
					face->lidar_cs.Leave();
					break;
				}
				case ALG_DATA_ADDR_PLANED_PATH:
				{
					face->Path_cs.Enter();
					memcpy(face->mPlanedPathPos, (u8*)header + sizeof(SdkProtocolHeader), sizeof(AlgTargetPos));
					face->Path_cs.Leave();
					if (face->mDeviceMsgCallBack) face->mDeviceMsgCallBack(id, NAVIGATION_STATUS, PATH_UPGRADE, NULL);
					break;
				}
				case ALG_DATA_ADDR_MAP:
				{
					face->Map_cs.Enter();
					//memcpy((u8*)face->mMap, (u8*)header + sizeof(SdkProtocolHeader), len - sizeof(SdkProtocolHeader));
					face->GetAlgMapDataFromRxData(face->mMap, data, len);
					face->Map_cs.Leave();
					break;
				}
				case ALG_DATA_ADDR_STATUS_R:
				{
					if (face->mDeviceMsgCallBack) face->mDeviceMsgCallBack(id, UPDATE_ALG_ATATUS_REG, 0, NULL);
					face->StatusRegister_cs.Enter();
					memcpy((u8*)face->mStatusRegister, (u8*)header + sizeof(SdkProtocolHeader), len - sizeof(SdkProtocolHeader));
					face->StatusRegister_cs.Leave();
					break;
				}
				case ALG_DATA_ADDR_LIDAR_RAW_DATA:		//�õ����״�ԭʼ����
				{
					LOGD("get lidar raw data %d", len);
					break;
				}

				default:
					break;
				}
				break;
			}
			default:
				break;
			}
		}
		else if (header->deviceAddr == DEVICE_MSG)
		{
			if (header->functionCode == SET_NAVAPACK_UPLOAD_MAP)
			{
				FileInfo* fileInfo = (FileInfo*)(data + sizeof(SdkProtocolHeader));
				if (face->mMapPackageCallBack) {
					int checkedOk = 0;
					MD5_CTX md5;
					char pmd5[32 + 1];
					char fileInfoMd5[32 + 1];
					pmd5[32] = 0;
					memcpy(fileInfoMd5, fileInfo->md5, sizeof(pmd5));
					fileInfoMd5[32] = 0;
					md5.GetBufferMd5(pmd5, (char*)(data + sizeof(SdkProtocolHeader) + sizeof(FileInfo)), fileInfo->partLen);
				//	LOGD(" %d md5 cmp \n\t%s \n\t%s", fileInfo->partLen, fileInfoMd5, pmd5);
					if (memcmp(pmd5, fileInfo->md5, sizeof(fileInfo->md5)) == 0)//У��ɹ�
					{
						checkedOk = 1;
					}
					else
					{
						LOGE("file transfrom error!! check md5 failed!");
					}
					face->mMapPackageCallBack(id, fileInfo, checkedOk,data + sizeof(SdkProtocolHeader) + sizeof(FileInfo), fileInfo->partLen);
				}	
			}
			else if (header->functionCode == UPDATE_MAP_LIST)//���µ�ͼ�б�
			{
				memcpy(&(face->mMapListInfo),&data[sizeof(SdkProtocolHeader)],sizeof(MapListInfo));
				if (face->mDeviceMsgCallBack) 
					face->mDeviceMsgCallBack(id, header->functionCode, face->mMapListInfo.mapListNum, NULL);
			}
			else {
				if (face->mDeviceMsgCallBack) 
					face->mDeviceMsgCallBack(id, header->functionCode, header->len, NULL);
			}
		
		}
		else if (header->deviceAddr == ERROR_MSG)
		{
			ErrorInfo *errInfo = (ErrorInfo *)(&data[sizeof(SdkProtocolHeader)]);
			if (face->mErrorMsgCallBack) face->mErrorMsgCallBack(id, errInfo->errorLevel, errInfo->errorCode, (char*)errInfo->info);
		}
		else if (header->deviceAddr == LIDAR_ADDRESS)
		{
			if (face->mLidarPackageCb) {
				face->mLidarPackageCb(id,data,len);
			}
		}
	}
}


