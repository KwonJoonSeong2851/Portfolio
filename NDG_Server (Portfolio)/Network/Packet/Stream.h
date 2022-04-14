#pragma once
#include "../../stdafx.h"

class Room;
class Stream
{
	size_t m_offset;
	size_t m_readPoint;

	std::array<UCHAR, SOCKET_BUF_SIZE> m_stream;

public:
	Stream();
	Stream(UCHAR* stream, size_t size);
	void Initialize();

	UCHAR* GetData();
	size_t GetSize();

	void operator = (Stream& stream);
	void Set(UCHAR* stream, size_t size);

	//write
	//-------------------------------------------------//
	bool CheckWriteBound(size_t len);

	template<class T>
	void operator << (const T& value);
	void operator << (const Byte& value);
    void operator << (const bool& value);
	//void operator << (const INT8& value);
	//void operator << (const UINT8& value);
	void operator << (const INT16& value);
	void operator << (const UINT16& value);
	void operator << (const INT32& value);
	void operator << (const UINT32& value);
	void operator << (const INT64& value);
	void operator << (const UINT64& value);
	void operator << (const Float& value);

	template<class T>
	void operator << (const std::vector<T>& value);
	//void operator << (const std::hash_map<wstr_t, wstr_t>& value);
	void operator << (const str_t value);
	void operator << (const wstr_t value);

	//void operator << (const Room& value);
	

	//read
	//-------------------------------------------------//
	bool CheckReadBound(size_t len);
	void Read(void* retVal, size_t len);

	template<class T>
	void operator >> (T* retVal);

	void operator >> (Byte* retVal);
	void operator >> (bool* retVal);
	void operator >> (INT8* retVal);
	//void operator >> (UINT8* retVal);
	void operator >> (INT16* retVal);
	void operator >> (UINT16* retVal);
	void operator >> (INT32* retVal);
	void operator >> (UINT32* retval);
	void operator >> (INT64* retVal);
	void operator >> (UINT64* retVal);
	void operator >> (Float* retVal);

	template<class T>
	void operator >> (std::vector<T>* retVal);
	//void operator >> (std::hash_map<wstr_t, wstr_t>* value);
	void operator >> (str_t* retVal);
	void operator >> (wstr_t* retVal);

	//void operator >> (Room* retval);

};