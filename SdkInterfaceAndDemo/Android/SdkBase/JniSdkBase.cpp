#ifndef NAVIPACK_WIN
#include "JniSdkBase.h"
#include "tools/LogFile.h"
#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include <assert.h>

#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include "tools/SDKProtocol.h"
#include "../../NaviPackSDK.h"
#include "errordeal/ErrorEvent.h"

#include <stddef.h>

#include<signal.h>
JavaVM * g_javaVM;
jobject g_javaObj;
jmethodID g_onRecvMsg;
jmethodID g_onRecvErrorMsg;
jmethodID g_onRecvFileBuffer;


static NaviPackParamMethod g_naviPackParamMethods;
static CarrierParamMethod g_carrierParamMethods;
static AlgSensorDataMethod g_algSensorDataMethods;

static AlgCtrlRegMethod g_algCtrlRegMethods;
static AlgStatusRegMethod g_algStatusRegMethods;
static AlgMapDataMethod g_algMapDataMethods;

static ChsCtrlRegMethod g_chasCtrlRegMethods;
static ChsParaRegMethod g_chasParaRegMethods;
static ChsStatusRegMethod g_chasStatusMethods;


static jobject g_naviPackParamObj = NULL;
static jobject g_carrierParamObj = NULL;
static jobject g_algSensorDataObj = NULL;

static jobject g_algStatusRegObj = NULL;
static jobject g_algCtrlRegObj = NULL;
static jobject g_algLadarDataObj = NULL;
static jobject g_algMapDataObj = NULL;

static jobject g_chsCtrlRegObj = NULL;
static jobject g_chsStatusRegObj = NULL;
static jobject g_chsParamRegObj = NULL;



static void InitCarrierParamMethods(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(CARRIER_PARAM_CLASS_NAME);
	g_carrierParamMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_carrierParamMethods.lidarPosX = env->GetFieldID(javaClass, "lidarPosX", "I");

	jobject carrierParamObj = env->NewObject(javaClass, g_carrierParamMethods.mid);
	g_carrierParamObj = env->NewGlobalRef(carrierParamObj);
}

static void InitNaviPackParamMethods(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(NAVIPACK_PARAM_CLASS_NAME);
	g_naviPackParamMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_naviPackParamMethods.maxLineVelocity = env->GetFieldID(javaClass, "maxLineVelocity", "I");

	jobject naviPackParamObj = env->NewObject(javaClass, g_naviPackParamMethods.mid);
	g_naviPackParamObj = env->NewGlobalRef(naviPackParamObj);
}

static void InitSensorDataMethods(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(ALG_SENSOR_DATA_CLASS_NAME);
	g_algSensorDataMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_algSensorDataMethods.sensorType = env->GetFieldID(javaClass, "sensorType", "B");
	g_algSensorDataMethods.num = env->GetFieldID(javaClass, "num", "I");
	g_algSensorDataMethods.posX = env->GetFieldID(javaClass, "posX", "I");
	g_algSensorDataMethods.posY = env->GetFieldID(javaClass, "posY", "I");
	g_algSensorDataMethods.posSita = env->GetFieldID(javaClass, "posSita", "I");
	g_algSensorDataMethods.localPosX = env->GetFieldID(javaClass, "localPosX", "[I");
	g_algSensorDataMethods.localPosY = env->GetFieldID(javaClass, "localPosY", "[I");


	jobject algSensorDataObj = env->NewObject(javaClass, g_algSensorDataMethods.mid);
	g_algSensorDataObj = env->NewGlobalRef(algSensorDataObj);
}

static void InitChasParaRegisterMethods(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(CHASSIS_PARAM_REG_CLASS_NAME);
	g_chasParaRegMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_chasParaRegMethods.setRegData = env->GetMethodID(javaClass, "setRegData", "([S[S[S[S)V");

	jobject chsParamRegObj = env->NewObject(javaClass, g_chasParaRegMethods.mid);
	g_chsParamRegObj = env->NewGlobalRef(chsParamRegObj);

}

static void InitChasStatusRegisterMethods(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(CHASSIS_STATUS_REG_CLASS_NAME);
	g_chasStatusMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_chasStatusMethods.ultrasound = env->GetFieldID(javaClass, "ultrasound", "[S");
	g_chasStatusMethods.dropSensor = env->GetFieldID(javaClass, "dropSensor", "B");
	g_chasStatusMethods.irSensor = env->GetFieldID(javaClass, "irSensor", "S");
	g_chasStatusMethods.collisionSensor = env->GetFieldID(javaClass, "collisionSensor", "B");
	g_chasStatusMethods.angularPos = env->GetFieldID(javaClass, "angularPos", "S");
	g_chasStatusMethods.leftEncoderPos = env->GetFieldID(javaClass, "leftEncoderPos", "I");
	g_chasStatusMethods.rightEncoderPos = env->GetFieldID(javaClass, "rightEncoderPos", "I");
	g_chasStatusMethods.lineVelocity = env->GetFieldID(javaClass, "lineVelocity", "I");
	g_chasStatusMethods.angularVelocity = env->GetFieldID(javaClass, "angularVelocity", "I");
	g_chasStatusMethods.chargeStatus = env->GetFieldID(javaClass, "chargeStatus", "B");
	g_chasStatusMethods.batteryStatus = env->GetFieldID(javaClass, "batteryStatus", "B");
	g_chasStatusMethods.errorState = env->GetFieldID(javaClass, "errorState", "S");

	jobject chsStatusRegObj = env->NewObject(javaClass, g_chasStatusMethods.mid, NULL);
	g_chsStatusRegObj = env->NewGlobalRef(chsStatusRegObj);

}

static void InitChasCtrlRegisterMethods(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(CHASSIS_CTRL_REG_CLASS_NAME);
	g_chasCtrlRegMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_chasCtrlRegMethods.lineVelocity = env->GetFieldID(javaClass, "lineVelocity", "I");
	g_chasCtrlRegMethods.angularVelocity = env->GetFieldID(javaClass, "angularVelocity", "I");

	jobject chsCtrlRegObj = env->NewObject(javaClass, g_chasCtrlRegMethods.mid, NULL);
	g_chsCtrlRegObj = env->NewGlobalRef(chsCtrlRegObj);
}

static void InitAlgCtrlRegMethods(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(ALG_CTRL_REG_CLASS_NAME);
	g_algCtrlRegMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_algCtrlRegMethods.vectorTargetDistance = env->GetFieldID(javaClass, "vectorTargetDistance", "I");
	g_algCtrlRegMethods.vectorTargetAngle = env->GetFieldID(javaClass, "vectorTargetAngle", "I");
	g_algCtrlRegMethods.relativeTargetPosX = env->GetFieldID(javaClass, "relativeTargetPosX", "F");
	g_algCtrlRegMethods.relativeTargetPosY = env->GetFieldID(javaClass, "relativeTargetPosY", "F");
	g_algCtrlRegMethods.setLineVelocity = env->GetFieldID(javaClass, "setLineVelocity", "I");
	g_algCtrlRegMethods.setAngularVelocity = env->GetFieldID(javaClass, "setAngularVelocity", "I");
	g_algCtrlRegMethods.backuint8_tge = env->GetFieldID(javaClass, "backuint8_tge", "B");
	g_algCtrlRegMethods.backuint8_tgeThreshold = env->GetFieldID(javaClass, "backuint8_tgeThreshold", "B");
	g_algCtrlRegMethods.setBackuint8_tgePosX = env->GetFieldID(javaClass, "setBackuint8_tgePosX", "I");
	g_algCtrlRegMethods.setBackuint8_tgePosY = env->GetFieldID(javaClass, "setBackuint8_tgePosY", "I");
	g_algCtrlRegMethods.startMapping = env->GetFieldID(javaClass, "startMapping", "B");
	g_algCtrlRegMethods.stopMapping = env->GetFieldID(javaClass, "stopMapping", "B");
	g_algCtrlRegMethods.setDefaultMap = env->GetFieldID(javaClass, "setDefaultMap", "B");
	g_algCtrlRegMethods.emergencyStop = env->GetFieldID(javaClass, "emergencyStop", "B");

	jobject algCtrlRegObj = env->NewObject(javaClass, g_algCtrlRegMethods.mid, NULL);
	g_algCtrlRegObj = env->NewGlobalRef(algCtrlRegObj);
}

static void InitAlgStatusRegMethods(JNIEnv *env, jobject obj)
{

	jclass javaClass = env->FindClass(ALG_STATUS_REG_CLASS_NAME);
	g_algStatusRegMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_algStatusRegMethods.workMode = env->GetFieldID(javaClass, "workMode", "B");
	g_algStatusRegMethods.lineVelocity = env->GetFieldID(javaClass, "lineVelocity", "I");
	g_algStatusRegMethods.angularVelocity = env->GetFieldID(javaClass, "angularVelocity", "I");
	g_algStatusRegMethods.posX = env->GetFieldID(javaClass, "posX", "I");
	g_algStatusRegMethods.posY = env->GetFieldID(javaClass, "posY", "I");
	g_algStatusRegMethods.posSita = env->GetFieldID(javaClass, "posSita", "I");
	g_algStatusRegMethods.errorState = env->GetFieldID(javaClass, "errorState", "S");

	jobject algStatusObj = env->NewObject(javaClass, g_algStatusRegMethods.mid, NULL);
	g_algStatusRegObj = env->NewGlobalRef(algStatusObj);
}

static void InitAlgMapData(JNIEnv *env, jobject obj)
{
	jclass javaClass = env->FindClass(ALG_MAP_DATA_CLASS_NAME);
	g_algMapDataMethods.mid = env->GetMethodID(javaClass, "<init>", "()V");
	g_algMapDataMethods.width = env->GetFieldID(javaClass, "width", "I");
	g_algMapDataMethods.height = env->GetFieldID(javaClass, "height", "I");
	g_algMapDataMethods.resolution = env->GetFieldID(javaClass, "resolution", "F");
	g_algMapDataMethods.x_min = env->GetFieldID(javaClass, "x_min", "F");
	g_algMapDataMethods.y_min = env->GetFieldID(javaClass, "y_min", "F");
	g_algMapDataMethods.map = env->GetFieldID(javaClass, "map", "[B");

	jobject algMapDataObj = env->NewObject(javaClass, g_algMapDataMethods.mid, NULL);
	g_algMapDataObj = env->NewGlobalRef(algMapDataObj);
}

static jobject GetAlgStatusRegValue(JNIEnv * env, char* data, jint start_addr, jint len)
{
	char* data_t = (char*)data;

	if (start_addr + len > sizeof(AlgStatusRegister))
	{
		LOGE("GetAlgStatusRegValue len is error, start = %d , len = %d , acture Need = %d ", start_addr, len, (int)sizeof(AlgStatusRegister));
		return NULL;
	}

	char buf[sizeof(AlgStatusRegister)];
	memcpy(buf + start_addr, data_t, len);
	AlgStatusRegister *statusReg = (AlgStatusRegister *)buf;

	env->SetByteField(g_algStatusRegObj, g_algStatusRegMethods.workMode, statusReg->workMode);
	env->SetIntField(g_algStatusRegObj, g_algStatusRegMethods.lineVelocity, statusReg->lineVelocity);
	env->SetIntField(g_algStatusRegObj, g_algStatusRegMethods.angularVelocity, statusReg->angularVelocity);
	env->SetIntField(g_algStatusRegObj, g_algStatusRegMethods.posX, statusReg->posX);
	env->SetIntField(g_algStatusRegObj, g_algStatusRegMethods.posY, statusReg->posY);
	env->SetIntField(g_algStatusRegObj, g_algStatusRegMethods.posSita, statusReg->posSita);
	env->SetShortField(g_algStatusRegObj, g_algStatusRegMethods.errorState, statusReg->errorState);

	return g_algStatusRegObj;
}

static jobject GetAlgCtrlRegValue(JNIEnv * env, char* data, jint start_addr, jint len)
{
	char* data_t = (char*)data;//获取到传来的数据


	if (start_addr + len > sizeof(AlgControlRegister))
	{
		LOGE("GetAlgCtrlRegValue len is error, start = %d , len = %d , acture Need = %d ", start_addr, len, (int)sizeof(AlgControlRegister));
		return NULL;
	}

	AlgControlRegister ctrlReg;

	char *p = (char*)&ctrlReg;
	memcpy(p + start_addr, data_t, len);

	env->SetIntField(g_algCtrlRegObj, g_algCtrlRegMethods.vectorTargetDistance, ctrlReg.vectorTargetDistance);
	env->SetIntField(g_algCtrlRegObj, g_algCtrlRegMethods.vectorTargetAngle, ctrlReg.vectorTargetAngle);
	env->SetFloatField(g_algCtrlRegObj, g_algCtrlRegMethods.relativeTargetPosX, ctrlReg.relativeTargetPosX);
	env->SetFloatField(g_algCtrlRegObj, g_algCtrlRegMethods.relativeTargetPosY, ctrlReg.relativeTargetPosY);
	env->SetIntField(g_algCtrlRegObj, g_algCtrlRegMethods.setLineVelocity, ctrlReg.setLineVelocity);
	env->SetIntField(g_algCtrlRegObj, g_algCtrlRegMethods.setAngularVelocity, ctrlReg.setAngularVelocity);
	env->SetByteField(g_algCtrlRegObj, g_algCtrlRegMethods.backuint8_tge, ctrlReg.backuint8_tge);
	env->SetByteField(g_algCtrlRegObj, g_algCtrlRegMethods.backuint8_tgeThreshold, ctrlReg.backuint8_tgeThreshold);
	env->SetIntField(g_algCtrlRegObj, g_algCtrlRegMethods.setBackuint8_tgePosX, ctrlReg.setBackuint8_tgePosX);
	env->SetIntField(g_algCtrlRegObj, g_algCtrlRegMethods.setBackuint8_tgePosY, ctrlReg.setBackuint8_tgePosY);
	env->SetByteField(g_algCtrlRegObj, g_algCtrlRegMethods.startMapping, ctrlReg.startMapping);
	env->SetByteField(g_algCtrlRegObj, g_algCtrlRegMethods.stopMapping, ctrlReg.stopMapping);
	env->SetByteField(g_algCtrlRegObj, g_algCtrlRegMethods.setDefaultMap, ctrlReg.setDefaultMap);
	env->SetByteField(g_algCtrlRegObj, g_algCtrlRegMethods.emergencyStop, ctrlReg.emergencyStop);

	return g_algCtrlRegObj;
}

static jintArray ByteArray2IntArray(JNIEnv * env, char* data, jint len)
{
	char* data_t = (char*)data;
	jintArray data_i = env->NewIntArray(len / 4);
	env->SetIntArrayRegion(data_i, 0, len / 4, (int*)data_t);
	return data_i;
}

//转换底盘控制寄存器
static jobject GetChsCtrlRegValue(JNIEnv * env, char* data, jint start_addr, jint len)
{
	char* data_t = (char*)data;//获取到传来的数据


	if (start_addr + len > sizeof(ChassisControlRegister))
	{
		LOGE("GetChsCtrlRegValue len is error, start = %d , len = %d , acture Need = %d ", start_addr, len, (int)sizeof(ChassisControlRegister));
		return NULL;
	}


	ChassisControlRegister chsCtrlReg_t;
	char *p = (char *)&chsCtrlReg_t;
	memcpy(p + start_addr, data_t, len);

	env->SetIntField(g_chsCtrlRegObj, g_chasCtrlRegMethods.lineVelocity, chsCtrlReg_t.lineVelocity);
	env->SetIntField(g_chsCtrlRegObj, g_chasCtrlRegMethods.angularVelocity, chsCtrlReg_t.angularVelocity);

	return g_chsCtrlRegObj;
}

//转换底盘状态寄存器
static jobject GetChsStatusRegValue(JNIEnv * env, char* data, jint start_addr, jint len)
{
	char* data_t = (char*)data;//获取到传来的数据

	if (start_addr + len > sizeof(ChassisStatusRegister))
	{
		LOGE("GetChsStatusRegValue len is error, start = %d , len = %d , acture Need = %d ", start_addr, len, (int)sizeof(ChassisStatusRegister));
		return NULL;
	}

	ChassisStatusRegister chsStatusReg_t;
	char *p = (char*)&chsStatusReg_t;
	memcpy(p + start_addr, data_t, len);

	jshortArray ultSoundArray = env->NewShortArray(sizeof(chsStatusReg_t.ultrasound) / sizeof(short));
	env->SetShortArrayRegion(ultSoundArray, 0, sizeof(chsStatusReg_t.ultrasound) / sizeof(short), (jshort*)chsStatusReg_t.ultrasound);
	env->SetObjectField(g_chsStatusRegObj, g_chasStatusMethods.ultrasound, ultSoundArray);
	env->DeleteLocalRef(ultSoundArray);

	env->SetByteField(g_chsStatusRegObj, g_chasStatusMethods.dropSensor, chsStatusReg_t.dropSensor);
	env->SetShortField(g_chsStatusRegObj, g_chasStatusMethods.irSensor, chsStatusReg_t.irSensor);
	env->SetByteField(g_chsStatusRegObj, g_chasStatusMethods.collisionSensor, chsStatusReg_t.collisionSensor);
	env->SetShortField(g_chsStatusRegObj, g_chasStatusMethods.angularPos, chsStatusReg_t.angularPos);
	env->SetIntField(g_chsStatusRegObj, g_chasStatusMethods.leftEncoderPos, chsStatusReg_t.leftEncoderPos);
	env->SetIntField(g_chsStatusRegObj, g_chasStatusMethods.rightEncoderPos, chsStatusReg_t.rightEncoderPos);
	env->SetIntField(g_chsStatusRegObj, g_chasStatusMethods.lineVelocity, chsStatusReg_t.lineVelocity);
	env->SetIntField(g_chsStatusRegObj, g_chasStatusMethods.angularVelocity, chsStatusReg_t.angularVelocity);
	env->SetByteField(g_chsStatusRegObj, g_chasStatusMethods.chargeStatus, chsStatusReg_t.chargeStatus);
	env->SetByteField(g_chsStatusRegObj, g_chasStatusMethods.batteryStatus, chsStatusReg_t.batteryStatus);
	env->SetShortField(g_chsStatusRegObj, g_chasStatusMethods.errorState, chsStatusReg_t.errorState);

	return g_chsStatusRegObj;
}

//转换底盘参数寄存器
static jobject GetChsParamRegValue(JNIEnv * env, char* data, jint start_addr, jint len)
{
	char* data_t = data;//获取到传来的数据

	if (start_addr + len > sizeof(ChassisParamRegister))
	{
		LOGE("GetChsParamRegValue len is error, start = %d , len = %d , acture Need = %d ", start_addr, len, (int)sizeof(ChassisParamRegister));
		return NULL;
	}

	ChassisParamRegister chsParamReg_t;
	char *p = (char*)&chsParamReg_t;
	memcpy(p + start_addr, data_t, len);

	short *q = (short*)p;

	jshortArray ultSoundArray = env->NewShortArray(16);
	env->SetShortArrayRegion(ultSoundArray, 0, 16, q);

	jshortArray dropSensorArray = env->NewShortArray(16);
	env->SetShortArrayRegion(dropSensorArray, 0, 16, q + 16);

	jshortArray irSensoArray = env->NewShortArray(32);
	env->SetShortArrayRegion(irSensoArray, 0, 32, q + 16 + 16);

	jshortArray collisionArray = env->NewShortArray(32);
	env->SetShortArrayRegion(collisionArray, 0, 32, q + 16 + 16 + 32);

	env->CallVoidMethod(g_chsParamRegObj, g_chasParaRegMethods.setRegData, ultSoundArray, dropSensorArray, irSensoArray, collisionArray);



	env->DeleteLocalRef(ultSoundArray);
	env->DeleteLocalRef(dropSensorArray);
	env->DeleteLocalRef(irSensoArray);
	env->DeleteLocalRef(collisionArray);
	
	return g_chsParamRegObj;
}


void DefSignalFun(int signo)
{
	LOGE("SIGPIPE ----> pipe broken\n"); //
}

void JniOnGetDeviceMsg(int id, int msgType, int msgCode, void* param)
{
	//LOGD("JniOnGetDeviceMsg %d", msgType);
	JNIEnv *env;
	int ret = g_javaVM->AttachCurrentThread(&env, NULL);
	if (ret != JNI_OK)
	{
		//("thread attach failed: %#x", ret);
		g_javaVM->DetachCurrentThread();
		return;
	}

	env->CallVoidMethod(g_javaObj, g_onRecvMsg, 0, msgType, msgCode, NULL);
	g_javaVM->DetachCurrentThread();
}

void JniOnGetErrorMsg(s32 id,s32 errorLevel, s32 errorCode,char* msg)
{
	LOGD("JniOnGetErrorMsg %s", msg);
	JNIEnv *env;
	int ret = g_javaVM->AttachCurrentThread(&env, NULL);
	if (ret != JNI_OK)
	{
		g_javaVM->DetachCurrentThread();
		return;
	}

	jbyte *by = (jbyte*)msg;
	int len = strlen(msg);
	jbyteArray jarray = env->NewByteArray(len);
	env->SetByteArrayRegion(jarray, 0, len, by);
	env->CallVoidMethod(g_javaObj, g_onRecvErrorMsg, 0, errorLevel, errorCode, jarray);
	env->DeleteLocalRef(jarray);

	g_javaVM->DetachCurrentThread();
}

void JniOnGetMapPackageMsg(s32 id, FileInfo* fileInfo, s32 checkedOk, const u8* data, u32 len)
{
	LOGD("JniOnGetMapPackageMsg --->>>checkedOk(%d) %d", checkedOk,len);
	JNIEnv *env;
	int ret = g_javaVM->AttachCurrentThread(&env, NULL);
	if (ret != JNI_OK)
	{
		g_javaVM->DetachCurrentThread();
		return;
	}

	jbyteArray jarrayName = env->NewByteArray(strlen(fileInfo->fileName));
	env->SetByteArrayRegion(jarrayName, 0, strlen(fileInfo->fileName), (const jbyte*)(fileInfo->fileName));
	jbyteArray jarrayBuf = env->NewByteArray(len);
	env->SetByteArrayRegion(jarrayBuf, 0, len, (const jbyte*)data);

	env->CallVoidMethod(g_javaObj, g_onRecvFileBuffer, id, jarrayName,fileInfo->partNum, jarrayBuf, checkedOk,fileInfo->fileLen);

	env->DeleteLocalRef(jarrayName);
	env->DeleteLocalRef(jarrayBuf);

	g_javaVM->DetachCurrentThread();

}

void JniOnGetLidarCallBack(s32 id, const u8 *buffer, s32 len)
{
	LOGD("JniOnGetLidarCallBack %d",len);
}

int JniSdkInit(JNIEnv *env, jobject obj)
{
	signal(SIGPIPE, DefSignalFun);

	env->GetJavaVM(&g_javaVM);

	jclass javaClass = env->FindClass(NAVIPACK_CLASS_NAME);
	jobject jobj = env->AllocObject(javaClass);
	g_javaObj = env->NewGlobalRef(jobj);

	InitAlgCtrlRegMethods(env, obj);
	InitAlgStatusRegMethods(env, obj);
	InitChasParaRegisterMethods(env, obj);
	InitChasStatusRegisterMethods(env, obj);
	InitChasCtrlRegisterMethods(env, obj);
	InitAlgMapData(env, obj);
	InitCarrierParamMethods(env, obj);
	InitNaviPackParamMethods(env, obj);
	InitSensorDataMethods(env,obj);

	g_onRecvMsg = env->GetMethodID(javaClass, "onRecvMsg", "(IIILjava/lang/Object;)V");
	g_onRecvErrorMsg = env->GetMethodID(javaClass, "onRecvErrorMsg", "(III[B)V");
	g_onRecvFileBuffer = env->GetMethodID(javaClass, "onRecvMapFileBuf", "(I[BI[BII)V");
	
	return 0;
}

int JniGetSdkVersion(JNIEnv *env, jobject obj)
{
	return GetSDKVersion();
}

int JniSetGetNaviPackVersion(JNIEnv *env, jobject obj,jint id)
{
	return SetGetNaviPackVersion(id);
}

int JniCreate(JNIEnv *env, jobject obj,jint conType)
{
	if (TCP_CONNECT == conType) 
	{
		return Create(TCP_CONNECT);
	}
	else if(SERIAL_CONNECT == conType)
	{
		return Create(SERIAL_CONNECT);
	}
	else
	{
		return -1;
	}
}
void JniDestory(JNIEnv *env, jobject obj, jint id)
{
	Destroy(id);
}

int JniGetCarrierParam(JNIEnv *env, jobject obj, jint id, jobject o)
{
	CarrierParam param;
	return GetCarrierParam(id, &param);
}

int JniSetCarrierParam(JNIEnv *env, jobject obj, jint id, jobject o)
{
	CarrierParam param;
	return SetCarrierParam(id, &param);
}



int JniOpen(JNIEnv *env, jobject obj, jint id, jstring fileName, jint port)
{
	jboolean iscopy;
	const char *ip_utf = env->GetStringUTFChars(fileName, &iscopy);
	LOGD("Opening tcp %s:%d", ip_utf,port);
	//      fd = open(path_utf, O_RDWR | O_DIRECT | O_SYNC);
	int ret = Open(id, ip_utf, port);
	LOGD("Opening tcp result %d", ret);
	env->ReleaseStringUTFChars(fileName, ip_utf);
	if (ret == 0)
	{
		SetCallback(id, JniOnGetDeviceMsg, JniOnGetErrorMsg, JniOnGetMapPackageMsg, JniOnGetLidarCallBack);
	}
	return ret;

}

int JniReOpen(JNIEnv *env, jobject obj, jint id)
{
	return Reopen(id);
}

int JniClose(JNIEnv *env, jobject obj, jint id)
{
	return Close(id);
}


static void SwitchNaviPackParamToJava(JNIEnv *env, jobject obj,NaviPackParam& param, jobject& jparam)
{
	env->SetIntField(jparam, g_naviPackParamMethods.maxLineVelocity, param.maxLineVelocity);
}

jint JniGetNaviPackParam(JNIEnv *env, jobject obj, jint id, jobject naviPackPara)
{
	NaviPackParam para;
	int ret = GetNaviPackParam(id, &para);
	SwitchNaviPackParamToJava(env, obj, para, naviPackPara);
	return ret;
}

static void SwitchNaviPackParamToCpp(JNIEnv *env, jobject obj, jobject& jparam, NaviPackParam& param)
{
	param.maxLineVelocity = env->GetIntField(jparam, g_naviPackParamMethods.maxLineVelocity);
}

jint JniSetNaviPackParam(JNIEnv *env, jobject obj, jint id, jobject naviPackPara)
{
	NaviPackParam param;
	SwitchNaviPackParamToCpp(env,obj,naviPackPara,param);
	int ret = SetNaviPackParam(id, &param);
	return ret;
}

jint JniSetTargets(JNIEnv *env, jobject obj, jint id, jintArray posX, jintArray posY, jint num,jint phi)
{
	int * pX = env->GetIntArrayElements(posX, 0);
	int * pY = env->GetIntArrayElements(posY, 0);
	int ret = SetTargets(id, pX, pY, num, phi);
	env->ReleaseIntArrayElements(posX, pX, 0);
	env->ReleaseIntArrayElements(posY, pY, 0);
	return ret;

}

jint JniGetCurrentPath(JNIEnv *env, jobject obj, jint id, jintArray posX, jintArray posY)
{
	int num = 0;
	int x_len = env->GetArrayLength(posX);
	int y_len = env->GetArrayLength(posY);
	int * px = new int[x_len];
	int * py = new int[y_len];
	int ret = GetCurrentPath(id, px, py, &num);
	if (num > 0)
	{
		env->SetIntArrayRegion(posX, 0, num, px);
		env->SetIntArrayRegion(posY, 0, num, py);
	}
	delete[] px;
	delete[] py;
	return num;

}
jint JniSetSpeed(JNIEnv *env, jobject obj, jint id, jint v, jint w)
{
	return SetSpeed(id, v, w);
}

jint JniAutoCharge(JNIEnv *env, jobject obj, jint id)
{
	return AutoCharge(id);
}

jint JniSetChargerPosition(JNIEnv *env, jobject obj, jint id, jint posX, jint posY)
{
	return SetChargerPosition(id,posX,posY);
}

jint JniStartMapping(JNIEnv *env, jobject obj, jint id, jint mappingMode)
{
	return StartMapping(id,mappingMode);
}

jint JniStopMapping(JNIEnv *env, jobject obj, jint id, jint save_flag)
{
	return StopMapping(id,save_flag);
}

jint JniGetMapList(JNIEnv *env, jobject obj, jint id, jintArray id_buffer)
{
	int idLen = env->GetArrayLength(id_buffer);
	int pId[MAX_MAP_LIST];
	int ret = GetMapList(id, pId);
	if (ret > 0 && ret <= idLen)
	{
		env->SetIntArrayRegion(id_buffer, 0, ret, pId);
	}
	return ret;
}

jint JniSaveCurrentMapToList(JNIEnv *env, jobject obj, jint id, jint mapId)
{
	return SaveCurrentMapToList(id,mapId);
}

jint JniLoadMap(JNIEnv *env, jobject obj, jint id, jint map_id)
{
	
	return LoadMap(id, map_id);
}

jint JniSetGetCurrentMap(JNIEnv *env, jobject obj, jint id)
{
	return SetGetCurrentMap(id);
}

jint JniLoadLocalMap(JNIEnv *env, jobject obj, jint id, jstring local_map_path, jint map_id)
{
	const char *filePath = env->GetStringUTFChars(local_map_path, 0);
	int ret = LoadLocalMap(id, filePath, map_id);
	env->ReleaseStringUTFChars(local_map_path, filePath);
	return ret;
}


static void SwitchAlgMapDataToCpp(JNIEnv *env, jobject obj, jobject& map_data, AlgMapData& map)
{
	map.width = env->GetIntField(g_algMapDataObj, g_algMapDataMethods.width);
	map.height = env->GetIntField(g_algMapDataObj, g_algMapDataMethods.height);
	map.resolution = env->GetFloatField(g_algMapDataObj, g_algMapDataMethods.resolution);
	map.x_min = env->GetFloatField(g_algMapDataObj, g_algMapDataMethods.x_min);
	map.y_min = env->GetFloatField(g_algMapDataObj, g_algMapDataMethods.y_min);

	jbyteArray mapArray = (jbyteArray) env->GetObjectField(g_algMapDataObj, g_algMapDataMethods.map);
	char* mapData = (char*)env->GetByteArrayElements(mapArray, 0);
	int mapLen = env->GetArrayLength(mapArray);
	memcpy(map.map, mapData, mapLen);
	env->ReleaseByteArrayElements(mapArray, (jbyte*)mapData, 0);
}


static void SwitchAlgMapDataToJava(JNIEnv *env, jobject obj, AlgMapData *map, jobject& map_data)
{

	env->SetIntField(map_data, g_algMapDataMethods.width, map->width);
	env->SetIntField(map_data, g_algMapDataMethods.height, map->height);
	env->SetFloatField(map_data, g_algMapDataMethods.resolution, map->resolution); 
	env->SetFloatField(map_data, g_algMapDataMethods.x_min, map->x_min);
	env->SetFloatField(map_data, g_algMapDataMethods.y_min, map->y_min);

	jbyteArray mapArray = env->NewByteArray(MAX_MAP_SIZE);
	env->SetByteArrayRegion(mapArray, 0, MAX_MAP_SIZE, (jbyte*)(map->map));
	env->SetObjectField(map_data, g_algMapDataMethods.map, mapArray);
	env->DeleteLocalRef(mapArray);
}

jint JniGetMapLayer(JNIEnv *env, jobject obj, jint id, jobject map_data, jint map_type)
{

	AlgMapData* algMapData = new AlgMapData;
	int ret = GetMapLayer(id, algMapData, map_type);
	LOGD("JniGetMapLayer width(%d) height(%d)", algMapData->width, algMapData->height);
	SwitchAlgMapDataToJava(env, obj, algMapData, map_data);
	delete algMapData;
	return ret;
}

jint JniSetMapLayer(JNIEnv *env, jobject obj, jint id, jobject map_data, jint map_type)
{
	AlgMapData algMapData;
	SwitchAlgMapDataToCpp(env, obj, map_data, algMapData);
	return SetMapLayer(id, &algMapData,map_type);
}

static void SwitchAlgSensorDataToJava(JNIEnv *env, jobject obj, AlgSensorData& sensorData, jobject& sensorDataObj)
{
	env->SetByteField(sensorDataObj, g_algSensorDataMethods.sensorType, sensorData.sensorType);
	env->SetIntField(sensorDataObj, g_algSensorDataMethods.num, sensorData.num);
	env->SetIntField(sensorDataObj, g_algSensorDataMethods.posX, sensorData.posX);
	env->SetIntField(sensorDataObj, g_algSensorDataMethods.posY, sensorData.posY);
	env->SetIntField(sensorDataObj, g_algSensorDataMethods.posSita, sensorData.posSita);

	jintArray localPosX = env->NewIntArray(LIDAR_RESOLUTION);
	env->SetIntArrayRegion(localPosX, 0, sensorData.num, sensorData.localPosX);
	env->SetObjectField(sensorDataObj, g_algSensorDataMethods.localPosX, localPosX);
	env->DeleteLocalRef(localPosX);

	jintArray localPosY = env->NewIntArray(LIDAR_RESOLUTION);
	env->SetIntArrayRegion(localPosY, 0, sensorData.num, sensorData.localPosY);
	env->SetObjectField(sensorDataObj, g_algSensorDataMethods.localPosY, localPosY);
	env->DeleteLocalRef(localPosY);
}

jint JniGetSensorData(JNIEnv *env, jobject obj, jint id, jobject sensor_data, jint sensorType)
{
	AlgSensorData *sensorData = new AlgSensorData;
	int ret = GetSensorData(id, sensorData, sensorType);
//	LOGD("GetSensorData %d %d %d",id,sensorType, sensorData->num);
	if (ret >= 0)
	{
		SwitchAlgSensorDataToJava(env,obj,*sensorData,sensor_data);
	}
	delete sensorData;
	return ret;
}

void SwitchAlgStatusRegisterToJava(JNIEnv *env, jobject obj, AlgStatusRegister& algStatusRegister, jobject& status)
{
	env->SetByteField(status, g_algStatusRegMethods.workMode, algStatusRegister.workMode);
	env->SetIntField(status, g_algStatusRegMethods.lineVelocity, algStatusRegister.lineVelocity);
	env->SetIntField(status, g_algStatusRegMethods.angularVelocity, algStatusRegister.angularVelocity);
	env->SetIntField(status, g_algStatusRegMethods.posX, algStatusRegister.posX);
	env->SetIntField(status, g_algStatusRegMethods.posY, algStatusRegister.posY);
	env->SetIntField(status, g_algStatusRegMethods.posSita, algStatusRegister.posSita);
	env->SetShortField(status, g_algStatusRegMethods.errorState, algStatusRegister.errorState);
}

jint JniGetStatus(JNIEnv *env, jobject obj, jint id, jobject status)
{
	AlgStatusRegister algStatusRegister;
	int ret = GetStatus(id, &algStatusRegister);
	if (ret >= 0)
	{
		SwitchAlgStatusRegisterToJava(env, obj, algStatusRegister, status);
	}
	return ret;
}

jint JniInitLocation(JNIEnv *env, jobject obj, jint id)
{
	return InitLocation(id);
}

jint JniSetWiFiParam(JNIEnv *env, jobject obj, jint id, jstring ssid, jstring password)
{
	const char* name = env->GetStringUTFChars(ssid, 0);
	const char* pwd = env->GetStringUTFChars(password, 0);
	int ret = SetWiFiParam(id, name, pwd);
	env->ReleaseStringUTFChars(ssid, name);
	env->ReleaseStringUTFChars(password, pwd);
	return ret;
}

jint JniSearchDevice(JNIEnv *env, jobject obj, jint id, jstring device_list, jint timeout)
{
	return -1;
}

jint JniCheckConnection(JNIEnv *env, jobject obj)
{
	return CheckConnection();
}
//jobject JniGetAlgMapData(JNIEnv *env, jobject obj, jint id, jint type);

jint JniUpdateNaviPackFile(JNIEnv *env, jobject obj, jint id,jstring fileName)
{
	const char* file = env->GetStringUTFChars(fileName, 0);
	int ret = UpdateNaviPackFile(id, file);
	env->ReleaseStringUTFChars(fileName, file);
	return ret;
}

jint JniSetSelfStream(JNIEnv *env, jobject obj, jint id, jbyteArray stream)
{
	jbyte* buf = env->GetByteArrayElements(stream,0);
	jsize len = env->GetArrayLength(stream);
	int ret = SetSelfStream(id, (char*)buf,len);
	env->ReleaseByteArrayElements(stream, buf, 0);
	return ret;

}

jint JniSetSaveMap(JNIEnv *env, jobject obj, jint id, jstring filePath, jstring fileName)
{
	const char* path = env->GetStringUTFChars(filePath, 0);
	const char* name = env->GetStringUTFChars(fileName, 0);
	LOGD("JniSetSaveMap %s/%s", path,name);
	int ret = -1;
	ret = SetSaveMap(id,path,name);
	env->ReleaseStringUTFChars(filePath, path);
	env->ReleaseStringUTFChars(fileName, name);

	return ret;
}

jint JniSendFile(JNIEnv *env, jobject obj, jint id, jint type,jstring filePath, jstring fileName)
{
	const char* path = env->GetStringUTFChars(filePath, 0);
	const char* name = env->GetStringUTFChars(fileName, 0);
	LOGD("JniSetSaveMap %s/%s", path, name);
	int ret = -1;
	ret = SendFile(id, type,path, name);
	env->ReleaseStringUTFChars(filePath, path);
	env->ReleaseStringUTFChars(fileName, name);

	return ret;
}

jint JniSetChangeNaviPackMode(JNIEnv *env, jobject obj, jint id, jint mode)
{
	return SetChangeNaviPackMode(id,mode);
}

jint JniImuCalibrate(JNIEnv *env, jobject obj, jint id)
{
	return ImuCalibrate(id);
}
#endif