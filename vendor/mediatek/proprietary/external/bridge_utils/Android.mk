
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	libbridge/libbridge_devif.c \
	libbridge/libbridge_if.c \
	libbridge/libbridge_init.c \
	libbridge/libbridge_misc.c


LOCAL_SHARED_LIBRARIES := \
	libcutils libc liblog


LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libbridge \
	$(LOCAL_PATH)/src

LOCAL_MODULE_TAGS := optional

#LOCAL_CFLAGS := -DANDROID_CHANGES -DCHAPMS=1 -DMPPE=1 -Iexternal/openssl/include


LOCAL_MODULE:= libbrctl

include $(BUILD_STATIC_LIBRARY)


###########################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	src/brctl.c \
	src/brctl_cmd.c \
	src/brctl_disp.c



LOCAL_SHARED_LIBRARIES := \
	libcutils libc liblog

LOCAL_STATIC_LIBRARIES := libbrctl

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libbridge \
	$(LOCAL_PATH)/src

LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_MODULE_TAGS := optional


#LOCAL_CFLAGS := -DANDROID_CHANGES -DCHAPMS=1 -DMPPE=1 -Iexternal/openssl/include


LOCAL_MODULE:= brctl

include $(BUILD_EXECUTABLE)
