

#include "../protocols/include/ProtocolSocial.h"

namespace cocos2d { namespace plugin{
    class SocialGooglePlayHelper{
    public:
        static void setGooglePlayAccountInfo( std::string const& );
        static std::string const& getGooglePlayAccountInfo();
    };
}}

