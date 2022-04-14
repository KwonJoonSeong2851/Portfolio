#pragma once
#include "../../stdafx.h"


class EventData;
class Protocol
{
public:
    enum GpType : char
    {
        /// <summary>Unkown type.</summary>
        E_Unknown = 0,
        /// <summary>Null value don't have types.</summary>
        E_Null = 42, // 0x2A
        /// <summary>A dictionary</summary>
        /// <remarks>This type is new in version 1.6.</remarks>
        E_Dictionary = 68, // 0x44
        /// <summary>An array of string values.</summary>
        E_StringArray = 97, // 0x61
        /// <summary>A byte value.</summary>
        E_Byte = 98, // 0x62
        /// <summary>A custom type. 0x63</summary>
        E_Custom = 99, // 0x63
        /// <summary>A 64-bit floating-point value.</summary>
        /// <remarks>This type is new in version 1.5.</remarks>
        E_Double = 100, // 0x64
        E_EventData = 101, // 0x65
        /// <summary>A 32-bit floating-point value.</summary>
        /// <remarks>This type is new in version 1.5.</remarks>
        E_Float = 102, // 0x66
        /// <summary>A Hashtable.</summary>
        E_Hashtable = 104, // 0x68
        /// <summary>A 32-bit integer value.</summary>
        E_Integer = 105, // 0x69
        /// <summary>A 16-bit integer value.</summary>
        E_Short = 107, // 0x6B
        /// <summary>A 64-bit integer value.</summary>
        E_Long = 108, // 0x6C
        /// <summary>An array of 32-bit integer values.</summary>
        E_IntegerArray = 110, // 0x6E
        /// <summary>A boolean Value.</summary>
        E_Boolean = 111, // 0x6F
        E_OperationResponse = 112, // 0x70
        E_OperationRequest = 113, // 0x71
        /// <summary>A string value.</summary>
        E_String = 115, // 0x73
        /// <summary>An array of bytes.</summary>
        E_ByteArray = 120, // 0x78
        /// <summary>An array of objects.</summary>
        /// <remarks>This type is new in version 1.5.</remarks>
        E_Array = 121, // 0x79
        /// <summary>An array of objects.</summary>
        E_ObjectArray = 122, // 0x7A
    };
private:
	type_info GetTypeOfCode(char typeCode);
    char GetCodeOfType(const char* typeInfo);

public:
    template<typename T>
    void Serialize(Stream& stream,const T& object, bool setType);



    void SerializeEventData(Stream& stream, const  EventData& eventData, bool setType);
    void SerializeParameterTable(Stream& stream, const Dictionary& dic);
    void SerializeDictionary(Stream& stream, const Dictionary& dic, bool setType);
    void SerializeDataPair(Stream& stream, const std::pair<char, void*> p,bool setType);

    void SerializeByte(Stream& stream, const Byte& object, bool setType);
    void SerializeBoolean(Stream& stream, const bool& object, bool setType);
    void SerializeShort(Stream& stream, const Int16& object, bool setType);
    void SerializeInteger(Stream& stream, const Int32& object, bool setType);
    //void SerializeUInteger(Stream& stream, UInt32 object, bool setType);
    void SerializeLong(Stream& stream, const Int64& object, bool setType);
    //void SerializeULong(Stream& stream, UInt64 object, bool setType);
    void SerializeFloat(Stream& stream, const Float& object, bool setType);
    void SerializeString(Stream& stream, const string& object, bool setType);

    template<typename T,std::size_t N>
    void SerializeArray(Stream& stream, const T (&arr)[N], bool setType);

    void SerializeHashtabe(Stream& stream, const Hashtable& object, bool setType);


    void* Deserialize(Stream& stream,Byte type);

    void* DeserializeByte(Stream& stream);
    void* DeserializeBoolean(Stream& stream);
    void* DeserializeShort(Stream& stream);
    void* DeserializeInteger(Stream& stream);

    void* DeserializeLong(Stream& stream);

    void* DeserializeFloat(Stream& stream);
    void* DeserializeString(Stream& stream);

    Hashtable* DeserializeHashtable(Stream& stream);


    EventData* DeserializeEventData(Stream& stream, EventData* object);
    Dictionary* DeserializeParameterTable(Stream& stream, Dictionary* dic);
    Dictionary* DeserializeDictionary(Stream& stream);
}; 