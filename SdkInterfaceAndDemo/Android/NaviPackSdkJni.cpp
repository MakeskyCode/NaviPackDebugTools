#ifndef NAVIPACK_WIN
#include <jni.h>
#include "tools/LogFile.h"
#include "SdkBase/JniSdkBase.h"
#include <pthread.h>
#include <unistd.h>
//定义方法映射关系,把调用频率非常高的东西加进来,提升效率
static const JNINativeMethod gMethods[] =
{
	{ "native_init", "()I", (void*)JniSdkInit },
	{ "native_getSdkVersion", "()I", (void*)JniGetSdkVersion },
	{ "native_setGetNaviPackVersion", "(I)I", (void*)JniSetGetNaviPackVersion },
	{ "native_create", "(I)I", (void*)JniCreate },
	{ "native_destroy", "(I)V", (void*)JniDestory },
	{ "native_open", "(ILjava/lang/String;I)I", (void*)JniOpen },
	{ "native_reopen", "(I)I", (void*)JniReOpen },
	{ "native_close", "(I)I", (void*)JniClose },
	{ "native_getCarrierParam", "(ILcom/imscv/navipacksdk/data/CarrierParam;)I", (void*)JniGetCarrierParam },
	{ "native_setCarrierParam", "(ILcom/imscv/navipacksdk/data/CarrierParam;)I", (void*)JniSetCarrierParam },
	{ "native_getNaviPackParam", "(ILcom/imscv/navipacksdk/data/NaviPackParam;)I", (void*)JniGetNaviPackParam },
	{ "native_getNaviPackParam", "(ILcom/imscv/navipacksdk/data/NaviPackParam;)I", (void*)JniSetNaviPackParam },
	{ "native_setTargets", "(I[I[III)I", (void*)JniSetTargets },
	{ "native_getCurrentPath", "(I[I[I)I", (void*)JniGetCurrentPath },
	{ "native_setSpeed", "(III)I", (void*)JniSetSpeed },
	{ "native_autoCharge", "(I)I", (void*)JniAutoCharge },
	{ "native_setChargerPosition", "(III)I", (void*)JniSetChargerPosition },
	{ "native_startMapping", "(II)I", (void*)JniStartMapping },
	{ "native_stopMapping", "(II)I", (void*)JniStopMapping },
	{ "native_getMapList", "(I[I)I", (void*)JniGetMapList },
	{ "native_saveCurrentMapToMapList","(II)I",(void*) JniSaveCurrentMapToList},
	{ "native_loadMap", "(II)I", (void*)JniLoadMap },
	{ "native_setGetCurrentMap","(I)I",(void*)JniSetGetCurrentMap},
	{ "native_loadLocalMap", "(ILjava/lang/String;I)I", (void*)JniLoadLocalMap },
	{ "native_getMapLayer", "(ILcom/imscv/navipacksdk/data/AlgMapData;I)I", (void*)JniGetMapLayer },
	{ "native_setMapLayer", "(ILcom/imscv/navipacksdk/data/AlgMapData;I)I", (void*)JniSetMapLayer },
	{ "native_getSensorData", "(ILcom/imscv/navipacksdk/data/AlgSensorData;I)I", (void*)JniGetSensorData },
	{ "native_getStatus", "(ILcom/imscv/navipacksdk/regparam/AlgStatusReg;)I", (void*)JniGetStatus },
	{ "native_initLocation", "(I)I", (void*)JniInitLocation },
	{ "native_setWiFiParam", "(ILjava/lang/String;Ljava/lang/String;)I", (void*)JniSetWiFiParam },
	{ "native_searchDevice", "(ILjava/lang/String;I)I", (void*)JniSearchDevice },
	{ "native_checkConnection", "()I", (void*)JniCheckConnection },
	{ "native_updateNaviPackFile","(ILjava/lang/String;)I",(void*)JniUpdateNaviPackFile },
	{ "native_setSelfStream","(I[B)I", (void*)JniSetSelfStream},
	{ "native_setSaveMap","(ILjava/lang/String;Ljava/lang/String;)I",(void*)JniSetSaveMap },
	{ "native_sendFile","(IILjava/lang/String;Ljava/lang/String;)I",(void*)JniSendFile },
	{ "native_setChangeNaviPackMode","(II)I",(void*) JniSetChangeNaviPackMode},
	{ "native_imuCalibrate","(I)I",(void*)JniImuCalibrate},
	{ "native_enableUpdateMap","(IZ)I",(void*)JniEnableUpdateMap},
	//{ "native_getAlgMapData", "(II)Lcom/imscv/NaviPackSdk/data/AlgMapData", (void*)JniGetAlgMapData },

};

static int registerMethods(JNIEnv* env)
{

	const char* kClassName = "com/imscv/navipacksdk/NaviPackSdk";
	jclass clazz = env->FindClass(kClassName);
	if (clazz == NULL)
	{
		LOGE("Can't find class %s\n", kClassName);
		return -1;
	}
	if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) != JNI_OK)
	{
		LOGE("Failed registering methods for %s\n", kClassName);
		return -1;
	}
	return 0;
}


jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	//if ((*vm)->GetEnv(vm,(void**) &env, JNI_VERSION_1_4) != JNI_OK) {
	if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("ERROR: GetEnv failed\n");
		goto fail;
	}

	if (env == NULL)
	{
		goto fail;
	}


	if (registerMethods(env) != 0)
	{
		LOGE("ERROR: Platform Library native registration failed\n");
		goto fail;
	}
	result = JNI_VERSION_1_4;
fail: return result;
}
#endif