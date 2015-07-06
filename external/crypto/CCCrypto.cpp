
#include "CCCrypto.h"

extern "C" {
#include "base64/libbase64.h"
#include "md5/md5.h"
#include "aes/aes.h"
#include "lzma/7zCrc.h"
#include "lzma/LzmaEnc.h"
#include "lzma/LzmaDec.h"
}
#include <vector>
#include <algorithm>

const char* _getKey()
{
	const char* p = "f0a193d50ade553e9835595f5cd75ddd";
	static char sztemp[64] = {0};
	strcpy( sztemp, p );
	sztemp[1] += 5;
	return sztemp;
}

//std::string CCCrypto::m_sKey = CCCrypto::fromHex("f5a193d50ade553e9835595f5cd75ddd");
std::string CCCrypto::m_sKey = CCCrypto::fromHex(_getKey());
//NS_CC_EXTRA_BEGIN

int CCCrypto::encodeBase64Len(const char* input, int inputLength)
{
    return Base64encode_len(inputLength);
}

int CCCrypto::encodeBase64(const char* input,
                           int inputLength,
                           char* output,
                           int outputBufferLength)
{
    CCAssert(Base64encode_len(inputLength) <= outputBufferLength, "CCCrypto::encodeBase64() - outputBufferLength too small");
    return Base64encode(output, input, inputLength);
}

int CCCrypto::decodeBase64Len(const char* input)
{
    return Base64decode_len(input);
}

int CCCrypto::decodeBase64(const char* input,
                           char* output,
                           int outputBufferLength)
{
    CCAssert(Base64decode_len(input) <= outputBufferLength, "CCCrypto::decodeBase64() - outputBufferLength too small");
    return Base64decode(output, input);
}

void CCCrypto::MD5(void* input, unsigned long inputLength, unsigned char* output)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, input, inputLength);
    MD5_Final(output, &ctx);
}

const std::string CCCrypto::MD5File(const char* path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return "";
    
    MD5_CTX ctx;
    MD5_Init(&ctx);
    
    int i;
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    while ((i = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        MD5_Update(&ctx, buffer, (unsigned) i);
    }
    
	fclose(file);
	unsigned char output[MD5_BUFFER_LENGTH];
	MD5_Final(output, &ctx);
	char* hex = bin2hex(output, MD5_BUFFER_LENGTH);
	std::string ret(hex);
	delete[] hex;
	return ret;
}

const std::string CCCrypto::MD5String(void* input, int inputLength)
{
    unsigned char buffer[MD5_BUFFER_LENGTH];
    MD5(static_cast<void*>(input), inputLength, buffer);
	
    char* hex = bin2hex(buffer, MD5_BUFFER_LENGTH);
    std::string ret(hex);
    delete[] hex;
    return ret;
}

std::string CCCrypto::aes_encrypt(const std::string &sData, const std::string &sKey)
{
	std::string sResult;
	if (!aes_encrypt(sData, sKey, sResult))
	{
		throw std::runtime_error("aes encrypt");
	}
	return sResult;
}

std::string CCCrypto::aes_decrypt(const std::string &sData, const std::string &sKey)
{
	std::string sResult;
	if (!aes_decrypt(sData, sKey, sResult))
	{
		throw std::runtime_error("aes decrypt");
	}
	return sResult;
}

bool CCCrypto::aes_encrypt(const std::string &sData, const std::string &sKey, std::string &sResult)
{
	return aes_encrypt(sData.c_str(), sData.c_str() + sData.size(), sKey, sResult);
}

bool CCCrypto::aes_decrypt(const std::string &sData, const std::string &sKey, std::string &sResult)
{
	return aes_decrypt(sData.c_str(), sData.c_str() + sData.size(), sKey, sResult);
}

bool CCCrypto::aes_encrypt(const char *pBegin, const char *pEnd, const std::string &sKey, std::string &sResult)
{
	aes_context ctx;
	if (aes_set_key(&ctx, (unsigned char *)sKey.c_str(), sKey.size() * 8) != 0)
	{
		return false;
	}

	return CCCrypto::cbc_encrypt(pBegin, pEnd, (CCCrypto::block_encrypt_func)::aes_encrypt, 16, &ctx, sResult);
}

bool CCCrypto::aes_decrypt(const char *pBegin, const char *pEnd, const std::string &sKey, std::string &sResult)
{
	aes_context ctx;
	if (aes_set_key(&ctx, (unsigned char *)sKey.c_str(), sKey.size() * 8) != 0)
	{
		return false;
	}
	return CCCrypto::cbc_decrypt(pBegin, pEnd, (CCCrypto::block_decrypt_func)::aes_decrypt, 16, &ctx, sResult);
}
bool CCCrypto::cbc_decrypt(const char *pBegin, const char *pEnd, block_decrypt_func decrypt_func, int iBlockSize, void *ctx,char* pResult,int iBufferSize)
{
	int startIdx = 0;
	unsigned char buf[256] = { 0 };
	if (iBlockSize < 8 || iBlockSize > (int)sizeof(buf))
	{
		return false;
	}

	int iDataSize = pEnd - pBegin;
	int rem = iDataSize % iBlockSize;
	if (rem == 0)
	{
		//sResult.reserve(sResult.size() + iDataSize);
	}
	else if (rem == 1)
	{
		if (iDataSize < iBlockSize + 1)
		{
			return false;
		}
		rem = *(pEnd - 1);
		if (rem <= 0 || rem >= iBlockSize)
		{
			return false;
		}
		//sResult.reserve(sResult.size() + iDataSize - iBlockSize - 1 + rem);
		--pEnd;
	}
	else
	{
		return false;
	}

	for (const char *r = pBegin; r < pEnd; r += iBlockSize)
	{
		decrypt_func(ctx, r, buf);
		if (r != pBegin)
		{
			for (int i = 0; i < iBlockSize; ++i)
			{
				buf[i] ^= *(r - iBlockSize + i);
			}
		}
		if (rem != 0 && r + iBlockSize >= pEnd)
		{
			char* currentPtr = pResult + startIdx;
			memcpy( currentPtr , buf, rem );
			startIdx = startIdx + rem;
		}
		else
		{	
			char* currentPtr = pResult + startIdx;
			memcpy( currentPtr , buf, iBlockSize );
			startIdx = startIdx + iBlockSize;
		}
	}
	return true;
}
bool CCCrypto::aes_decrypt(const char *pBegin, const char *pEnd, const std::string &sKey ,char* pResult,int iBufferSize)
{
	aes_context ctx; 
	if (aes_set_key(&ctx, (unsigned char *)sKey.c_str(), sKey.size() * 8) != 0)
	{
		return false;
	}
	return CCCrypto::cbc_decrypt(pBegin, pEnd, (CCCrypto::block_decrypt_func)::aes_decrypt, 16, &ctx, pResult, iBufferSize);
}

bool CCCrypto::cbc_encrypt(const char *pBegin, const char *pEnd, block_encrypt_func encrypt_func, int iBlockSize, void *ctx, std::string &sResult)
{
	unsigned char buf[32] = { 0 };
	if (iBlockSize < 8 || iBlockSize > (int)sizeof(buf))
	{
		return false;
	}

	int iDataSize = pEnd - pBegin;
	int rem = iDataSize % iBlockSize;
	if (rem == 0)
	{
		sResult.reserve(sResult.size() + iDataSize);
	}
	else
	{
		sResult.reserve(sResult.size() + iDataSize - rem + iBlockSize + 1);
	}

	for (const char *r = pBegin; r < pEnd; r += iBlockSize)
	{
		int len = std::min(iBlockSize, (int)(pEnd - r));
		for (int i = 0; i < len; ++i)
		{
			buf[i] = *(r + i) ^ buf[i];
		}
		encrypt_func(ctx, buf, buf);
		sResult.append(buf, buf + iBlockSize);
	}
	if (rem != 0)
	{
		sResult.append(1, rem);
	}
	return true;
}

bool CCCrypto::cbc_decrypt(const char *pBegin, const char *pEnd, block_decrypt_func decrypt_func, int iBlockSize, void *ctx, std::string &sResult)
{
	unsigned char buf[32] = { 0 };
	if (iBlockSize < 8 || iBlockSize > (int)sizeof(buf))
	{
		return false;
	}

	int iDataSize = pEnd - pBegin;
	int rem = iDataSize % iBlockSize;
	if (rem == 0)
	{
		sResult.reserve(sResult.size() + iDataSize);
	}
	else if (rem == 1)
	{
		if (iDataSize < iBlockSize + 1)
		{
			return false;
		}
		rem = *(pEnd - 1);
		if (rem <= 0 || rem >= iBlockSize)
		{
			return false;
		}
		sResult.reserve(sResult.size() + iDataSize - iBlockSize - 1 + rem);
		--pEnd;
	}
	else
	{
		return false;
	}

	for (const char *r = pBegin; r < pEnd; r += iBlockSize)
	{
		decrypt_func(ctx, r, buf);
		if (r != pBegin)
		{
			for (int i = 0; i < iBlockSize; ++i)
			{
				buf[i] ^= *(r - iBlockSize + i);
			}
		}
		if (rem != 0 && r + iBlockSize >= pEnd)
		{
			sResult.append(buf, buf + rem);
		}
		else
		{
			sResult.append(buf, buf + iBlockSize);
		}
	}
	return true;
}

bool CCCrypto::cbc_encrypt(const std::string &sData, block_encrypt_func encrypt_func, int iBlockSize, void *ctx, std::string &sResult)
{
	return cbc_encrypt(sData.c_str(), sData.c_str() + sData.size(), encrypt_func, iBlockSize, ctx, sResult);
}

bool CCCrypto::cbc_decrypt(const std::string &sData, block_decrypt_func decrypt_func, int iBlockSize, void *ctx, std::string &sResult)
{
	return cbc_decrypt(sData.c_str(), sData.c_str() + sData.size(), decrypt_func, iBlockSize, ctx, sResult);
}

void CCCrypto::setKey(std::string sKey)
{
	m_sKey = CCCrypto::fromHex(sKey);
}

std::string CCCrypto::getKey()
{
	return m_sKey;
}

char* CCCrypto::bin2hex(unsigned char* bin, int binLength)
{
    static const char* hextable = "0123456789abcdef";
    
    int hexLength = binLength * 2 + 1;
    char* hex = new char[hexLength];
    memset(hex, 0, sizeof(char) * hexLength);
    
    int ci = 0;
    for (int i = 0; i < 16; ++i)
    {
        unsigned char c = bin[i];
        hex[ci++] = hextable[(c >> 4) & 0x0f];
        hex[ci++] = hextable[c & 0x0f];
    }
    
    return hex;
}
std::string CCCrypto::fromHex(const std::string &s)
{
	std::string r;
	std::string::size_type n = s.size() / 2;
	r.resize(n);
	for (std::string::size_type i = 0; i < n; ++i)
	{
		uint8_t hi = s[i * 2];
		uint8_t lo = s[i * 2 + 1];
		hi = hi >= 'a' ? (hi - 'a' + 10) : (hi >= 'A' ? (hi - 'A' + 10) : hi - '0');
		lo = lo >= 'a' ? (lo - 'a' + 10) : (lo >= 'A' ? (lo - 'A' + 10) : lo - '0');
		r[i] = (hi << 4) | lo;
	}
	return r;
}

void CCCrypto::xor_encrypt(const char* pBegin, unsigned int nSize, std::string &sResult)
{
	sResult.resize( nSize );
	for ( unsigned int i = 0; i < nSize; i ++ )
	{
		if ( i < 128 )
			sResult[i] = pBegin[i]^1002;
		else
			sResult[i] = pBegin[i];
	}
}
void CCCrypto::xor_decrypt(char* pBegin, unsigned int nSize )
{
	for ( unsigned int i = 0; i < nSize && i < 128; i ++ )
	{
		pBegin[i] ^= 1002;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
static void * AllocForLzma(void *p, size_t size) { return malloc(size); } 
static void FreeForLzma(void *p, void *address) { free(address); }
static ISzAlloc SzAllocForLzma = { &AllocForLzma, &FreeForLzma };
#pragma pack(1)
struct stHead
{
	unsigned int m_sign;
	unsigned char m_szPorp[LZMA_PROPS_SIZE];
	unsigned char m_nRandomValue;			// useless
	unsigned int m_OriginalSize;			// ^1002
};
#pragma pop
bool CCCrypto::compressData( const unsigned char* pInData, unsigned int nInSize, unsigned char* pOutData, unsigned int& nOutSize )
{
	if ( nOutSize <= sizeof(stHead) )
		return false;
	nOutSize -= sizeof(stHead);

	unsigned int propsSize = LZMA_PROPS_SIZE;
	CLzmaEncProps props;
	LzmaEncProps_Init(&props);
	props.level = 1;
	// 	props.dictSize = 1 << 26;	// 64 MB
	// 	props.fb = 64;
	// 	props.level = 1;
	// 	props.dictSize = 1 << 12;
	// 	props.algo = 0;
	props.writeEndMark = 0;		// 0 or 1

	stHead* phead = (stHead*)pOutData;
	int res = LzmaEncode( &(pOutData[sizeof(stHead)]), (SizeT*)&nOutSize, pInData, nInSize, &props, (phead->m_szPorp), (SizeT*)&propsSize, props.writeEndMark, NULL, &SzAllocForLzma, &SzAllocForLzma ); // 隐含的要求pOutData必须大于LZMA_PROPS_SIZE+4
	if ( res == SZ_OK && propsSize == LZMA_PROPS_SIZE )
	{
		nOutSize += sizeof(stHead);
		phead->m_szPorp[3] ^= 5;
		phead->m_OriginalSize = nInSize^1002;
		phead->m_sign = '@Fml';
		phead->m_nRandomValue = rand()%256;
		for ( unsigned i = 0; i < 16 && i < nOutSize; i ++ )
		{
			pOutData[sizeof(stHead)+i] ^= 6636;
		}
		return true;
	}
	return false;
}

bool CCCrypto::uncompressData( char* pInData, unsigned int nInSize, unsigned char** pOutData, SizeT& nOutSize )
{
	if (nInSize<sizeof(stHead))
		return false;
	stHead* phead = (stHead*)pInData;
	if ( phead->m_sign!='@Fml' )
		return false;
	nOutSize = phead->m_OriginalSize;
	nOutSize ^= 1002;
	*pOutData = (unsigned char*)malloc(nOutSize+1);
	(*pOutData)[nOutSize] = 0;

	SizeT srcLen = nInSize - sizeof(stHead);
	ELzmaStatus status;
	Byte prop[LZMA_PROPS_SIZE];
	for ( int i = 0; i < LZMA_PROPS_SIZE; i ++ )
		prop[i] = phead->m_szPorp[i];
	prop[3] ^= 5;
	for ( unsigned i = 0; i < 16 && i < nOutSize; i ++ )
	{
		pInData[sizeof(stHead)+i] ^= 6636;
	}

	SRes res = LzmaDecode( *pOutData, (SizeT*)&nOutSize, (const Byte*)&pInData[sizeof(stHead)], (SizeT*)&srcLen, prop, LZMA_PROPS_SIZE, LZMA_FINISH_END, &status, &SzAllocForLzma ); // LZMA_FINISH_ANY
	if ( res == SZ_OK )
		return true;

	free( *pOutData );
	*pOutData = NULL;
	nOutSize = 0;
	return false;
}

//NS_CC_EXTRA_END
