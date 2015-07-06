
#include "SocialGooglePlay.h"

namespace cocos2d { namespace plugin{
    std::string staticGoogleAccountInfo;
    void SocialGooglePlayHelper::setGooglePlayAccountInfo( std::string const& s){
        staticGoogleAccountInfo = s;
    }
    std::string const& SocialGooglePlayHelper::getGooglePlayAccountInfo(){
        return staticGoogleAccountInfo;
    }
}}