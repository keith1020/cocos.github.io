LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := PluginProtocolStatic

LOCAL_MODULE_FILENAME := libPluginProtocolStatic

LOCAL_SRC_FILES :=\
protocols/platform/android/PluginFactory.cpp \
protocols/platform/android/PluginJniHelper.cpp \
protocols/platform/android/PluginUtils.cpp \
protocols/platform/android/PluginProtocol.cpp \
protocols/platform/android/ProtocolAnalytics.cpp \
protocols/platform/android/ProtocolIAP.cpp \
protocols/platform/android/ProtocolAds.cpp \
protocols/platform/android/ProtocolShare.cpp \
protocols/platform/android/ProtocolUser.cpp \
protocols/platform/android/ProtocolSocial.cpp \
protocols/PluginManager.cpp \
protocols/PluginParam.cpp \
GooglePlayService/SocialGooglePlay.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/protocols/include $(LOCAL_PATH)/protocols/platform/android
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include $(LOCAL_PATH)/../../platform/android

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)
