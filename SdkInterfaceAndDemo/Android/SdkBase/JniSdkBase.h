#ifndef _JNI_SDK_BASE2_H_
#define _JNI_SDK_BASE2_H_

#ifndef NAVIPACK_WIN

#include <jni.h>
#include <android/log.h>

#define NAVIPACK_CLASS_NAME "com/imscv/navipacksdk/NaviPackSdk"

#define NAVIPACK_PARAM_CLASS_NAME "com/imscv/navipacksdk/data/NaviPackParam"
#define CARRIER_PARAM_CLASS_NAME "com/imscv/navipacksdk/data/CarrierParam"
#define ALG_SENSOR_DATA_CLASS_NAME "com/imscv/navipacksdk/data/AlgSensorData"

#define ALG_MAP_DATA_CLASS_NAME "com/imscv/navipacksdk/data/AlgMapData"

#define ALG_CTRL_REG_CLASS_NAME "com/imscv/navipacksdk/regparam/AlgCtrlReg"
#define ALG_STATUS_REG_CLASS_NAME "com/imscv/navipacksdk/regparam/AlgStatusReg"

#define CHASSIS_CTRL_REG_CLASS_NAME "com/imscv/navipacksdk/regparam/ChsCtrlReg"
#define CHASSIS_STATUS_REG_CLASS_NAME "com/imscv/navipacksdk/regparam/ChsStatusReg"
#define CHASSIS_PARAM_REG_CLASS_NAME "com/imscv/navipacksdk/regparam/ChsParaReg"



typedef struct
{
	jmethodID mid;
	jfieldID lidarPosX;
}CarrierParamMethod;

typedef struct
{
	jmethodID mid;
	jfieldID maxLineVelocity;
}NaviPackParamMethod;

typedef struct{
	jmethodID mid;
	jfieldID sensorType;
	jfieldID num;
	jfieldID posX;
	jfieldID posY;
	jfieldID posSita;
	jfieldID localPosX;
	jfieldID localPosY;
}AlgSensorDataMethod;



//地图数据
typedef struct{
	jmethodID mid;
	jfieldID width;
	jfieldID height;
	jfieldID resolution;
	jfieldID x_min;
	jfieldID y_min;
	jfieldID map;
}AlgMapDataMethod;

//算法控制寄存器
typedef struct
{
	jmethodID mid;
	jfieldID vectorTargetDistance;
	jfieldID vectorTargetAngle;
	jfieldID relativeTargetPosX;
	jfieldID relativeTargetPosY;
	jfieldID setLineVelocity;
	jfieldID setAngularVelocity;
	jfieldID backuint8_tge;
	jfieldID backuint8_tgeThreshold;
	jfieldID setBackuint8_tgePosX;
	jfieldID setBackuint8_tgePosY;
	jfieldID startMapping;
	jfieldID stopMapping;
	jfieldID setDefaultMap;
	jfieldID emergencyStop;
}AlgCtrlRegMethod;

//算法状态寄存器
typedef struct
{
	jmethodID mid;
	jfieldID workMode;
	jfieldID lineVelocity;
	jfieldID angularVelocity;
	jfieldID posX;
	jfieldID posY;
	jfieldID posSita;
	jfieldID errorState;
}AlgStatusRegMethod;


//底盘控制寄存器
typedef struct
{
	jmethodID mid;
	jfieldID lineVelocity;
	jfieldID angularVelocity;
}ChsCtrlRegMethod;


//底盘状态寄存器
typedef struct
{
	jmethodID mid;
	jfieldID ultrasound;
	jfieldID dropSensor;
	jfieldID irSensor;
	jfieldID collisionSensor;
	jfieldID angularPos;
	jfieldID leftEncoderPos;                  //当前左边里程计的积分位置
	jfieldID rightEncoderPos;                  //当前右边里程计的积分位置
	jfieldID lineVelocity;
	jfieldID angularVelocity;
	jfieldID chargeStatus;
	jfieldID batteryStatus;
	jfieldID errorState;
}ChsStatusRegMethod;


//底盘参数寄存器
typedef struct
{
	jmethodID mid;
	jmethodID setRegData;

}ChsParaRegMethod;

//初始化sdk
int JniSdkInit(JNIEnv *env, jobject obj);
int JniGetSdkVersion(JNIEnv *env, jobject obj);
int JniSetGetNaviPackVersion(JNIEnv *env, jobject obj, jint id);
int JniCreate(JNIEnv *env, jobject obj, jint conType);
void JniDestory(JNIEnv *env, jobject obj,jint id);
int JniOpen(JNIEnv *env, jobject obj,jint id,jstring ip,jint port);
int JniReOpen(JNIEnv *env, jobject obj, jint id);
int JniClose(JNIEnv *env, jobject obj, jint id);
int JniGetCarrierParam(JNIEnv *env, jobject obj, jint id,jobject o);
int JniSetCarrierParam(JNIEnv *env, jobject obj, jint id, jobject o);
//jobject JniGetAlgMapData(JNIEnv *env, jobject obj,jint id,jint type);

//------------------->>>>>>>>>>>>>>>>>>>>
jint JniGetNaviPackParam(JNIEnv *env, jobject obj, jint id,jobject naviPackPara);
jint JniSetNaviPackParam(JNIEnv *env, jobject obj, jint id, jobject naviPackPara);
jint JniSetTargets(JNIEnv *env, jobject obj, jint id,jintArray posX, jintArray posY, jint num, jint phi);
jint JniGetCurrentPath(JNIEnv *env, jobject obj, jint id,jintArray posX, jintArray posY);
jint JniSetSpeed(JNIEnv *env, jobject obj, jint id, jint v,jint w);
jint JniAutoCharge(JNIEnv *env, jobject obj, jint id);
jint JniSetChargerPosition(JNIEnv *env, jobject obj, jint id,jint posX,jint posY);
jint JniStartMapping(JNIEnv *env, jobject obj, jint id, jint mappingMode);
jint JniStopMapping(JNIEnv *env, jobject obj, jint id, jint save_flag);
jint JniGetMapList(JNIEnv *env, jobject obj, jint id, jintArray id_buffer);
jint JniSaveCurrentMapToList(JNIEnv *env, jobject obj, jint id, jint mapId);
jint JniLoadMap(JNIEnv *env, jobject obj, jint id, jint map_id);
jint JniSetGetCurrentMap(JNIEnv *env, jobject obj, jint id);
jint JniLoadLocalMap(JNIEnv *env, jobject obj, jint id, jstring local_map_path, jint map_id);
jint JniGetMapLayer(JNIEnv *env, jobject obj, jint id, jobject map_data, jint map_type);
jint JniSetMapLayer(JNIEnv *env, jobject obj, jint id, jobject map_data, jint map_type);
jint JniGetSensorData(JNIEnv *env, jobject obj, jint id, jobject sensor_data, jint sensorType);
jint JniGetStatus(JNIEnv *env, jobject obj, jint id, jobject status);
jint JniInitLocation(JNIEnv *env, jobject obj, jint id);
jint JniSetWiFiParam(JNIEnv *env, jobject obj, jint id, jstring ssid, jstring password);
jint JniSearchDevice(JNIEnv *env, jobject obj, jint id, jstring device_list, jint timeout);
jint JniCheckConnection(JNIEnv *env, jobject obj);
jint JniUpdateNaviPackFile(JNIEnv *env, jobject obj,jint id,jstring fileName);
jint JniSetSelfStream(JNIEnv *env, jobject obj,jint id,jbyteArray stream);
jint JniSetSaveMap(JNIEnv *env, jobject obj, jint id,jstring filePath, jstring fileName);
jint JniSendFile(JNIEnv *env, jobject obj, jint id, jint type, jstring filePath, jstring fileName);
jint JniSetChangeNaviPackMode(JNIEnv *env, jobject obj, jint id, jint mode);
jint JniImuCalibrate(JNIEnv *env, jobject obj, jint id);
//jobject JniGetAlgMapData(JNIEnv *env, jobject obj, jint id, jint type);

#endif
#endif