
#include "CCNetwork.h"

NS_CC_EXTRA_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

bool CCNetwork::isLocalWiFiAvailable(void)
{
	return true;
}

bool CCNetwork::isInternetConnectionAvailable(void)
{
	return true;
}

bool CCNetwork::isHostNameReachable(const char* hostName)
{
	return true;
}

int CCNetwork::getInternetConnectionStatus(void)
{
	return 1;
}
#endif

NS_CC_EXTRA_END
