/****************************************************************************
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "lua_cocos2dx_extra_manual.hpp"
#include "external/extra/native/CCNative.h"
#include "external/extra/network/CCNetwork.h"
#include "external/plugin/protocols/include/PluginManager.h"
#include "external/plugin/protocols/include/ProtocolSocial.h"
#include "external/plugin/protocols/include/ProtocolShare.h"
#include "external/plugin/protocols/include/ProtocolAnalytics.h"
#include "external/plugin/protocols/include/ProtocolAds.h"
#include "external/plugin/protocols/include/ProtocolUser.h"
#include "external/plugin/protocols/include/ProtocolIAP.h"
#include "external/plugin/GooglePlayService/SocialGooglePlay.h"
#include "LuaBasicConversions.h"

USING_NS_CC_EXTRA;

int lua_cocos2dx_extra_Native_getDeviceID(lua_State* tolua_S)
{
	string tolua_ret = CCNative::getDeviceID();
	tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
	return 1;
}
int lua_cocos2dx_extra_Native_getUniqeID(lua_State* tolua_S)
{
    string tolua_ret = CCNative::getUniqeID();
    tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
    return 1;
}
int lua_cocos2dx_extra_Native_getIDForVendor(lua_State* tolua_S)
{
	string tolua_ret = CCNative::getIDForVendor();
	tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
	return 1;
}
int lua_cocos2dx_extra_Native_getSystemVersion(lua_State* tolua_S)
{
    string tolua_ret = CCNative::getOSVersion();
    tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
    return 1;
}

int lua_cocos2dx_extra_Native_getDeviceExtraInformation(lua_State* tolua_S)
{
	string tolua_ret = CCNative::getDeviceExtraInformation();
	tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
	return 1;
}

int lua_cocos2dx_extra_Native_getMacAddress(lua_State* tolua_S)
{
	string tolua_ret = CCNative::getMacAddress();
	tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
	return 1;
}
int lua_cocos2dx_extra_Native_getAdvertiseID(lua_State* tolua_S)
{
	string tolua_ret = CCNative::getAdvertiseID();
	tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
	return 1; 
}

int lua_cocos2dx_extra_Native_getAndroidID(lua_State* tolua_S)
{
	string tolua_ret = CCNative::getAndroidID();
	tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
	return 1;
}

int lua_cocos2dx_extra_Native_getTelephonyDeviceID(lua_State* tolua_S)
{
	string tolua_ret = CCNative::getTelephonyDeviceID();
	tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
	return 1;
}
int lua_cocos2dx_extra_Native_getDeviceMem(lua_State* tolua_S)
{
	unsigned int iTotalMem = CCNative::getTotalMem();
	unsigned int iFreeMem = CCNative::getFreeMem();
	tolua_pushnumber( tolua_S , iTotalMem);
	tolua_pushnumber( tolua_S , iFreeMem);
	return 2;
}

int lua_cocos2dx_extra_Native_getDeviceName(lua_State* tolua_S)
{
    string tolua_ret = CCNative::getDeviceName();
    tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
    return 1;
}
static int lua_cocos2dx_extra_Native_openURL(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.Native",0,&tolua_err) ||
		!tolua_isstring(tolua_S,2,0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
		{
			CCNative::openURL(url);
		}
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'openURL'.",&tolua_err);
	return 0;
#endif
}

int lua_register_cocos2dx_extra_native(lua_State* L)
{
	tolua_usertype(L,"cc.Native");
	tolua_cclass(L,"Native","cc.Native","",nullptr);
	tolua_beginmodule(L,"Native");
    
        tolua_function(L,"getDeviceName",lua_cocos2dx_extra_Native_getDeviceName);
		tolua_function(L,"getDeviceID",lua_cocos2dx_extra_Native_getDeviceID);
        tolua_function(L,"getIDForVendor",lua_cocos2dx_extra_Native_getIDForVendor);
		tolua_function(L,"getUniqeID",lua_cocos2dx_extra_Native_getUniqeID);
        tolua_function(L,"getSystemVersion",lua_cocos2dx_extra_Native_getSystemVersion);
        tolua_function(L,"openURL",lua_cocos2dx_extra_Native_openURL);
		tolua_function(L,"getDeviceMem",lua_cocos2dx_extra_Native_getDeviceMem);
		tolua_function(L,"getDeviceExtraInformation",lua_cocos2dx_extra_Native_getDeviceExtraInformation);

		tolua_function(L,"getAndroidID",lua_cocos2dx_extra_Native_getAndroidID);
		tolua_function(L,"getTelephonyDeviceID",lua_cocos2dx_extra_Native_getTelephonyDeviceID);
		tolua_function(L,"getMacAddress",lua_cocos2dx_extra_Native_getMacAddress);
		tolua_function(L,"getAdvertiseID",lua_cocos2dx_extra_Native_getAdvertiseID);
    
	tolua_endmodule(L);
	return 1;
}

static int lua_cocos2dx_extra_Network_isLocalWiFiAvailable(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.ExtraNetwork",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		bool isWifi = CCNetwork::isLocalWiFiAvailable();
		tolua_pushboolean(tolua_S,isWifi);
		return 1;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isLocalWiFiAvailable'.",&tolua_err);
	return 0;
#endif
}

static int lua_cocos2dx_extra_Network_isInternetConnectionAvailable(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.ExtraNetwork",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		bool isWifi = CCNetwork::isInternetConnectionAvailable();
		tolua_pushboolean(tolua_S,isWifi);
		return 1;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isInternetConnectionAvailable'.",&tolua_err);
	return 0;
#endif
}


static int lua_cocos2dx_extra_Network_isHostNameReachable(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.ExtraNetwork",0,&tolua_err)||
		!tolua_isstring(tolua_S,2,0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		const char* a = (const char*)  tolua_tostring(tolua_S,2,0);
		bool isWifi = CCNetwork::isHostNameReachable( a );
		tolua_pushboolean(tolua_S,isWifi);
		return 1;
	}
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'isHostNameReachable'.",&tolua_err);
	return 0;
#endif
}

static int lua_cocos2dx_extra_Network_getInternetConnectionStatus(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.ExtraNetwork",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int internateCode = CCNetwork::getInternetConnectionStatus();
		tolua_pushnumber(tolua_S, (lua_Number)internateCode);
		return 1;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getInternetConnectionStatus'.",&tolua_err);
	return 0;
#endif
}


int lua_register_cocos2dx_extra_network(lua_State* L)
{
	tolua_usertype(L,"cc.ExtraNetwork");
	tolua_cclass(L,"ExtraNetwork","cc.ExtraNetwork","",nullptr);
	tolua_beginmodule(L,"ExtraNetwork");
		tolua_constant(L,"NetworkStatusNotReachable",kCCNetworkStatusNotReachable);
		tolua_constant(L,"NetworkStatusReachableViaWiFi",kCCNetworkStatusReachableViaWiFi);
		tolua_constant(L,"NetworkStatusReachableViaWWAN",kCCNetworkStatusReachableViaWWAN);

		tolua_function(L,"isLocalWiFiAvailable",lua_cocos2dx_extra_Network_isLocalWiFiAvailable);
		tolua_function(L,"isInternetConnectionAvailable",lua_cocos2dx_extra_Network_isInternetConnectionAvailable);
		tolua_function(L,"isHostNameReachable",lua_cocos2dx_extra_Network_isHostNameReachable);
		tolua_function(L,"getInternetConnectionStatus",lua_cocos2dx_extra_Network_getInternetConnectionStatus);

	tolua_endmodule(L);
	return 1;
}


static int lua_cocos2dx_extra_PluginManager_initTencent(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
        class TencetServiceListener : public cocos2d::plugin::SocialListener
        {
        public:
            TencetServiceListener(){}
            virtual ~TencetServiceListener(){}
            void onSocialResult(cocos2d::plugin::SocialRetCode code, const char* msg)
            {
                auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
                if( code == cocos2d::plugin::LOGIN_SUCCESS && NULL != msg )
                {
                    eventDispatcher->dispatchCustomEvent( "TencentLoginSuccess" );
                }
                if( code == cocos2d::plugin::LOGIN_FAILED || NULL == msg )
                {
                    eventDispatcher->dispatchCustomEvent( "TencentLoginFailed" );
                }
				if( code == cocos2d::plugin::StateSafe || NULL != msg )
                {
                    eventDispatcher->dispatchCustomEvent( "TencentSafe");
                }
            }
        };
        static TencetServiceListener listener;
        cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
        if( tencentPlugin )
        {
            tencentPlugin->setListener( &listener );
            cocos2d::plugin::TSocialDeveloperInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
            tencentPlugin->configDeveloperInfo( dict );
        }
        else
        {
            listener.onSocialResult(cocos2d::plugin::LOGIN_FAILED, "");
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'initTencent'.",&tolua_err);
    return 0;
#endif
}
static int lua_cocos2dx_extra_PluginManager_logoutTencent(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
        if( tencentPlugin )
        {
            tencentPlugin->logout();
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'logoutGooglePlay'.",&tolua_err);
    return 0;
#endif
}
static int lua_cocos2dx_extra_PluginManager_loginTencent(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int argc = lua_gettop( tolua_S ) - 1;
        if (1 == argc)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            lua_Number platform =  tolua_tonumber(tolua_S,2,0);
            cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
            if( tencentPlugin )
            {
                cocos2d::plugin::PluginParam pluginParam( (int)(platform) );
                tencentPlugin->callFuncWithParam("setPlatform", &pluginParam, NULL );
				tencentPlugin->login();
                return 0;
            }
#endif
            return 0;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginTencent'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_sendToQQ(lua_State* tolua_S)
{
	#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
        cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
        if( tencentPlugin )
        {
            cocos2d::plugin::TSocialShareInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
			cocos2d::plugin::PluginParam params( dict );
            tencentPlugin->callFuncWithParam("tencentQQSharedInfo", &params, NULL );
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'sendToQQ'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_shareBigPicture(lua_State* tolua_S)
{
	#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
        cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
        if( tencentPlugin )
        {
            cocos2d::plugin::TSocialShareInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
			cocos2d::plugin::PluginParam params( dict );
            tencentPlugin->callFuncWithParam("tencentBigPictureShared", &params, NULL );
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'shareBigPicture'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_sendToQQGameFriend(lua_State* tolua_S)
{
	#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
        cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
        if( tencentPlugin )
        {
            cocos2d::plugin::TSocialShareInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
			cocos2d::plugin::PluginParam params( dict );
            tencentPlugin->callFuncWithParam("tencentSendToQQGameFriend", &params, NULL );
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'sendToQQGameFriend'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_queryQQGameFriendsInfo( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
        if( tencentPlugin)
        {
            tencentPlugin->callFuncWithParam("queryQQGameFriendsInfo" , NULL);
        }
#endif
        return 1;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'queryQQGameFriendsInfo'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_getTencentValue(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int argc = lua_gettop( tolua_S ) - 1;
        if (1 == argc)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            const char* param = ((const char*)  tolua_tostring(tolua_S,2,0));
            cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
            if( tencentPlugin && param )
            {
                cocos2d::plugin::PluginParam pluginParam( param );
                std::string s = tencentPlugin->callStringFuncWithParam("getValue", &pluginParam, NULL );
                tolua_pushcppstring(tolua_S,s);
                return 1;
            }
#endif
            return 0;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
        return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_getGooglePlayAccountInfo(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        cocos2d::plugin::ProtocolSocial* googlePlay = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGooglePlay"));
        if( googlePlay )
        {
            std::string s = cocos2d::plugin::SocialGooglePlayHelper::getGooglePlayAccountInfo();
            tolua_pushcppstring(tolua_S,s);
            return 1;
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getGooglePlayValue'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_logoutGooglePlay(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        cocos2d::plugin::ProtocolSocial* googlePlay = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGooglePlay"));
        if( googlePlay )
        {
            cocos2d::plugin::SocialGooglePlayHelper::setGooglePlayAccountInfo( "" );
            googlePlay->logout();
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'logoutGooglePlay'.",&tolua_err);
    return 0;
#endif
}
static int lua_cocos2dx_extra_PluginManager_loginGooglePlay(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        class GooglePlayServiceListener : public cocos2d::plugin::SocialListener
        {
        public:
            GooglePlayServiceListener(){}
            virtual ~GooglePlayServiceListener(){}
            void onSocialResult(cocos2d::plugin::SocialRetCode code, const char* msg)
            {
                auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
                if( code == cocos2d::plugin::LOGIN_SUCCESS && NULL != msg )
                {
                    cocos2d::plugin::SocialGooglePlayHelper::setGooglePlayAccountInfo( msg );
                    eventDispatcher->dispatchCustomEvent( "GooglePlaySuccess" );
                }
                if( code == cocos2d::plugin::LOGIN_FAILED || NULL == msg )
                {
                    eventDispatcher->dispatchCustomEvent( "GooglePlayFailed" );
                }
            }
        };
        static GooglePlayServiceListener listener;
        cocos2d::plugin::ProtocolSocial* googlePlay = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGooglePlay"));
        if( googlePlay )
        {
            googlePlay->setListener( &listener );
            cocos2d::plugin::TSocialDeveloperInfo googlePlayParam;
            googlePlayParam["placeholder"] = "empty";
            googlePlay->configDeveloperInfo( googlePlayParam );
            googlePlay->login();
        }
        else
        {
            listener.onSocialResult(cocos2d::plugin::LOGIN_FAILED, "");
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginGooglePlay'.",&tolua_err);
    return 0;
#endif
}
static int lua_cocos2dx_extra_PluginManager_setPluginName( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		int argc = lua_gettop(tolua_S)-1;
		if (argc != 2)
		{
			CCLOG("setPluginName has wrong number of arguments: %d, was expecting %d \n", argc, 2);
			return 0;
		}
		std::string key;
		luaval_to_std_string(tolua_S, 2,&key);
		std::string value;
		luaval_to_std_string(tolua_S, 3,&value);
		cocos2d::plugin::PluginManager::getInstance()->setPluinName( key, value);
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
	return 0;
#endif
}
static int lua_cocos2dx_extra_PluginManager_loginGamecenter(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        class GameCenterResultListener : public cocos2d::plugin::SocialListener
        {
        public:
            GameCenterResultListener(){}
            virtual ~GameCenterResultListener(){}
            void onSocialResult(cocos2d::plugin::SocialRetCode code, const char* msg)
            {
                auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
                if( code == cocos2d::plugin::LOGIN_SUCCESS )
                {
                    eventDispatcher->dispatchCustomEvent( "GameCenterSuccess" );
                }
                if( code == cocos2d::plugin::LOGIN_FAILED )
                {
                    eventDispatcher->dispatchCustomEvent( "GameCenterFailed" );
                }
                if( code == cocos2d::plugin::STATE_CHANGE )
                {
                    eventDispatcher->dispatchCustomEvent( "GameCenterStateChange" );
                }
            }
        };
        static GameCenterResultListener listener;
        cocos2d::plugin::ProtocolSocial* gameCenter = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGamecenter"));
        if( gameCenter )
        {
            gameCenter->setListener( &listener );
            cocos2d::plugin::TSocialDeveloperInfo gameCenterParam;
            gameCenterParam["placeholder"] = "empty";
            gameCenter->configDeveloperInfo( gameCenterParam );
        }
        else
        {
            listener.onSocialResult(cocos2d::plugin::LOGIN_FAILED, "");
        }
		#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
    return 0;
#endif
}
static int lua_cocos2dx_extra_PluginManager_setLeaderboards( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int argc = lua_gettop(tolua_S)-1;
		if (argc != 2)
		{
			CCLOG("setLeaderboards has wrong number of arguments: %d, was expecting %d \n", argc, 2);
			return 0;
		}
		std::string key;
		luaval_to_std_string(tolua_S, 2,&key);

		long score;
		luaval_to_long(tolua_S, 3,&score);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		cocos2d::plugin::ProtocolSocial* socialPltform = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGamecenter"));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGooglePlay"));
#endif
		if( socialPltform )
		{
			socialPltform->submitScore( key.c_str() , score );
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'setLeaderboards'.",&tolua_err);
	return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_showAchievements(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		cocos2d::plugin::ProtocolSocial* socialPltform = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGamecenter"));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGooglePlay"));
#endif
		if( socialPltform )
		{
			socialPltform->showAchievements();
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unlockAchievement'.",&tolua_err);
	return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_showLeaderboards(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{	
		int argc = lua_gettop(tolua_S)-1;
		if (argc != 1)
		{
			CCLOG("showLeaderboards has wrong number of arguments: %d, was expecting %d \n", argc, 1);
		}
		std::string ret;
		if (!luaval_to_std_string(tolua_S, 2, &ret))
		{
#if COCOS2D_DEBUG >= 1
			goto tolua_lerror;
#endif
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		cocos2d::plugin::ProtocolSocial* socialPltform = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGamecenter"));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGooglePlay"));
#endif
		if( socialPltform )
		{
			socialPltform->showLeaderboard( ret.c_str() );
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'showLeaderboards'.",&tolua_err);
	return 0;
#endif
}


static int lua_cocos2dx_extra_PluginManager_unlockAchievement(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int argc = lua_gettop(tolua_S)-1;
		if (argc != 1)
		{
			CCLOG("unlockAchivment has wrong number of arguments: %d, was expecting %d \n", argc, 1);
		}
		ValueMap ret;
		if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
		{
#if COCOS2D_DEBUG >= 1
			goto tolua_lerror;
#endif
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		cocos2d::plugin::ProtocolSocial* socialPltform = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGamecenter"));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		socialPltform = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGooglePlay"));
#endif
		if( socialPltform )
		{
			cocos2d::plugin::TAchievementInfo dict;
			for (auto e : ret) {
				dict[e.first] = e.second.asString();
			}
			socialPltform->unlockAchievement( dict );
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'unlockAchievement'.",&tolua_err);
	return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_facebookinit(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
        class FacebookResultListener : public cocos2d::plugin::ShareResultListener
        {
        public:
            FacebookResultListener(){}
            virtual ~FacebookResultListener(){}
            void onShareResult(cocos2d::plugin::ShareResultCode code, const char* msg)
            {
                auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
                if( code == cocos2d::plugin::kLoginSuccess )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookSuccess" );
                }
                else if( code == cocos2d::plugin::kLoginFail )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookFailed" );
                }
                else if( code == cocos2d::plugin::kShareSuccess )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookShareSuccess" );
                }
                else if( code == cocos2d::plugin::kShareFail )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookShareFailed" );
                }
                else if( code == cocos2d::plugin::kGetFriendsSuccess )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookGetFriendsSuccess" );
                }
                else if( code == cocos2d::plugin::kGetFriendsFailed )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookGetFriendsFailed" );
                }
                else if( code == cocos2d::plugin::kGetInvitableFriendsSuccess )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookGetInvitableFriendsSuccess" );
                }
                else if( code == cocos2d::plugin::kGetInvitableFriendsFailed )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookGetInvitableFriendsFailed" );
                }
                else if( code == cocos2d::plugin::kInviteFriendsSuccess )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookInviteFriendsSuccess" );
                }
                else if( code == cocos2d::plugin::kInviteFriendsFailed )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookInviteFriendsFailed" );
                }
                else if( code == cocos2d::plugin::kGetAppRequestSuccess )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookGetAppRequestSuccess" );
                }
                else if( code == cocos2d::plugin::kGetAppRequestFailed )
                {
                    eventDispatcher->dispatchCustomEvent( "FaceBookGetAppRequestFailed" );
                }
				else if( code == cocos2d::plugin::kGameRequestSuccess )
				{
					eventDispatcher->dispatchCustomEvent( "FaceBookGameRequestSuccess" );
				}
				else if( code == cocos2d::plugin::kGameRequestFailed )
				{
					eventDispatcher->dispatchCustomEvent( "FaceBookGameRequestFailed" );
				}
				else if( code == cocos2d::plugin::kGameRequestCancel )
				{
					eventDispatcher->dispatchCustomEvent( "FaceBookGameRequestCancel" );
				}
				else if( code == cocos2d::plugin::kGraphRequestSuccess )
				{
					eventDispatcher->dispatchCustomEvent( "FaceBookGraphRequestSuccess" );
				}
				else if( code == cocos2d::plugin::kGraphRequestFailed )
				{
					eventDispatcher->dispatchCustomEvent( "FaceBookGraphRequestFailed" );
				}
            }
        };
        static FacebookResultListener listener;
        cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
        if( facebook )
        {
            facebook->setResultListener( &listener );
            cocos2d::plugin::TSocialDeveloperInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
            facebook->configDeveloperInfo( dict );
        }
        else
        {
            listener.onShareResult(cocos2d::plugin::kShareFail, "");
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'facebookinit'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_facebookLogout(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
		if( facebook )
		{
			facebook->callFuncWithParam( "logout" , NULL );
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'logout'.",&tolua_err);
	return 0;
#endif
}
 
static int lua_cocos2dx_extra_PluginManager_facebookDoGraphRequest(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int argc = lua_gettop( tolua_S ) - 1;
        if (3 == argc)
        {
            std::string httpMethod;
            std::string action;
            ValueMap ret;
            if (!luaval_to_std_string(tolua_S, 2, &httpMethod))
            {
#if COCOS2D_DEBUG >= 1
                goto tolua_lerror;
#endif
            }
            if (!luaval_to_std_string(tolua_S, 3, &action))
            {
#if COCOS2D_DEBUG >= 1
                goto tolua_lerror;
#endif
            }
            if (!luaval_to_ccvaluemap(tolua_S, 4, &ret))
            {
#if COCOS2D_DEBUG >= 1
                goto tolua_lerror;
#endif
            }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
            if( facebook )
            {
                cocos2d::plugin::TShareDeveloperInfo dict;
                for (auto e : ret) {
                    dict[e.first] = e.second.asString();
                }
                cocos2d::plugin::PluginParam params1( httpMethod.c_str() );
                cocos2d::plugin::PluginParam params2( action.c_str() );
                cocos2d::plugin::PluginParam params3( dict );
                facebook->callFuncWithParam( "doGraphRequest" , &params1,&params2,&params3,NULL );
            }
#endif
            return 0;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'facebookDoGraphRequest'.",&tolua_err);
    return 0;
#endif
}


static int lua_cocos2dx_extra_PluginManager_facebookSendGameRequest(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int argc = lua_gettop( tolua_S ) - 1;
        if (1 == argc)
        {
            ValueMap ret;
            if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
            {
#if COCOS2D_DEBUG >= 1
                goto tolua_lerror;
#endif
            }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
            if( facebook )
            {
                cocos2d::plugin::TShareDeveloperInfo dict;
                for (auto e : ret) {
                    dict[e.first] = e.second.asString();
                }
                cocos2d::plugin::PluginParam params( dict );
                facebook->callFuncWithParam( "sendGameRequest" , &params,NULL );
            }
#endif
            return 0;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'facebookDoGraphRequest'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_facebookLogin(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
        if( facebook )
        {
            facebook->callFuncWithParam( "login" , NULL );
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_facebookLike(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
		#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
		#endif
        }
        cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
        if( facebook )
        {
			cocos2d::plugin::TShareDeveloperInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
			cocos2d::plugin::PluginParam params( dict );
            facebook->callFuncWithParam( "Like" ,&params, NULL );
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
    return 0;
#endif
}
static int lua_cocos2dx_extra_PluginManager_facebookGetValue(lua_State* tolua_S)
{
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    argc = lua_gettop( tolua_S ) - 1;
    if (1 == argc)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        const char* param = ((const char*)  tolua_tostring(tolua_S,2,0));
        cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
        if( facebook && param )
        {
            cocos2d::plugin::PluginParam pluginParam( param );
            std::string s = facebook->callStringFuncWithParam("getValue", &pluginParam, NULL );
            tolua_pushcppstring(tolua_S,s);
            return 1;
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
    return 0;
#endif

}
static int lua_cocos2dx_extra_PluginManager_facebookLogEvent(lua_State* tolua_S)
{
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
        argc = lua_gettop( tolua_S ) - 1;
    if (1 == argc)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
        cocos2d::plugin::TShareDeveloperInfo dict;
        for (auto e : ret) {
            dict[e.first] = e.second.asString();
        }
        cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
        if( facebook )
        {
            cocos2d::plugin::PluginParam pp(dict);
            facebook->callFuncWithParam("logEvent", &pp, NULL);
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'facebookLogEvent'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_facebookShare(lua_State* tolua_S)
{
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    argc = lua_gettop( tolua_S ) - 1;
    if (1 == argc)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
        cocos2d::plugin::TShareDeveloperInfo dict;
        for (auto e : ret) {
            dict[e.first] = e.second.asString();
        }
        cocos2d::plugin::ProtocolShare* facebook = dynamic_cast<cocos2d::plugin::ProtocolShare*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("ShareFacebook"));
        if( facebook )
        {
            facebook->share( dict );
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_getGamecenterValue(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int argc = lua_gettop(tolua_S)-1;
        if (argc == 1)
        {
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            const char* param = ((const char*)  tolua_tostring(tolua_S,2,0));
            cocos2d::plugin::ProtocolSocial* gameCenter = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialGamecenter"));
            if( gameCenter && param )
            {
                cocos2d::plugin::PluginParam pluginParam( param );
                std::string s = gameCenter->callStringFuncWithParam("getValue", &pluginParam, NULL );
                tolua_pushcppstring(tolua_S,s);
                return 1;
            }
			#endif
        }
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'loginGamecenter'.",&tolua_err);
    return 0;
#endif
}
static int lua_moonton_inmobi_init(lua_State* tolua_S)
{
    
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolAds* inmobi = dynamic_cast<cocos2d::plugin::ProtocolAds*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsInmobi"));
        if( inmobi ){
            cocos2d::plugin::TAdsDeveloperInfo devInfo;
            for (auto e : ret) {
                devInfo[e.first] = e.second.asString();
            }
            inmobi->configDeveloperInfo(devInfo);
       }
#endif
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_inmobi_init'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_moonton_mta_init(lua_State* tolua_S)
{
	int argc = 0;
	bool ok  = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	argc = lua_gettop(tolua_S)-1;
	if (argc == 2)
	{
		std::string arg0;
		ok = luaval_to_std_string(tolua_S, 2,(std::string*)&arg0);
		if(!ok)
		{
			return 0;
		}
		bool isDebug;
		ok = luaval_to_boolean(tolua_S, 3,&isDebug);
		if(!ok)
		{
			return 0;
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		cocos2d::plugin::ProtocolAnalytics* _mta = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsMta"));
		if( _mta ){
			_mta->setDebugMode( isDebug );
			_mta->startSession( arg0.c_str() );
		}
#endif
		return 0;
	}
	CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsMta_trackEvent'.",&tolua_err);
#endif

	return 0;
}

static int lua_moonton_xg_init(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.xg",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		ValueMap ret;
		if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
		{
#if COCOS2D_DEBUG >= 1
			goto tolua_lerror;
#endif
		}
		/*
		class TencetServiceListener : public cocos2d::plugin::SocialListener
		{
		public:
			TencetServiceListener(){}
			virtual ~TencetServiceListener(){}
			void onSocialResult(cocos2d::plugin::SocialRetCode code, const char* msg)
			{
				auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
				if( code == cocos2d::plugin::LOGIN_SUCCESS && NULL != msg )
				{
					eventDispatcher->dispatchCustomEvent( "TencentLoginSuccess" );
				}
				if( code == cocos2d::plugin::LOGIN_FAILED || NULL == msg )
				{
					eventDispatcher->dispatchCustomEvent( "TencentLoginFailed" );
				}
			}
		};
		static TencetServiceListener listener;
		*/
		cocos2d::plugin::ProtocolUser* _xgPlugin = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushXg"));
		if( _xgPlugin )
		{
			//tencentPlugin->setListener( &listener );
			cocos2d::plugin::TUserDeveloperInfo dict;
			for (auto e : ret) {
				dict[e.first] = e.second.asString();
			}
			_xgPlugin->configDeveloperInfo( dict );
		}
		//else
		//{
		//	listener.onSocialResult(cocos2d::plugin::LOGIN_FAILED, "");
		//}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'initxg'.",&tolua_err);
	return 0;
#endif
}

static int lua_moonton_xg_getXGValue(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.xg",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int argc = lua_gettop( tolua_S ) - 1;
		if (1 == argc)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			const char* param = ((const char*)  tolua_tostring(tolua_S,2,0));
			cocos2d::plugin::ProtocolUser* _xgPlugin = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushXg"));
			if( _xgPlugin && param )
			{
				cocos2d::plugin::PluginParam pluginParam( param );
				std::string s = _xgPlugin->callStringFuncWithParam("getValue", &pluginParam, NULL );
				tolua_pushcppstring(tolua_S,s);
				return 1;
			}
#endif
			return 0;
		}
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'getXGValue'.",&tolua_err);
	return 0;
#endif
}

static int lua_moonton_HelpShift_init(lua_State* tolua_S)
{
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolAds* helpshift = dynamic_cast<cocos2d::plugin::ProtocolAds*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsHelpShift"));
        if( helpshift ){
            cocos2d::plugin::TAdsDeveloperInfo devInfo;
            for (auto e : ret) {
                devInfo[e.first] = e.second.asString();
            }
            helpshift->configDeveloperInfo(devInfo);
        }
#endif
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackEvent'.",&tolua_err);
#endif
    
    return 0;
}


static int lua_moonton_AdsChartboost_init(lua_State* tolua_S)
{
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolAds* chartboost = dynamic_cast<cocos2d::plugin::ProtocolAds*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AdsChartboost"));
        if( chartboost ){
            cocos2d::plugin::TAdsDeveloperInfo devInfo;
            for (auto e : ret) {
                devInfo[e.first] = e.second.asString();
            }
            chartboost->configDeveloperInfo(devInfo);
        }
#endif
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackEvent'.",&tolua_err);
#endif
    
    return 0;
}

int lua_moonton_Nanigans_init( lua_State* tolua_S )
{
	int argc = 0;
	bool ok  = true;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	ValueMap ret;
	if (luaval_to_ccvaluemap(tolua_S, 2, &ret))
	{	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		cocos2d::plugin::ProtocolAnalytics* nanigans = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsNanigans"));
		if( nanigans ){
			cocos2d::plugin::TShareDeveloperInfo dict;
			for (auto e : ret) {
				dict[e.first] = e.second.asString();
			}
			cocos2d::plugin::PluginParam params( dict );
			nanigans->callFuncWithParam("initNanigans", &params,NULL);
		}
#endif
		return 0;
	}
	CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackEvent'.",&tolua_err);
#endif
	return 0;
}

int lua_moonton_Nanigans_trackRevenue(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	ValueMap ret;
	if (luaval_to_ccvaluemap(tolua_S, 2, &ret))
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		cocos2d::plugin::ProtocolAnalytics* nanigans = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsNanigans"));
		if( nanigans )
		{
			cocos2d::plugin::TShareDeveloperInfo dict;
			for (auto e : ret) {
				dict[e.first] = e.second.asString();
			}
			cocos2d::plugin::PluginParam params( dict );
			nanigans->callFuncWithParam("trackRevenue", &params,NULL);
		}
#endif
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackRevenue'.",&tolua_err);
#endif
	return 0;
}
int lua_moonton_Nanigans_trackEvent(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	ValueMap ret;
	if (luaval_to_ccvaluemap(tolua_S, 2, &ret))
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		cocos2d::plugin::ProtocolAnalytics* nanigans = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsNanigans"));
		if( nanigans )
		{
			cocos2d::plugin::TShareDeveloperInfo dict;
			for (auto e : ret) {
				dict[e.first] = e.second.asString();
			}
			cocos2d::plugin::PluginParam params( dict );
			nanigans->callFuncWithParam("trackEvent", &params,NULL);
		}
#endif
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackRevenue'.",&tolua_err);
#endif
	return 0;
}

int lua_moonton_AnalyticsAdjust_init(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    argc = lua_gettop(tolua_S)-1;
    if (argc == 2)
    {
        std::string arg0;
        ok = luaval_to_std_string(tolua_S, 2,(std::string*)&arg0);
        if(!ok)
        {
            return 0;
        }
		bool isDebug;
		ok = luaval_to_boolean(tolua_S, 3,&isDebug);
		if(!ok)
		{
			return 0;
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolAnalytics* _adjust = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsAdjust"));
        if( _adjust ){
            _adjust->startSession( arg0.c_str() );
			_adjust->setDebugMode( isDebug );
        }
#endif
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackEvent'.",&tolua_err);
#endif
    
    return 0;
}
int lua_moonton_AnalyticsAdjust_trackRevenue(lua_State* tolua_S)
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    ValueMap ret;
    if (luaval_to_ccvaluemap(tolua_S, 2, &ret))
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolAnalytics* _adjust = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsAdjust"));
        if( _adjust )
        {
            cocos2d::plugin::TShareDeveloperInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
            cocos2d::plugin::PluginParam params( dict );
            _adjust->callFuncWithParam("trackRevenue", &params,NULL);
        }
#endif
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackRevenue'.",&tolua_err);
#endif
    return 0;
}
int lua_moonton_AnalyticsAdjust_trackEvent(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        std::string arg0;
        ok = luaval_to_std_string(tolua_S, 2,(std::string*)&arg0);
        if(!ok)
        {
            return 0;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolAnalytics* _adjust = dynamic_cast<cocos2d::plugin::ProtocolAnalytics*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsAdjust"));
        if( _adjust ){
            _adjust->logEvent( arg0.c_str() );
        }
#endif
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "trackEvent",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackEvent'.",&tolua_err);
#endif
    
    return 0;
}
int lua_moonton_HelpShift_showFAQ(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolAds* _helpShift = dynamic_cast<cocos2d::plugin::ProtocolAds*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsHelpShift"));
    if( _helpShift ){
        cocos2d::plugin::TAdsInfo adsInfo;
		ValueMap ret;
		if (luaval_to_ccvaluemap(tolua_S, 2, &ret)){
			for (auto e : ret) {
				adsInfo[e.first] = e.second.asString();
			}
		}
        _helpShift->showAds( adsInfo );
    }
#endif
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_AnalyticsAdjust_trackEvent'.",&tolua_err);
#endif
    return 0;
}
int lua_moonton_HelpShift_registerDeviceToken(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	std::string token;
    luaval_to_std_string(tolua_S, 2 ,&token);

    cocos2d::plugin::ProtocolAds* _helpShift = dynamic_cast<cocos2d::plugin::ProtocolAds*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AnalyticsHelpShift"));
    if( _helpShift ){
		cocos2d::plugin::PluginParam params(token.c_str());
        _helpShift->callFuncWithParam("registerDeviceToken", &params, NULL );
    }
#endif
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_moonton_HelpShift_registerDeviceToken'.",&tolua_err);
#endif
    return 0;
}
int lua_cocos2dx_extra_PluginManager_initPushNotifacation( lua_State* tolua_S )
{   
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
    if( _pushNotifacation )
    {
        class PushNotifacationResultListener : public cocos2d::plugin::UserActionListener
        {
        public:
            PushNotifacationResultListener(){}
            virtual ~PushNotifacationResultListener(){}
            void onActionResult(cocos2d::plugin::ProtocolUser* pPlugin, cocos2d::plugin::UserActionResultCode code, const char* msg)
            {
                if( std::string(pPlugin->getPluginName()) == "PushNotifacation" )
                {
                    auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
                    if( code == cocos2d::plugin::kUserType_1 )
                    {
                        eventDispatcher->dispatchCustomEvent( "ReceiveNotifacationToken" );
                    }
                    if( code == cocos2d::plugin::kUserType_2 )
                    {
                        eventDispatcher->dispatchCustomEvent( "ReceiveLocalNotifacation" );
                    }
                    if( code == cocos2d::plugin::kUserType_3 )
                    {
                        eventDispatcher->dispatchCustomEvent( "ReceiveRemoteNotifacation" );
                    }
                }
            }
        };
        static PushNotifacationResultListener listener;
        cocos2d::plugin::TUserDeveloperInfo info;
		ValueMap ret;
		if (luaval_to_ccvaluemap(tolua_S, 2, &ret))
		{
			cocos2d::plugin::TShareDeveloperInfo dict;
			for (auto e : ret) {
				info[e.first] = e.second.asString();
			}
		}
        info["placeHolder"] = "YES";
        _pushNotifacation->configDeveloperInfo( info );
        _pushNotifacation->setActionListener( &listener );
    }
#endif
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_addPushNotifacation'.",&tolua_err);
#endif
    return 0;
}
int lua_cocos2dx_extra_PluginManager_addPushNotifacation( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    ValueMap ret;
    if (luaval_to_ccvaluemap(tolua_S, 2, &ret))
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
        if( _pushNotifacation )
        {
            cocos2d::plugin::TShareDeveloperInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
            cocos2d::plugin::PluginParam params( dict );
            _pushNotifacation->callFuncWithParam("createPushNotifacation", &params,NULL);
        }
#endif
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_addPushNotifacation'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_extra_PluginManager_removePushNotifacation( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    ValueMap ret;
    if (luaval_to_ccvaluemap(tolua_S, 2, &ret))
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
        if( _pushNotifacation )
        {
            cocos2d::plugin::PluginParam notifationName( ret["name"].asString().c_str() );
            _pushNotifacation->callFuncWithParam("removePushNotifacation", &notifationName , NULL);
        }
#endif
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}
int lua_cocos2dx_extra_PluginManager_getPushToken( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
    if( _pushNotifacation )
    {
        std::string token = _pushNotifacation->callStringFuncWithParam("getDeviceToken" , NULL);
        tolua_pushcppstring(tolua_S,token);
        return 1;
    }
#endif
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_extra_PluginManager_setBadgeNumber( lua_State* tolua_S )
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    int ret;
    if (luaval_to_int32(tolua_S, 2, &ret))
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
        if( _pushNotifacation )
        {
            cocos2d::plugin::PluginParam notifationName( ret );
            _pushNotifacation->callFuncWithParam("setBadgeNumber", &notifationName , NULL);
        }
#endif
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_extra_PluginManager_refreshLocalNotifacationBadgeNumber( lua_State* tolua_S )
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
    if( _pushNotifacation )
    {
        _pushNotifacation->callFuncWithParam("refreshLocalNotifacationBadgeNumber" , NULL);
    }
#endif
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_extra_PluginManager_removeAllNotifacation( lua_State* tolua_S )
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
    if( _pushNotifacation )
    {
        _pushNotifacation->callFuncWithParam("removeAllNotifacation" , NULL);
    }
#endif
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_extra_PluginManager_removeTimecloseNotifacation( lua_State* tolua_S )
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    int ret;
    if (luaval_to_int32(tolua_S, 2, &ret))
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
        if( _pushNotifacation )
        {
            cocos2d::plugin::PluginParam notifationName( ret );
            _pushNotifacation->callFuncWithParam("removeTimecloseNotifacation", &notifationName , NULL);
        }
#endif
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_extra_PluginManager_getLaunchNotification( lua_State* tolua_S )
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
    if( _pushNotifacation )
    {
        std::string ret = _pushNotifacation->callStringFuncWithParam("getLaunchNotification" , NULL);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
#endif
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_extra_PluginManager_getCurrentNotification( lua_State* tolua_S )
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
    if( _pushNotifacation )
    {
        std::string ret = _pushNotifacation->callStringFuncWithParam("getCurrentNotification" , NULL);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
#endif
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}
int lua_cocos2dx_extra_PluginManager_clearRemoteNotifacation( lua_State* tolua_S )
{
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    cocos2d::plugin::ProtocolUser* _pushNotifacation = dynamic_cast<cocos2d::plugin::ProtocolUser*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("PushNotifacation"));
    if( _pushNotifacation )
    {
        _pushNotifacation->callFuncWithParam("clearRemoteNotifacation" , NULL);
        return 0;
    }
#endif
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_extra_PluginManager_removePushNotifacation'.",&tolua_err);
#endif
    return 0;
}

cocos2d::plugin::ProtocolIAP* getProtocolIAP()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	string pluginName = cocos2d::plugin::PluginManager::getInstance()->getPluginName("ProtocolIAP");
	if( pluginName.length() == 0 )
	{
		pluginName = "SKPay";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		pluginName = "IABGooglePlay";
#endif
	}
    cocos2d::plugin::ProtocolIAP* iap = dynamic_cast<cocos2d::plugin::ProtocolIAP*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin(pluginName.c_str()));
    return iap;
#else
	return NULL;
#endif
}

int lua_cocos2dx_extra_PluginManager_iapServiceInit( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int argc = lua_gettop(tolua_S)-1;
        ///!用来保存没有发送完毕的收据的键值
        std::string identifier;
        std::string accessGroup;
        std::string publicKey;
        std::string zoneIndex;
        std::string accountIdx;
        if( argc != 3 && argc != 2 )
        {
            CCLOG("iapServiceInit has wrong number of arguments: %d, was expecting %d \n", argc, 2);
        }
        ///!苹果的支付初始化参数
		if (argc == 2)
		{
            luaval_to_std_string(tolua_S, 2,&identifier);
            luaval_to_std_string(tolua_S, 3,&accessGroup);
		}
        ///!google的支付初始化参数
        if( argc == 3 )
        {
            luaval_to_std_string(tolua_S, 2,&publicKey);
            luaval_to_std_string(tolua_S, 3,&zoneIndex);
            luaval_to_std_string(tolua_S, 4,&accountIdx);
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		class SKPayResultListener : public cocos2d::plugin::PayResultListener
		{
		public:
			SKPayResultListener() { }
			virtual ~SKPayResultListener(){}
			virtual void onPayResult(cocos2d::plugin::PayResultCode ret, const char* msg, cocos2d::plugin::TProductInfo info)
			{
				auto eventDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
				if( ret == cocos2d::plugin::kPaySuccess )
				{
					eventDispatcher->dispatchCustomEvent( "PaySuccess");
				}
				else if( ret == cocos2d::plugin::kPayCancel)
				{
					eventDispatcher->dispatchCustomEvent( "PayCancel" );
				}
				else if( ret == cocos2d::plugin::kPayFail || ret == cocos2d::plugin::kPayTimeOut )
				{
					eventDispatcher->dispatchCustomEvent( "PayFail");
				}
                else if( ret == cocos2d::plugin::GetProductsInfoSuccess )
                {
                    eventDispatcher->dispatchCustomEvent( "GetProductsInfoSuccess");
                }
                else if( ret == cocos2d::plugin::GetProductsInfoFailed )
                {
                    eventDispatcher->dispatchCustomEvent( "GetProductsInfoFailed");
                }
				else if( ret == cocos2d::plugin::kPayConsumeSuccess )
				{
					eventDispatcher->dispatchCustomEvent( "PayComsumeSuccess");
				}
                else if( ret == cocos2d::plugin::kPayConsumeFailed )
                {
                    eventDispatcher->dispatchCustomEvent( "PayComsumeFailed");
                }
			}
		};
        cocos2d::plugin::ProtocolIAP* iap = getProtocolIAP();
		static SKPayResultListener listener;
		if( iap )
		{
			iap->setResultListener( &listener );
			cocos2d::plugin::TIAPDeveloperInfo iapParam;
			iapParam["identifier"] = identifier;
			iapParam["accessGroup"] = accessGroup;
            iapParam["publicKey"] = publicKey;
            iapParam["zoneID"] = zoneIndex;
            iapParam["accountID"] = accountIdx;
			iap->configDeveloperInfo( iapParam );
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'iapServiceInit'.",&tolua_err);
	return 0;
#endif
}
int lua_cocos2dx_extra_PluginManager_iapGetProductsInfomation( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		int argc = lua_gettop(tolua_S)-1;
		if (argc != 1)
		{
			CCLOG("iapGetProductsInfomation has wrong number of arguments: %d, was expecting %d \n", argc, 1);
		}
		std::string products;
		luaval_to_std_string(tolua_S, 2,&products);
        cocos2d::plugin::ProtocolIAP* iap = getProtocolIAP();
        if( iap )
		{
            cocos2d::plugin::PluginParam param(products.c_str());
            iap->callFuncWithParam("getProductsInformation", &param , NULL );
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'iapGetProductsInfomation'.",&tolua_err);
	return 0;
#endif
}

int lua_cocos2dx_extra_PluginManager_iapGetValue( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		int argc = lua_gettop(tolua_S)-1;
		if (argc != 1)
		{
			CCLOG("iapGetValue has wrong number of arguments: %d, was expecting %d \n", argc, 1);
		}
		std::string key;
		luaval_to_std_string(tolua_S, 2,&key);
		cocos2d::plugin::ProtocolIAP* iap = getProtocolIAP();
		if( iap )
		{
            cocos2d::plugin::PluginParam param(key.c_str());
            std::string s = iap->callStringFuncWithParam("getValue", &param , NULL);
            tolua_pushcppstring(tolua_S, s);
            return 1;
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'iapGetValue'.",&tolua_err);
	return 0;
#endif
}

int lua_cocos2dx_extra_PluginManager_iapBuyProduct( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		ValueMap ret;
		if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
		{
#if COCOS2D_DEBUG >= 1
			goto tolua_lerror;
#endif
		}
        cocos2d::plugin::ProtocolIAP* iap = getProtocolIAP();
		if( iap )
		{
			cocos2d::plugin::TProductInfo dict;
			for (auto e : ret) {
				dict[e.first] = e.second.asString();
			}
            iap->payForProduct( dict );
		}
#endif
		return 0;
	}
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'iapBuyProduct'.",&tolua_err);
	return 0;
#endif
}

int lua_cocos2dx_extra_PluginManager_iapGetNoneVerifiedReceipt( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        cocos2d::plugin::ProtocolIAP* iap = getProtocolIAP();
        if( iap )
        {
            std::string s = iap->callStringFuncWithParam("getNoneVerifiedReceipt" , NULL);
            tolua_pushcppstring(tolua_S, s);
        }
#endif
        return 1;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'iapBuyProduct'.",&tolua_err);
    return 0;
#endif
}

int lua_cocos2dx_extra_PluginManager_removeNoneVerifiedReceipt( lua_State* tolua_S )
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        int argc = lua_gettop(tolua_S)-1;
        if (argc != 1 )
        {
            CCLOG("removeNoneVerifiedReceipt has wrong number of arguments: %d, was expecting %d \n", argc, 1);
        }
        std::string key;
        luaval_to_std_string(tolua_S, 2,&key);
        cocos2d::plugin::ProtocolIAP* iap = getProtocolIAP();
        if( iap )
        {
            cocos2d::plugin::PluginParam param(key.c_str());
            iap->callFuncWithParam("removeNoneVerifiedReceipt",&param,NULL);
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'iapBuyProduct'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_SafeLogin(lua_State* tolua_S)
{
	#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ValueMap ret;
        if (!luaval_to_ccvaluemap(tolua_S, 2, &ret))
        {
#if COCOS2D_DEBUG >= 1
            goto tolua_lerror;
#endif
        }
		std::string platfromjavaFuncName;
        luaval_to_std_string(tolua_S, 3,&platfromjavaFuncName);

        cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
        if( tencentPlugin )
        {
            cocos2d::plugin::TSocialSafeInfo dict;
            for (auto e : ret) {
                dict[e.first] = e.second.asString();
            }
			cocos2d::plugin::PluginParam params( dict );
			tencentPlugin->callFuncWithParam(platfromjavaFuncName.c_str(), &params, NULL );
        }
#endif
        return 0;
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'SafeLogin'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_extra_PluginManager_sendDataReceivedFromGameSvr(lua_State* tolua_S)
{

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"cc.PluginManager",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        int argc = lua_gettop( tolua_S ) - 1;
        if (1 == argc)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            const char* param = ((const char*)  tolua_tostring(tolua_S,2,0));
            cocos2d::plugin::ProtocolSocial* tencentPlugin = dynamic_cast<cocos2d::plugin::ProtocolSocial*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("SocialTencent"));
            if( tencentPlugin && param )
            {
                cocos2d::plugin::PluginParam pluginParam( param );
                std::string s = tencentPlugin->callStringFuncWithParam("sendDataReceivedFromSvr", &pluginParam, NULL );
                tolua_pushcppstring(tolua_S,s);
                return 1;
            }
#endif
            return 0;
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
        tolua_error(tolua_S,"#ferror in function 'sendDataReceivedFromGameSvr'.",&tolua_err);
        return 0;
#endif
}

int lua_register_cocos2dx_extra_plugin(lua_State* L)
{
    tolua_usertype(L,"cc.PluginManager");
    tolua_cclass(L,"PluginManager","cc.PluginManager","",nullptr);
    tolua_beginmodule(L,"PluginManager");
    
		tolua_function(L,"setPluginName",lua_cocos2dx_extra_PluginManager_setPluginName);

        tolua_function(L,"loginGamecenter",lua_cocos2dx_extra_PluginManager_loginGamecenter);
        tolua_function(L,"getGamecenterValue",lua_cocos2dx_extra_PluginManager_getGamecenterValue);
    
        tolua_function(L,"facebookinit",lua_cocos2dx_extra_PluginManager_facebookinit);
        tolua_function(L,"facebookLogin",lua_cocos2dx_extra_PluginManager_facebookLogin);
		tolua_function(L,"facebookLogout",lua_cocos2dx_extra_PluginManager_facebookLogout);
        tolua_function(L,"facebookLike",lua_cocos2dx_extra_PluginManager_facebookLike);
        tolua_function(L,"facebookShare",lua_cocos2dx_extra_PluginManager_facebookShare);
        tolua_function(L,"facebookShareSilence",lua_cocos2dx_extra_PluginManager_facebookShare);
        tolua_function(L,"facebookGetValue",lua_cocos2dx_extra_PluginManager_facebookGetValue);
        tolua_function(L,"facebookLogEvent",lua_cocos2dx_extra_PluginManager_facebookLogEvent);
        tolua_function(L,"facebookDoGraphRequest",lua_cocos2dx_extra_PluginManager_facebookDoGraphRequest);
        tolua_function(L,"facebookSendGameRequest",lua_cocos2dx_extra_PluginManager_facebookSendGameRequest);
    
        tolua_function(L,"initPushNotifacation",lua_cocos2dx_extra_PluginManager_initPushNotifacation);
        tolua_function(L,"addPushNotifacation",lua_cocos2dx_extra_PluginManager_addPushNotifacation);
        tolua_function(L,"removePushNotifacation",lua_cocos2dx_extra_PluginManager_removePushNotifacation);
        tolua_function(L,"getPushToken",lua_cocos2dx_extra_PluginManager_getPushToken);
        tolua_function(L,"setBadgeNumber",lua_cocos2dx_extra_PluginManager_setBadgeNumber);
        tolua_function(L,"refreshLocalNotifacationBadgeNumber",lua_cocos2dx_extra_PluginManager_refreshLocalNotifacationBadgeNumber);
        tolua_function(L,"removeTimecloseNotifacation",lua_cocos2dx_extra_PluginManager_removeTimecloseNotifacation);
        tolua_function(L,"removeAllNotifacation",lua_cocos2dx_extra_PluginManager_removeAllNotifacation);
    
        tolua_function(L,"getLaunchNotification",lua_cocos2dx_extra_PluginManager_getLaunchNotification);
        tolua_function(L,"getCurrentNotification",lua_cocos2dx_extra_PluginManager_getCurrentNotification);
        tolua_function(L,"clearRemoteNotifacation",lua_cocos2dx_extra_PluginManager_clearRemoteNotifacation);

		tolua_function(L, "iapServiceInit", lua_cocos2dx_extra_PluginManager_iapServiceInit);
		tolua_function(L, "iapGetProductsInfomation", lua_cocos2dx_extra_PluginManager_iapGetProductsInfomation);
		tolua_function(L, "iapGetValue", lua_cocos2dx_extra_PluginManager_iapGetValue);
		tolua_function(L, "iapBuyProduct", lua_cocos2dx_extra_PluginManager_iapBuyProduct);
        tolua_function(L, "iapGetNoneVerifiedReceipt", lua_cocos2dx_extra_PluginManager_iapGetNoneVerifiedReceipt);
        tolua_function(L, "removeNoneVerifiedReceipt", lua_cocos2dx_extra_PluginManager_removeNoneVerifiedReceipt);
    
        tolua_function(L,"loginGooglePlay",lua_cocos2dx_extra_PluginManager_loginGooglePlay);
        tolua_function(L,"logoutGooglePlay",lua_cocos2dx_extra_PluginManager_logoutGooglePlay);
        tolua_function(L,"getGooglePlayAccountInfo",lua_cocos2dx_extra_PluginManager_getGooglePlayAccountInfo);
		tolua_function(L,"unlockAchievement",lua_cocos2dx_extra_PluginManager_unlockAchievement);
		tolua_function(L,"setLeaderboards",lua_cocos2dx_extra_PluginManager_setLeaderboards);
		tolua_function(L,"showAchievements",lua_cocos2dx_extra_PluginManager_showAchievements);
		tolua_function(L,"showLeaderboards",lua_cocos2dx_extra_PluginManager_showLeaderboards);
    
        tolua_function(L,"initTencent",lua_cocos2dx_extra_PluginManager_initTencent);
        tolua_function(L,"logoutTencent",lua_cocos2dx_extra_PluginManager_logoutTencent);
        tolua_function(L,"getTencentValue",lua_cocos2dx_extra_PluginManager_getTencentValue);
        tolua_function(L,"loginTencent",lua_cocos2dx_extra_PluginManager_loginTencent);
		tolua_function(L,"queryQQGameFriendsInfo",lua_cocos2dx_extra_PluginManager_queryQQGameFriendsInfo);
		tolua_function(L,"sendToQQ",lua_cocos2dx_extra_PluginManager_sendToQQ);
		tolua_function(L,"shareBigPicture",lua_cocos2dx_extra_PluginManager_shareBigPicture);
		tolua_function(L,"sendToQQGameFriend",lua_cocos2dx_extra_PluginManager_sendToQQGameFriend);
   
		tolua_function(L,"SafeLogin",lua_cocos2dx_extra_PluginManager_SafeLogin);
		tolua_function(L,"sendDataReceivedFromGameSvr",lua_cocos2dx_extra_PluginManager_sendDataReceivedFromGameSvr);
    tolua_endmodule(L);
    
    
    tolua_usertype(L,"cc.AnalyticsAdjust");
    tolua_cclass(L,"AnalyticsAdjust","cc.AnalyticsAdjust","",nullptr);
    tolua_beginmodule(L,"AnalyticsAdjust");
        tolua_function(L,"init",lua_moonton_AnalyticsAdjust_init);
        tolua_function(L,"trackEvent",lua_moonton_AnalyticsAdjust_trackEvent);
        tolua_function(L,"trackRevenue",lua_moonton_AnalyticsAdjust_trackRevenue);
    tolua_endmodule(L);
    
    tolua_usertype(L,"cc.AdsChartboost");
    tolua_cclass(L,"AdsChartboost","cc.AdsChartboost","",nullptr);
    tolua_beginmodule(L,"AdsChartboost");
        tolua_function(L,"init",lua_moonton_AdsChartboost_init);
    tolua_endmodule(L);
    
    tolua_usertype(L,"cc.HelpShift");
    tolua_cclass(L,"HelpShift","cc.HelpShift","",nullptr);
    tolua_beginmodule(L,"HelpShift");
		tolua_function(L,"init",lua_moonton_HelpShift_init);
        tolua_function(L,"showFAQ",lua_moonton_HelpShift_showFAQ);
		tolua_function(L,"registerDeviceToken",lua_moonton_HelpShift_registerDeviceToken);
    tolua_endmodule(L);
    
    tolua_usertype(L,"cc.inmobi");
    tolua_cclass(L,"inmobi","cc.inmobi","",nullptr);
    tolua_beginmodule(L,"inmobi");
        tolua_function(L,"init",lua_moonton_inmobi_init);
    tolua_endmodule(L);

	tolua_usertype(L,"cc.mta");
	tolua_cclass(L,"mta","cc.mta","",nullptr);
	tolua_beginmodule(L,"mta");
	tolua_function(L,"init",lua_moonton_mta_init);
	tolua_endmodule(L);

	tolua_usertype(L,"cc.xg");
	tolua_cclass(L,"xg","cc.xg","",nullptr);
	tolua_beginmodule(L,"xg");
	tolua_function(L,"init",lua_moonton_xg_init);
	tolua_function(L,"getXGValue",lua_moonton_xg_getXGValue);
	tolua_endmodule(L);

	tolua_usertype(L,"cc.Nanigans");
    tolua_cclass(L,"Nanigans","cc.Nanigans","",nullptr);
    tolua_beginmodule(L,"Nanigans");
        tolua_function(L,"init",lua_moonton_Nanigans_init);
        tolua_function(L,"trackEvent",lua_moonton_Nanigans_trackEvent);
        tolua_function(L,"trackRevenue",lua_moonton_Nanigans_trackRevenue);
    tolua_endmodule(L);
    
    return 1;
}

int luaopen_cocos2dx_extra_luabinding(lua_State* L)
{
    if (nullptr == L)
        return 0;
	tolua_module(L,"cc",0);
	tolua_beginmodule(L,"cc");
		lua_register_cocos2dx_extra_native( L );
		lua_register_cocos2dx_extra_network( L );
        lua_register_cocos2dx_extra_plugin( L );
	tolua_endmodule(L);
    return 0;
}
