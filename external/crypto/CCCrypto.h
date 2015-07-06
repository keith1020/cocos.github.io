
#ifndef __CC_EXTENSION_CCCRYPTO_H_
#define __CC_EXTENSION_CCCRYPTO_H_

#include "cocos2d.h"
#include <string>

#ifdef _LZMA_NO_SYSTEM_SIZE_T
typedef UInt32 SizeT;
#else
typedef size_t SizeT;
#endif

//#include "CCLuaValue.h"

//#define LUA_ENCRYPT // 是否开启lua加密
#ifdef LUA_ENCRYPT
#define LUA_EXTENSION ".mt"	//add by jonyu
#else
#define LUA_EXTENSION ".lua" //add by jonyu
#endif
USING_NS_CC;

class CC_DLL CCCrypto
{
public:
    static const int MD5_BUFFER_LENGTH = 16;
    

    /** @brief Get length of encoding data with Base64 algorithm */
    static int encodeBase64Len(const char* input, int inputLength);
    
    /** @brief Encoding data with Base64 algorithm, return encoded string length */
    static int encodeBase64(const char* input, int inputLength,
                            char* output, int outputBufferLength);
    
    /** @brief Get length of Decoding Base 64 */
    static int decodeBase64Len(const char* input);

    /** @brief Decoding Base64 string to data, return decoded data length */
    static int decodeBase64(const char* input,
                            char* output, int outputBufferLength);
    
    /** @brief Calculate MD5, get MD5 code (not string) */
    static void MD5(void* input, unsigned long inputLength,
                    unsigned char* output);
    
    static const std::string MD5File(const char* path);    

    static const std::string MD5String(void* input, int inputLength);

	static std::string aes_encrypt(const std::string &sData, const std::string &sKey);
	static std::string aes_decrypt(const std::string &sData, const std::string &sKey);

	static bool aes_encrypt(const std::string &sData, const std::string &sKey, std::string &sResult);
	static bool aes_decrypt(const std::string &sData, const std::string &sKey, std::string &sResult);
	static bool aes_encrypt(const char *pBegin, const char *pEnd, const std::string &sKey, std::string &sResult);
	static bool aes_decrypt(const char *pBegin, const char *pEnd, const std::string &sKey, std::string &sResult);

	static bool aes_decrypt(const char *pBegin, const char *pEnd, const std::string &sKey ,char* pResult,int iBufferSize);
 
	static void xor_encrypt(const char* pBegin, unsigned int nSize, std::string &sResult);
	static void xor_decrypt(char* pBegin, unsigned int nSize );
 
#pragma mark -
#pragma mark for Lua
   
    ///** @brief Encoding data with Base64 algorithm, return encoded string */
    //static int encodeBase64Lua(const char* input, int inputLength)
    //{
    //    return encodingBase64Lua(false, input, inputLength);
    //}
    //
    ///** @brief Decoding Base64 string to data, return decoded data length */
    //static int decodeBase64Lua(const char* input)
    //{
    //    return encodingBase64Lua(true, input, (int)strlen(input));
    //}
    
    /** @brief Calculate MD5, return MD5 string */
    static int MD5Lua(char* input, unsigned long inputLength, bool isRawOutput);

	static int getLuaExtension();   
    static int MD5FileLua(const char* path);

	static void setKey(std::string sKey);
	static std::string getKey();

	static bool compressData( const unsigned char* pInData, unsigned int nInSize, unsigned char* pOutData, unsigned int& nOutSize );
	static bool uncompressData( char* pInData, unsigned int nInSize, unsigned char** pOutData, SizeT& nOutSize );//pOutData在函数内malloc，由调用者负责free
    
#pragma mark -
#pragma mark private methods
    
private:
    CCCrypto(void) {}
    
    //static int encodingBase64Lua(bool isDecoding,
    //                                    const char* input,
    //                                    int inputLength);
    
    static char* bin2hex(unsigned char* bin, int binLength);
	static std::string fromHex(const std::string &s);

	typedef void (*block_encrypt_func)(void *ctx, const void *input, void *output);
	typedef void (*block_decrypt_func)(void *ctx, const void *input, void *output);

	static bool cbc_encrypt(const char *pBegin, const char *pEnd, block_encrypt_func encrypt_func, int iBlockSize, void *ctx, std::string &sResult);
	static bool cbc_decrypt(const char *pBegin, const char *pEnd, block_decrypt_func decrypt_func, int iBlockSize, void *ctx, std::string &sResult);
	static bool cbc_decrypt(const char *pBegin, const char *pEnd, block_decrypt_func decrypt_func, int iBlockSize, void *ctx,char* pResult,int iBufferSize);

	static bool cbc_encrypt(const std::string &sData, block_encrypt_func encrypt_func, int iBlockSize, void *ctx, std::string &sResult);
	static bool cbc_decrypt(const std::string &sData, block_decrypt_func decrypt_func, int iBlockSize, void *ctx, std::string &sResult);
	static std::string m_sKey;
};

#define ENCRYPT_AES	1
#define ENCRYPT_XOR	2

#pragma pack(1)
struct EncryedHeader
{
	unsigned int	m_sign;			// 'mtnA'
	unsigned char	m_nType;		// 1 AES, 2 xor
	unsigned char	m_nRandomValue1;// useless
	unsigned char	m_nVersion;		// 1
	unsigned char	m_nRandomValue2;
	unsigned int	m_nDecryptSize;	// ^0x00ABCDEF
	unsigned int	m_nReserved;	// 0 for now

	bool isEncryed() const
	{
		return m_sign == 'mtnA';
	}
	void setupHeader( unsigned char encryptType, unsigned int nDecryptSize )
	{
		m_sign = 'mtnA';
		m_nType = encryptType;
		m_nRandomValue1 = rand()%256;
		m_nRandomValue2 = rand()%256;
		m_nVersion = 1;
		m_nDecryptSize = nDecryptSize^0x00ABCDEF;
		m_nReserved = 0;
	}
	unsigned int getDecryptSize()
	{
		return m_nDecryptSize^0x00ABCDEF;
	}
};
#pragma pop

#endif // __CC_EXTENSION_CCCRYPTO_H_
