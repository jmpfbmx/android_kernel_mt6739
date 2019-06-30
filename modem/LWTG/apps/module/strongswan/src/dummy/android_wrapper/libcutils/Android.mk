LOCAL_PATH:= $(call my-dir)

###############################
# IMS Core library
###############################

include $(CLEAR_VARS)
LOCAL_MODULE             := libcutils_st
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER       := mtk
LOCAL_MODULE_TAGS        := optional

LOCAL_SRC_FILES         := socket_local_client_unix.c strdup8to16.c properties.c

LOCAL_ARM_MODE          := arm
LOCAL_C_INCLUDES        := $(LOCAL_PATH)/src           \
                           $(LOCAL_PATH)/include
LOCAL_C_INCLUDES        += $(LOCAL_PATH)/../

LOCAL_CFLAGS            += -D __ANDROID__

LOCAL_STATIC_LIBRARIES  +=
LOCAL_SHARED_LIBRARIES  := 
LOCAL_PRELINK_MODULE    := false

LOCAL_CFLAGS            += $(EXTERNAL_CFLAGS)
LOCAL_C_INCLUDES        += $(EXTERNAL_C_INCLUDES)

# lib no need LDLIBS
#LOCAL_LDLIBS            += $(EXTERNAL_LDLIBS)

LOCAL_STATIC_LIBRARIES  += $(EXTERNAL_STATIC_LIBRARIES)
LOCAL_SHARED_LIBRARIES  += $(EXTERNAL_SHARED_LIBRARIES)

include $(MTK_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE             := libcutils
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER       := mtk
LOCAL_MODULE_TAGS        := optional

LOCAL_SRC_FILES         := socket_local_client_unix.c strdup8to16.c properties.c

LOCAL_ARM_MODE          := arm

LOCAL_C_INCLUDES        += $(EXTERNAL_C_INCLUDES)
LOCAL_C_INCLUDES        += $(LOCAL_PATH)/../

LOCAL_CFLAGS            += -D __ANDROID__ -O0 -g

LOCAL_C_INCLUDES        += $(EXTERNAL_C_INCLUDES)

LOCAL_SHARED_LIBRARIES  := 
LOCAL_PRELINK_MODULE    := false

ifdef MTK_SHARED_LIBRARY
	include $(MTK_SHARED_LIBRARY)
else
	include $(BUILD_SHARED_LIBRARY)
endif
