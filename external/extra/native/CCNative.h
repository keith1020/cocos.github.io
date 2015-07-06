
#ifndef __CC_EXTENSION_CCNATIVE_H_
#define __CC_EXTENSION_CCNATIVE_H_

#include "../cocos2dx_extra.h"

NS_CC_EXTRA_BEGIN

class CCNative
{
public:
    /** @brief Get OpenUDID value */
    static const string getDeviceID(void);
    static const string getIDForVendor(void);
	/** 取得随机唯一ID **/
	static const string getUniqeID(void);
    static const string getOSVersion();
    /** @brief Open a web page in the browser; create an email; or call a phone number. */
    static void openURL(const char* url);
    static const string getDeviceName();
	static string getCurrentLanguage();
	static string getDeviceExtraInformation();

	/****安卓的机器的特有的***/
	static string getMacAddress();
	static string getAndroidID();
	static string getTelephonyDeviceID();
	static string getAdvertiseID();
	///! in M
	static unsigned int getTotalMem();
	static unsigned int getFreeMem();

private:
    CCNative(void) {}
};

NS_CC_EXTRA_END

#endif // __CC_EXTENSION_CCNATIVE_H_
