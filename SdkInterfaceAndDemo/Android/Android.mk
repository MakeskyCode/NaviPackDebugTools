LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

# һЩϵͳ������������ڲ鿴��ǰ�ı���ϵͳ��Ϣ
$(info --------------------- BUILDER INFOMATION ---------------------)
$(info [NDK_ROOT] = $(NDK_ROOT))
$(info [HOST_ARCH] = $(HOST_ARCH))
$(info [HOST_TAG] = $(HOST_TAG))
$(info [TARGET_ARCH] = $(TARGET_ARCH))
$(info [TARGET_ABI] = $(TARGET_ABI))
$(info [TARGET_TOOLCHAIN] = $(TARGET_TOOLCHAIN))
$(info [APP_PROJECT_PATH] = $(APP_PROJECT_PATH))
$(info --------------------------------------------------------------)

# ��������Ŀ¼�µ�����.cpp�ļ�
# ����Ŀ¼����Ŀ¼�ĺ���
define walk
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef
# ����Դ�ļ����ݣ�ȫ�����þ���·��������VS�ڳ������output����˫��ֱ��Ŀ���ļ�
# APP_PROJECT_PATH ���ڱ���������ʱָ��

PRJ_ABS_PATH := $(APP_PROJECT_PATH)
ALLFILES = $(call walk, $(PRJ_ABS_PATH))
ALLFILES += $(call walk, $(PRJ_ABS_PATH)/../commonSrc)

FILE_LIST += $(filter %.cpp, $(ALLFILES))
FILE_LIST -= 
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

# ���ð���ͷ�ļ���ò��NDK�ı���ϵͳ�����Զ����$(LOCAL_PATH)��ͷ�ļ�����·����
# ��ˣ�ֻ��Ҫ���ó�LOCAL_PATH·��֮�������·������
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../commonSrc/

# �������ģ������
LOCAL_MODULE := NaviPackSdk
#�����־
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS += -DBUILD_ANDROID_APP -DBUILD_ANDROID_DEBUG

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)

