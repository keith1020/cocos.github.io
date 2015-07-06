
#import "IAPWrapper.h"
#include "PluginUtilsIOS.h"
#include "ProtocolIAP.h"

using namespace cocos2d::plugin;

@implementation IAPWrapper

+ (void) onPayResult:(id) obj withRet:(PayResult) ret withMsg:(NSString*) msg
{
    PluginProtocol* pPlugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(pPlugin);
    if (pIAP) {
        pIAP->onPayResult((PayResultCode)ret, [msg UTF8String]);
    } else {
        PluginUtilsIOS::outputLog("Can't find the C++ object of the Share plugin");
    }
}

@end
