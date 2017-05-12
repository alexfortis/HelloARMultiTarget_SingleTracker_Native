LOCAL_PATH_TOP := $(call my-dir)
EASYAR_PACKAGE_PATH := $(LOCAL_PATH_TOP)/../../../../../../package

include $(CLEAR_VARS)
LOCAL_PATH := $(EASYAR_PACKAGE_PATH)/Android/libs/armeabi-v7a
LOCAL_MODULE:=libEasyAR
LOCAL_SRC_FILES:=libEasyAR.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_PATH := $(LOCAL_PATH_TOP)
LOCAL_C_INCLUDES += $(EASYAR_PACKAGE_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH_TOP)
LOCAL_CPPFLAGS += -std=c++11 -DANDROID -Wall -Wextra
LOCAL_LDLIBS += -llog -lGLESv2 -landroid
LOCAL_STATIC_LIBRARIES := libpng
LOCAL_SHARED_LIBRARIES += libEasyAR
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPP_EXTENSION += .cpp
LOCAL_MODULE := libHelloARNative
LOCAL_SRC_FILES := ar.cc \
		   helloar.cc \
		   renderer.cc \
		   asset_utils.cpp \
		   filedata.cpp \
		   rawimage.cpp \
		   shader.cpp \
		   texture.cpp
include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,src/3rdparty)
$(call import-module,libpng)
