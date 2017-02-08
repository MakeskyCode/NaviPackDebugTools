LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

# 一些系统变量输出，便于查看当前的编译系统信息
$(info --------------------- BUILDER INFOMATION ---------------------)
$(info [NDK_ROOT] = $(NDK_ROOT))
$(info [HOST_ARCH] = $(HOST_ARCH))
$(info [HOST_TAG] = $(HOST_TAG))
$(info [TARGET_ARCH] = $(TARGET_ARCH))
$(info [TARGET_ABI] = $(TARGET_ABI))
$(info [TARGET_TOOLCHAIN] = $(TARGET_TOOLCHAIN))
$(info [APP_PROJECT_PATH] = $(APP_PROJECT_PATH))
$(info --------------------------------------------------------------)

# 遍历给定目录下的所有.cpp文件
# 遍历目录及子目录的函数
define walk
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef
# 以下源文件内容，全部采用绝对路径，方便VS在出错后，在output窗口双击直达目标文件
# APP_PROJECT_PATH 是在编译命令行时指定

PRJ_ABS_PATH := $(APP_PROJECT_PATH)
ALLFILES = $(call walk, $(PRJ_ABS_PATH))
ALLFILES += $(call walk, $(PRJ_ABS_PATH)/../commonSrc)

FILE_LIST += $(filter %.cpp, $(ALLFILES))
FILE_LIST -= 
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

# 设置包含头文件，貌似NDK的编译系统，会自动添加$(LOCAL_PATH)到头文件搜索路径中
# 因此，只需要设置除LOCAL_PATH路径之外的其他路径即可
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../commonSrc/

# 设置输出模块名称
LOCAL_MODULE := NaviPackSdk
#输出日志
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS += -DBUILD_ANDROID_APP -DBUILD_ANDROID_DEBUG

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)

