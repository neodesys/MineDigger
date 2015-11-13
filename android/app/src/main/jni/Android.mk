#
# Paths
#
LOCAL_PATH := $(call my-dir)

EXT_DIR := $(LOCAL_PATH)/../../../../../ext
SRC_DIR := $(LOCAL_PATH)/../../../../../src
PREBUILD_DIR := ../../../prebuild/$(TARGET_ARCH_ABI)

#
# SDL2_static prebuilt library
#
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_static
LOCAL_SRC_FILES := $(PREBUILD_DIR)/libSDL2.a
LOCAL_EXPORT_C_INCLUDES := $(EXT_DIR)/SDL2-2.0.3/include $(EXT_DIR)/SDL2-2.0.3/include/SDL2
LOCAL_EXPORT_LDLIBS := -Wl,--undefined=Java_org_libsdl_app_SDLActivity_nativeInit -ldl -lGLESv1_CM -lGLESv2 -llog -landroid
include $(PREBUILT_STATIC_LIBRARY)

#
# SDL2_image_static prebuilt library
#
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_image_static
LOCAL_SRC_FILES := $(PREBUILD_DIR)/libSDL2_image.a
LOCAL_EXPORT_C_INCLUDES := $(EXT_DIR)/SDL2_image-2.0.0/include
LOCAL_EXPORT_LDLIBS := -lz
include $(PREBUILT_STATIC_LIBRARY)

#
# SDL2_mixer_static prebuilt library
#
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_mixer_static
LOCAL_SRC_FILES := $(PREBUILD_DIR)/libSDL2_mixer.a
LOCAL_EXPORT_C_INCLUDES := $(EXT_DIR)/SDL2_mixer-2.0.0/include
include $(PREBUILT_STATIC_LIBRARY)

#
# SDL2_ttf_static prebuilt library
#
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_ttf_static
LOCAL_SRC_FILES := $(PREBUILD_DIR)/libSDL2_ttf.a
LOCAL_EXPORT_C_INCLUDES := $(EXT_DIR)/SDL2_ttf-2.0.12/include
include $(PREBUILT_STATIC_LIBRARY)

#
# MineDigger build
#
include $(CLEAR_VARS)
LOCAL_MODULE := MineDigger

RECFIND_CPP = $(strip $(wildcard $1/*.cpp) $(foreach d, $(wildcard $1/*), $(call RECFIND_CPP, $d)))
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, $(call RECFIND_CPP, $(SRC_DIR)))

LOCAL_PCH := $(subst $(LOCAL_PATH)/,, $(SRC_DIR)/precomp.h)

LOCAL_CPPFLAGS += -Wall -Werror -std=c++11 -pedantic

LOCAL_STATIC_LIBRARIES := SDL2_static SDL2_image_static SDL2_mixer_static SDL2_ttf_static

include $(BUILD_SHARED_LIBRARY)
