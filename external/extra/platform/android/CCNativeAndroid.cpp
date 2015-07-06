
#include "../../native/CCNative.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

NS_CC_EXTRA_BEGIN
const std::string CCNative::getDeviceID(void)
{
#define UDID_KEY "__UDID_KEY__"
	std::string udid;
	UserDefault* userDefault = UserDefault::getInstance();
	if( NULL != userDefault )
	{
		udid = userDefault->getStringForKey( UDID_KEY );
	}
	if( udid.size() == 0 )
	{
		//GUID guid = {0};
		//char szGuid[128]={0};
		//CoCreateGuid(&guid);
		//sprintf(szGuid, "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
		//udid = "aaaaaaaaaaaaaaaaaaaa";
		udid = Application::getInstance()->getMacAddress();
		/// xx:xx:xx:xx:xx
		///一共有16位
		///4个冒号
		std::string::iterator end_pos = std::remove(udid.begin(), udid.end(), ' ');
		udid.erase(end_pos, udid.end()); 
		if( userDefault && udid.size() >= 16 )
		{
			userDefault->setStringForKey(UDID_KEY, udid);
		}
	}
	return udid;
}
const std::string CCNative::getUniqeID(void)
{
    JniMethodInfo t;
    std::string ret("");
    
    CCLOG("getUniqeID()");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getUniqeID", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }else{
        CCLOG("getUniqeID failed");
    }
    
    return ret;
}

string CCNative::getAndroidID()
{
    JniMethodInfo t;
    std::string ret("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getAndroidID", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }else{
        CCLOG("getAndroidID failed");
    }
    
    return ret;
}
string CCNative::getTelephonyDeviceID()
{
    JniMethodInfo t;
    std::string ret("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getTelephonyDeviceID", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }else{
        CCLOG("getTelephonyDeviceID failed");
    }
    return ret;
}
string CCNative::getMacAddress()
{
    JniMethodInfo t;
    std::string ret("");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getMacAddress", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }else{
        CCLOG("getMacAddress failed");
    }
    return ret;
}
std::string CCNative::getDeviceExtraInformation(void)
{
    JniMethodInfo t;
    std::string ret("");
    CCLOG("getDeviceExtraInformation");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getDeviceExtraInformation", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }else{
        CCLOG("getDeviceExtraInformation failed");
    }
    return ret;
}
std::string CCNative::getAdvertiseID(void)
{
    JniMethodInfo t;
    std::string ret("");
    CCLOG("getAdvertiseID");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getAdvertiseID", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }else{
        CCLOG("getAdvertiseID failed");
    }
    return ret;
}

const std::string CCNative::getIDForVendor(void)
{
    return getDeviceID();
}

const std::string CCNative::getDeviceName(void)
{
    JniMethodInfo t;
    std::string ret("");
    
    CCLOG("getDeviceName");
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getDeviceName", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }else{
        CCLOG("getDeviceName failed");
    }
    
    return ret;
}

const string CCNative::getOSVersion()
{
    return "Android";
}
void CCNative::openURL(const char* url)
{
    if( NULL == url ){
        return;
    }
    Application::getInstance()->openURL(url);
	//WCHAR urlWideChar[ 256 ] = { 0 };
	//MultiByteToWideChar( CP_ACP,0,url,strlen(url)+1,urlWideChar, sizeof(urlWideChar)/sizeof(urlWideChar[0]) );
	//ShellExecute(NULL, L"open", urlWideChar, NULL, NULL, SW_SHOWNORMAL);
}


static unsigned int getFreeMemoryImpl(const char* const sums[], const size_t sumsLen[], size_t num)
{
    int fd = open("/proc/meminfo", O_RDONLY);
    
    if (fd < 0) {
        return -1;
    }
    
    char buffer[256];
    const int len = read(fd, buffer, sizeof(buffer)-1);
    close(fd);
    
    if (len < 0) {
        return -1;
    }
    buffer[len] = 0;
    
    size_t numFound = 0;
    unsigned long long mem = 0;
    
    char* p = buffer;
    while (*p && numFound < num) {
        int i = 0;
        while (sums[i]) {
            if (strncmp(p, sums[i], sumsLen[i]) == 0) {
                p += sumsLen[i];
                while (*p == ' ') p++;
                char* num = p;
                while (*p >= '0' && *p <= '9') p++;
                if (*p != 0) {
                    *p = 0;
                    p++;
                    if (*p == 0) p--;
                }
                mem += atoll(num) * 1024;
                numFound++;
                break;
            }
            i++;
        }
        p++;
    }
    
    return numFound > 0 ? (mem / (1024 * 1024)) : -1;
}

unsigned int CCNative::getTotalMem()
{
    static const char* const sums[] = { "MemTotal:", NULL };
    static const size_t sumsLen[] = { strlen("MemTotal:"), 0 };
    return getFreeMemoryImpl(sums, sumsLen, 1);
}
unsigned int CCNative::getFreeMem()
{
    static const char* const sums[] = { "MemFree:", "Cached:", NULL };
    static const size_t sumsLen[] = { strlen("MemFree:"), strlen("Cached:"), 0 };
    return getFreeMemoryImpl(sums, sumsLen, 2);
}

NS_CC_EXTRA_END
