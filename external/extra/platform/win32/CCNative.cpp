
#include "../../native/CCNative.h"

NS_CC_EXTRA_BEGIN

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Netapi32.lib")

#define MACSESION 6
	typedef struct _ASTAT_ 
{ 
	ADAPTER_STATUS adapt; 
	NAME_BUFFER NameBuff [30]; 
}ASTAT, * PASTAT; 

ASTAT Adapter; // to store MAC information 
std::string getmac_one( int lana_num )
{ 
	NCB ncb; 
	UCHAR uRetCode; 

	memset( &ncb, 0, sizeof( ncb ) ); 
	ncb.ncb_command = NCBRESET; 
	ncb.ncb_lana_num = lana_num; 

	uRetCode = Netbios( &ncb ); 
	//printf( "The NCBRESET return code is:0x%x \n", uRetCode ); 

	memset( &ncb, 0, sizeof( ncb ) ); 
	ncb.ncb_command = NCBASTAT; 
	ncb.ncb_lana_num = lana_num; 

	strcpy( (char *)ncb.ncb_callname, "* " ); 
	ncb.ncb_buffer = ( unsigned char * ) &Adapter; 

	ncb.ncb_length = sizeof( Adapter ); 
	uRetCode = Netbios( &ncb ); 
	// printf( "The NCBASTAT return code is: 0x%x \n", uRetCode );
	std::string s;
	if ( uRetCode == 0 ) 
	{
		int bAddressInt [ MACSESION ];
		char CommarSeperatedAddress[ MACSESION * 3 ]={0};
		for( int i = 0; i < MACSESION; ++i )
		{
			bAddressInt[ i ] = Adapter.adapt.adapter_address[ i ];
			bAddressInt[ i ] &= 0x000000ff; // avoid "ff" leading bytes when "char" is lager then 0x7f
		}
		sprintf( CommarSeperatedAddress, "%02x:%02x:%02x:%02x:%02x:%02x",
			bAddressInt[ 0 ],
			bAddressInt[ 1 ],
			bAddressInt[ 2 ],
			bAddressInt[ 3 ],
			bAddressInt[ 4 ],
			bAddressInt[ 5 ]); // Should use scl::FormatString inside 
		s = CommarSeperatedAddress;
	}
	return s;
}
std::string GetNetBiosMacAddresses()
{
	NCB ncb; 
	UCHAR uRetCode; 
	LANA_ENUM lana_enum; 
	memset( &ncb, 0, sizeof( ncb ) ); 
	ncb.ncb_command = NCBENUM; 

	ncb.ncb_buffer = (unsigned char *) &lana_enum; 
	ncb.ncb_length = sizeof( lana_enum ); 

	uRetCode = Netbios( &ncb ); 
	//printf( "The NCBENUM return code is:0x%x \n", uRetCode );
	std::string vAdd;
	if ( uRetCode == 0 ) 
	{ 
		//printf( "Ethernet Count is : %d\n\n", lana_enum.length); 
		for ( int i=0; i < lana_enum.length; ++i )
		{
			std::string s = getmac_one( lana_enum.lana[i] ); 
			if( ! s.empty() )
			{
				vAdd.append( s );
			}
		}
	}
	return vAdd;
}
const std::string CCNative::getUniqeID(void)
{
	return GetNetBiosMacAddresses();
}
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
		//GUID uuid; 
		//CoCreateGuid(&uuid); 
		//// Spit the address out 
		//char mac_addr[18]; 
		//sprintf(mac_addr,"%02X:%02X:%02X:%02X:%02X:%02X", 
		//uuid.Data4[2],uuid.Data4[3],uuid.Data4[4], 
		//uuid.Data4[5],uuid.Data4[6],uuid.Data4[7]); 
		udid = GetNetBiosMacAddresses();
		if( userDefault )
		{
			userDefault->setStringForKey(UDID_KEY, udid);
            userDefault->flush();
		}
	}
	return udid;
}

const std::string CCNative::getIDForVendor(void)
{
    return getDeviceID();
}

const std::string CCNative::getDeviceName(void)
{
    return "Windows";
}
const string CCNative::getOSVersion()
{
	return "Windows";
}
void CCNative::openURL(const char* url)
{
	WCHAR urlWideChar[ 256 ] = { 0 };
	MultiByteToWideChar( CP_ACP,0,url,strlen(url)+1,urlWideChar, sizeof(urlWideChar)/sizeof(urlWideChar[0]) );
	ShellExecute(NULL, L"open", urlWideChar, NULL, NULL, SW_SHOWNORMAL);
}

string CCNative::getCurrentLanguage()
{
    return "en";
}

unsigned int CCNative::getTotalMem()
{
	return 1024 * 4;
}
unsigned int CCNative::getFreeMem()
{
	return 1024 * 4;
}
string CCNative::getDeviceExtraInformation()
{
	return "";
}
string CCNative::getAndroidID()
{
	return "";
}
string CCNative::getTelephonyDeviceID()
{
	return "";
}
string CCNative::getMacAddress()
{
	return "";
}
string CCNative::getAdvertiseID()
{
	return "";
}
#endif

NS_CC_EXTRA_END
