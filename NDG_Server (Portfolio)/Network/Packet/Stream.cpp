#pragma once
#include "../../stdafx.h"
#include "Stream.h"


Stream::Stream()
{
	this->Initialize();
}

Stream::Stream(UCHAR* stream, size_t size)
{
	this->Initialize();
	this->Set(stream, size);
}

void Stream::Initialize()
{
	m_readPoint = 0;
	m_offset = 0;
	ZeroMemory(&m_stream, sizeof(m_stream));
}

UCHAR* Stream::GetData()
{
	return m_stream.data();
}

size_t Stream::GetSize()
{
	return m_offset;
}

void Stream::operator = (Stream& stream)
{
	this->Set(stream.GetData(), stream.GetSize());
}

void Stream::Set(UCHAR* data, size_t size)
{
	this->m_offset = size;
	memcpy_s(this->m_stream.data(), m_stream.size(), (void*)data, size);
}

//Write
//-------------------------------------------------//
bool Stream::CheckWriteBound(size_t len)
{
	if (m_offset + len > sizeof(m_stream))
	{
		SLOG(L"Stream : Socket stream over !!!");
		ASSERT(false);
		return false;
	}
	return true;
}

#define STREAM_WRTIE(value)                          \
    INT32 size = sizeof(value);                      \
    if (this->CheckWriteBound(size) == false)        \
    {return;}                                        \
    memcpy_s((void*)(m_stream.data() + m_offset), m_stream.size() - m_offset, (const void*)&value,size); \
    m_offset += size; 





template<class T>
void Stream::operator << (const T& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator<<(const Byte& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator <<  (const bool& value)
{
	STREAM_WRTIE(value);
}

//void Stream::operator << (const INT8& value)
//{
//	STREAM_WRTIE(value);
//}

//void Stream::operator << (const UINT8& value)
//{
//	STREAM_WRTIE(value);
//}

void Stream::operator << (const INT16& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator << (const UINT16& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator << (const INT32& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator << (const UINT32& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator << (const INT64& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator << (const UINT64& value)
{
	STREAM_WRTIE(value);
}

void Stream::operator<<(const Float& value)
{
	STREAM_WRTIE(value);
}

template<class T>
void Stream::operator << (const std::vector<T>& value)
{
	*this << value.size();
	for (auto i : value)
	{
		*this << i;
	}
}


//void Stream::operator<<(const std::hash_map<wstr_t, wstr_t>& value)
//{
//	*this << value.size();
//	for (auto i : value)
//	{
//		*this << i.first;
//		*this << i.second;
//	}
//}

void Stream::operator << (const str_t value)
{
	*this << (Int32)value.length();
	for (auto i : value)
	{
		*this << i;
	}
}

void Stream::operator << (const wstr_t value)
{
	*this << (Int32)(value.length() * 2);
	for (auto i : value)
	{
		*this << i;
	}
}


//void Stream::operator << (const Room& value)
//{
//	*this << value.m_roomId;
//	*this << value.m_userCount;
//	*this << value.m_maxUser;
//	*this << value.m_roomName;
//	*this << value.m_isOpen;
//	*this << value.m_isVisible;
//	*this << value.m_isPrivateRoom;
//	*this << value.m_roomPassword;;
//}




// Read
//-------------------------------------------------////-------------------------------------------------////-------------------------------------------------//
//-------------------------------------------------////-------------------------------------------------////-------------------------------------------------//



bool Stream::CheckReadBound(size_t len)
{
	if (m_readPoint + len > m_offset)
	{
		SLOG(L"Stream : read Offset %d, totaloffset = %d", m_readPoint, len, m_offset);
		SLOG(L"Stream : socket stream has not more memory !!!");
		ASSERT(FALSE);
		return false;
	}
	return true;
}

void Stream::Read(void* retVal, size_t len)
{
	memcpy_s(retVal, len, (void*)(m_stream.data() + m_readPoint), len);
	m_readPoint += len;
}
 

#define STREAM_READ(type,retVal)                  \
     size_t size = sizeof(type);                  \
     if (this->CheckReadBound(size) == false)     \
          return;                                 \
     this->Read((void*)(retVal), size);

template<class T>
void Stream::operator >> (T* retVal)
{
	STREAM_READ(T, retVal);
}



void Stream::operator >> (bool* retVal)
{
	STREAM_READ(bool, retVal);
}

void Stream::operator >> (INT8* retVal)
{
	STREAM_READ(INT8, retVal);
}

void Stream::operator >> (UINT8* retVal)
{
	STREAM_READ(UINT8, retVal);
}

void Stream::operator >> (INT16* retVal)
{
	STREAM_READ(INT16, retVal);
}

void Stream::operator >> (UINT16* retVal)
{
	STREAM_READ(UINT16, retVal);
}

void Stream::operator >> (INT32* retVal)
{
	STREAM_READ(INT32, retVal);
}

void Stream::operator >> (UINT32* retVal)
{
	STREAM_READ(UINT32, retVal);
}

void Stream::operator >> (INT64* retVal)
{
	STREAM_READ(INT64, retVal);
}

void Stream::operator >> (UINT64* retVal)
{
	STREAM_READ(UINT64, retVal);
}

void Stream::operator>>(Float* retVal)
{
	STREAM_READ(Float, retVal);
}

template<class T>
void Stream::operator >> (std::vector<T>* retVal)
{
	size_t size;
	*this >> &size;

	for (size_t i = 0; i < size; ++i)
	{
		T tmp;
		*this >> &tmp;
		retVal->push_back(tmp);
	}
}

//void Stream::operator>>(std::hash_map<wstr_t, wstr_t>* retval)
//{
//	size_t size;
//	*this >> &size;
//
//	for (size_t i = 0; i < size; ++i)
//	{
//		wstr_t tmp1, tmp2;
//		*this >> &tmp1;
//		*this >> &tmp2;
//		retval->insert(std::pair<wstr_t, wstr_t>(tmp1, tmp2));
//	}
//
//}


void Stream::operator >> (str_t* retVal)
{
	INT32 size;
	*this >> &size;
	if (this->CheckReadBound(size) == false)
	{
		return;
	}

	char* buf = new char[size + 1];
	this->Read((void*)(buf), size * sizeof(CHAR));
	buf[size] = '\0';

	retVal->clear();
	*retVal = buf;

	delete buf;
}

void Stream::operator >> (wstr_t* retVal)
{
	INT32 size;
	*this >> &size;
	if (this->CheckReadBound(size) == false)
	{
		return;
	}

	WCHAR* buf = new WCHAR[size + 1];
	this->Read((void*)(buf), size * sizeof(WCHAR));
	buf[size] = '\0';

	retVal->clear();
	*retVal = buf;

	delete buf;
}


//void Stream::operator>>(Room* retval)
//{
//	*this >> &retval->m_roomId;
//	*this >> &retval->m_userCount;
//	*this >> &retval->m_maxUser;
//	*this >> &retval->m_roomName;
//	*this >> &retval->m_isOpen;
//	*this >> &retval->m_isVisible;
//	*this >> &retval->m_isPrivateRoom;
//	*this >> &retval->m_roomPassword;
//}



