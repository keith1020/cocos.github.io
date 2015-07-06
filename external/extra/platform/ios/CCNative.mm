#include <sys/types.h>
#include <sys/sysctl.h>
#include "../../native/CCNative.h"
#include "AdSupport/ASIdentifierManager.h"
#include <GameKit/GameKit.h>
#include "base/CCUserDefault.h"

#include <sys/socket.h>
#include <net/if.h>
#include <net/if_dl.h>

NS_CC_EXTRA_BEGIN

const std::string CCNative::getDeviceName(void)
{
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = new char[size+1];
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    delete[] machine;
    return [platform UTF8String];
}

const std::string CCNative::getDeviceID(void)
{
#define UDID_KEY "__UDID_KEY__"
    std::string udid;
    UserDefault* userDefault = UserDefault::getInstance();
    if( nullptr != userDefault )
    {
        udid = userDefault->getStringForKey( UDID_KEY );
    }
    if( 0 == udid.size() ) {
        if (nil == NSClassFromString(@"ASIdentifierManager")) {
            bool isSucceeded = false;
            do {
                int                 mib[6];
                size_t              len;
                char                *buf;
                unsigned char       *ptr;
                struct if_msghdr    *ifm;
                struct sockaddr_dl  *sdl;
                
                mib[0] = CTL_NET;
                mib[1] = AF_ROUTE;
                mib[2] = 0;
                mib[3] = AF_LINK;
                mib[4] = NET_RT_IFLIST;
                
                if ((mib[5] = if_nametoindex("en0")) == 0) {
                    printf("Error: if_nametoindex error\n");
                    break;
                }
                
                if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
                    printf("Error: sysctl, take 1\n");
                    break;
                }
                
                if ((buf = (char *)malloc(len)) == NULL) {
                    printf("Could not allocate memory. error!\n");
                    break;
                }
                
                if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
                    printf("Error: sysctl, take 2");
                    break;
                }
                
                ifm = (struct if_msghdr *)buf;
                sdl = (struct sockaddr_dl *)(ifm + 1);
                ptr = (unsigned char *)LLADDR(sdl);
                NSString *outstring = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X",
                                       *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
                free(buf);
                
                udid = [outstring UTF8String];
                
                isSucceeded = true;
            }while (0);
            if (!isSucceeded) {
                CFUUIDRef uuidRef = CFUUIDCreate(kCFAllocatorDefault);
                udid = [(NSString *)CFUUIDCreateString(NULL,uuidRef) UTF8String];
                CFRelease(uuidRef);
            }
        }else {
			BOOL isIDFAEnabled = true;
			isIDFAEnabled = [ASIdentifierManager sharedManager].isAdvertisingTrackingEnabled;
			if(false == isIDFAEnabled ){
				CFUUIDRef uuidRef = CFUUIDCreate(kCFAllocatorDefault);
				udid = [(NSString *)CFUUIDCreateString(NULL,uuidRef) UTF8String];
				CFRelease(uuidRef);
			}else{
				udid = [[[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString] UTF8String];
			}
        }
        if( nullptr != userDefault ) {
            userDefault->setStringForKey(UDID_KEY, udid);
            userDefault->flush();
        }
    }
	return udid;
}

const std::string CCNative::getIDForVendor(void)
{
#define UDID_KEY_VENDOR "__UDID_KEY_VENDOR__"
    std::string udid;
    UserDefault* userDefault = UserDefault::getInstance();
    if( NULL != userDefault )
    {
        udid = userDefault->getStringForKey( UDID_KEY_VENDOR );
    }
    if( udid.size() == 0 )
    {
        if ([UIDevice.currentDevice respondsToSelector:@selector(identifierForVendor)])
        {
            udid = [[UIDevice.currentDevice.identifierForVendor UUIDString] UTF8String];
        }
        else
        {
            CFUUIDRef uuidRef = CFUUIDCreate(kCFAllocatorDefault);
            udid = [(NSString *)CFUUIDCreateString(NULL,uuidRef) UTF8String];
            CFRelease(uuidRef);
        }
        if( userDefault )
        {
            userDefault->setStringForKey(UDID_KEY_VENDOR, udid);
        }
    }
    return udid;
}
const string CCNative::getOSVersion()
{
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    currSysVer = [NSString stringWithFormat:@"%.2f",[currSysVer floatValue]];
    return [currSysVer UTF8String];
}

void CCNative::openURL(const char* url)
{
    if (!url) return;
    NSURL *nsurl = [NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];
    bool isOpenSuccess = [[UIApplication sharedApplication] openURL:nsurl];
    NSLog( @"isOpenSuccess = %d , %@", isOpenSuccess, nsurl );
}

unsigned int CCNative::getTotalMem()
{
    // Find the total amount of memory
    @try {
        // Set up the variables
        double TotalMemory = 0.00;
        double AllMemory = [[NSProcessInfo processInfo] physicalMemory];
        
        // Total Memory (formatted)
        TotalMemory = (AllMemory / 1024.0) / 1024.0;
        
        // Round to the nearest multiple of 256mb - Almost all RAM is a multiple of 256mb (I do believe)
        int toNearest = 256;
        int remainder = (int)TotalMemory % toNearest;
        
        if (remainder >= toNearest / 2) {
            // Round the final number up
            TotalMemory = ((int)TotalMemory - remainder) + 256;
        } else {
            // Round the final number down
            TotalMemory = (int)TotalMemory - remainder;
        }
        
        // Check to make sure it's valid
        if (TotalMemory <= 0) {
            // Error, invalid memory value
            return -1;
        }
        
        // Completed Successfully
        return TotalMemory;
    }
    @catch (NSException *exception) {
        // Error
        return -1;
    }
}

const std::string CCNative::getUniqeID(void)
{
    return getDeviceID();
}

std::string CCNative::getAdvertiseID(void)
{
    return getDeviceID();
}
std::string CCNative::getTelephonyDeviceID(void)
{
    return getDeviceID();
}
std::string CCNative::getMacAddress(void)
{
    return getDeviceID();
}
std::string CCNative::getAndroidID(void)
{
    return getDeviceID();
}
#include <mach/mach.h>
unsigned int CCNative::getFreeMem()
{
    @try {
        // Set up the variables
        double TotalMemory = 0.00;
        vm_statistics_data_t vmStats;
        mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
        kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
        
        if(kernReturn != KERN_SUCCESS) {
            return -1;
        }
        
        TotalMemory = ((vm_page_size * vmStats.free_count) / 1024.0) / 1024.0;
        
        // Check to make sure it's valid
        if (TotalMemory <= 0) {
            // Error, invalid memory value
            return -1;
        }
        
        // Completed Successfully
        return TotalMemory;
    }
    @catch (NSException *exception) {
        // Error
        return -1;
    }
}
std::string CCNative::getDeviceExtraInformation(void)
{
    return getDeviceID();
}


NS_CC_EXTRA_END
