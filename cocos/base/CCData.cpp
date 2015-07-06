/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
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

#include "base/CCData.h"

#include <string>
#include "../external/crypto/CCCrypto.h"

NS_CC_BEGIN

const Data Data::Null;

Data::Data() :
_bytes(nullptr),
_size(0),
m_pOld(nullptr)
{
    CCLOGINFO("In the empty constructor of Data.");
}

Data::Data(Data&& other) :
_bytes(nullptr),
_size(0),
m_pOld(nullptr)
{
    CCLOGINFO("In the move constructor of Data.");
    move(other);
}

Data::Data(const Data& other) :
_bytes(nullptr),
_size(0),
m_pOld(nullptr)
{
    CCLOGINFO("In the copy constructor of Data.");
    copy(other._bytes, other._size);
}

Data::~Data()
{
    CCLOGINFO("deallocing Data: %p", this);
    clear();
}

Data& Data::operator= (const Data& other)
{
    CCLOGINFO("In the copy assignment of Data.");
    copy(other._bytes, other._size);
    return *this;
}

Data& Data::operator= (Data&& other)
{
    CCLOGINFO("In the move assignment of Data.");
    move(other);
    return *this;
}

void Data::move(Data& other)
{
	m_pOld = other.m_pOld;
    _bytes = other._bytes;
    _size = other._size;
    
	other.m_pOld = nullptr;
    other._bytes = nullptr;
    other._size = 0;
}

bool Data::isNull() const
{
    return (_bytes == nullptr || _size == 0);
}

unsigned char* Data::getBytes() const
{
    return _bytes;
}

ssize_t Data::getSize() const
{
    return _size;
}

void Data::copy(const unsigned char* bytes, const ssize_t size)
{
    clear();
    
    if (size > 0)
    {
        _size = size;
        _bytes = (unsigned char*)malloc(sizeof(unsigned char) * _size);
        memcpy(_bytes, bytes, _size);
    }
}

void Data::fastSet(unsigned char* bytes, const ssize_t size)
{
    _bytes = bytes;
    _size = size;
}

void Data::clear()
{
    if (m_pOld!=nullptr)
        free(m_pOld);
    else
        free(_bytes);
    _bytes = nullptr;
    _size = 0;
}

bool Data::isEncryed() const
{
	if ( _size > sizeof(EncryedHeader) )
	{
		EncryedHeader* header = (EncryedHeader*)(_bytes);
		return header->isEncryed();
	}
	return false;
}
void Data::decryptData()
{
	if ( _size < sizeof(EncryedHeader) )
		return;
	EncryedHeader* header = (EncryedHeader*)(_bytes);
	if ( !header->isEncryed() )
		return;
	if ( header->getDecryptSize()==0 )
		return;

	if ( header->m_nType==ENCRYPT_AES )
	{
		char* realBuffer = (char*) malloc(header->getDecryptSize()+1);
		realBuffer[header->getDecryptSize()] = 0;
		bool isOK = CCCrypto::aes_decrypt( (char const*)(_bytes+sizeof(EncryedHeader)),(char const*)(_bytes+_size),CCCrypto::getKey(),(char*)realBuffer,header->getDecryptSize());
		if( isOK )
		{
			int totalSize = header->getDecryptSize();
			clear();
			_bytes = (unsigned char*)realBuffer;
			_size = totalSize;

			// uncompress
			unsigned char* pOutData = NULL;
			SizeT nOutSize = 0;
			isOK = CCCrypto::uncompressData( (char*)_bytes, _size, &pOutData, nOutSize );
			if ( isOK )
			{
				clear();
				_bytes = (unsigned char*)pOutData;
				_size = nOutSize;
			}
		}
		else
			free( realBuffer );
	}
	else if ( header->m_nType==ENCRYPT_XOR )
	{
		int totalSize = header->getDecryptSize();
		CCCrypto::xor_decrypt( (char*)(_bytes+sizeof(EncryedHeader)), totalSize);
		// uncompress
		unsigned char* pOutData = NULL;
		SizeT nOutSize = 0;
		bool isOK = CCCrypto::uncompressData( (char*)_bytes+sizeof(EncryedHeader), totalSize, &pOutData, nOutSize );
		if ( isOK )
		{
			clear();
			_bytes = (unsigned char*)pOutData;
			_size = nOutSize;
		}
        else
        {
            m_pOld = _bytes;
            _bytes = _bytes+sizeof(EncryedHeader);
            _size = totalSize;
        }
	}
}

NS_CC_END
