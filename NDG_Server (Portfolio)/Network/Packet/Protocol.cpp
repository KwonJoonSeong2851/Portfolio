#include "Protocol.h"
#include <typeinfo>

type_info Protocol::GetTypeOfCode(char typeCode)
{

}

char Protocol::GetCodeOfType(const char* type)
{
		if (type == typeid(bool).raw_name())
			return GpType::E_Boolean;

		if (type == typeid(char).raw_name())
			return GpType::E_Byte;

		if (type == typeid(short).raw_name())
			return GpType::E_Short;

		if (type == typeid(int).raw_name())
			return GpType::E_Integer;

		if (type == typeid(long).raw_name())
			return GpType::E_Long;

		if (type == typeid(float).raw_name())
			return GpType::E_Float;

		if (type == typeid(double).raw_name())
			return GpType::E_Double;

		if (type == typeid(string).raw_name())
			return GpType::E_String;

		if (type == typeid(EventData).raw_name())
			return GpType::E_EventData;

		if (type == typeid(Dictionary).raw_name())
			return GpType::E_Dictionary;

		throw "[Protocol::GetCodeOfType]정의된 타입이 없습니다";
}

void Protocol::SerializeEventData(Stream& stream, const EventData& eventData, bool setType = true)
{
	if (setType)
		stream << (Byte)GpType::E_EventData;

	stream << (Byte)eventData.Code;

	SerializeParameterTable(stream, eventData.Parameters);

}

void Protocol::SerializeParameterTable(Stream& stream, const Dictionary& dic)
{
	Int16 dicSize = dic.size();
	if (dicSize == 0)
	{
		SerializeShort(stream, dicSize, false);
	}
	else
	{
		SerializeShort(stream, dicSize, false);
		for (auto& var : dic)
		{
			stream << (Byte)var.first;
			SerializeDataPair(stream, var.second, true);
		}
	
	}
}

void Protocol::SerializeDictionary(Stream& stream,const Dictionary& dic, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Dictionary;

	SerializeShort(stream, (short)dic.size(), setType);

	
	for (auto &var : dic)
	{
		stream << (Byte)var.first;
		SerializeDataPair(stream,(std::pair<char,void*>)var.second,true);
	}
}

void Protocol::SerializeDataPair(Stream& stream,std::pair<char, void*> p,bool setType)
{
	switch ((GpType)p.first)
	{
	case GpType::E_Boolean:
SerializeBoolean(stream, (bool&)p.second, setType);
break;

	case GpType::E_Byte:
		SerializeByte(stream, (Byte&)p.second, setType);
		break;

	case GpType::E_Short:
		SerializeShort(stream, (short&)p.second, setType);
		break;

	case GpType::E_Integer:
		SerializeInteger(stream, (Int32&)p.second, setType);
		break;

	case GpType::E_Long:
		SerializeLong(stream, (Int64&)p.second, setType);
		break;

	case GpType::E_Float:
		SerializeFloat(stream, (float&)p.second, setType);
		break;

	case GpType::E_String:
		SerializeString(stream, (string&)p.second, setType);
		break;

	case GpType::E_EventData:
		SerializeEventData(stream, (EventData&)p.second, setType);
		break;

	case GpType::E_Dictionary:
		SerializeDictionary(stream, (Dictionary&)p.second, setType);
		break;

	}
}


template<typename T>
void Protocol::Serialize(Stream& stream, const T& object, bool setType)
{
	switch (GetCodeOfType(typeid(object).raw_name()))
	{
	case GpType::E_Boolean:
		SerializeBoolean(stream, (const bool&)object, setType);
		break;

	case GpType::E_Byte:
		SerializeByte(stream, (const Byte&)object, setType);
		break;

	case GpType::E_Short:
		SerializeShort(stream, (const Int16&)object, setType);
		break;

	case GpType::E_Integer:
		SerializeInteger(stream, (const Int32&)object, setType);
		break;

	case GpType::E_Long:
		SerializeLong(stream, (const Int32&)object, setType);
		break;

	case GpType::E_Float:
		SerializeFloat(stream, (const Float&)object, setType);
		break;

	case GpType::E_String:
		SerializeString(stream, (const string&)object, setType);
		break;

	case GpType::E_EventData:
		SerializeEventData(stream, (const EventData&)object, setType);
		break;

	case GpType::E_Dictionary:
		SerializeDictionary(stream, (const Dictionary&)object, setType);
		break;

	case GpType::E_Hashtable:
		SerializeHashtabe(stream, (const Hashtable&)object, setType);
		break;

	//case GpType::E_Array:
	//	SerializeArray(stream, (const T&)object, setType);
	//	break;


	}

}

template<typename T, std::size_t N>
void Protocol::SerializeArray(Stream& stream, const T(&arr)[N], bool setType)
{
	if (setType)
		Serialize(stream,(Byte)GpType::E_Array,false);

	SerializeShort(stream, (short)N, false);

	GpType type = (GpType)GetCodeOfType(typeid(arr[0]).raw_name);

	if ((UInt32)type > 0U)
	{
		Serialize(stream, (Byte)type, false);

		if (type == GpType::E_Dictionary)
			throw "Dictionary array is not supported";

		for (int index = 0; index < N; ++index)
		{
			T obj = arr[index];
			Serialize(stream, obj, false);
		}

	}
	else
		throw "CustomType array is not supported";

}

void Protocol::SerializeByte(Stream& stream, const Byte& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Byte;
	stream << object;
}

void Protocol::SerializeBoolean(Stream& stream, const bool& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Boolean;
	stream << object;
}

void Protocol::SerializeShort(Stream& stream,const Int16& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Short;
	stream << object;
}


void Protocol::SerializeInteger(Stream& stream, const Int32& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Integer;
	stream << object;
}

void Protocol::SerializeLong(Stream& stream, const Int64& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Long;
	stream << object;
}


void Protocol::SerializeFloat(Stream& stream, const Float& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Float;
	stream << object;
}

void Protocol::SerializeString(Stream& stream, const string& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_String;
	stream << object;
}

void Protocol::SerializeHashtabe(Stream& stream, const Hashtable& object, bool setType)
{
	if (setType)
		stream << (Byte)GpType::E_Hashtable;

	this->SerializeShort(stream, (short)object.size(), false);

	for (auto& var : object)
	{
		Serialize(stream, var.first.first, false);
		Serialize(stream, var.first.second, false); 
		Serialize(stream, var.second.first, false);
		Serialize(stream, var.second.second, false);
	}
	
}










void* Protocol::Deserialize(Stream& stream,Byte type)
{

	switch((GpType)type)
	{
	case GpType::E_Boolean:
		return DeserializeBoolean(stream);
		break;

	case GpType::E_Byte:
		return DeserializeByte(stream);
		break;

	case GpType::E_Short:
		return DeserializeShort(stream);
		break;

	case GpType::E_Integer:
		return DeserializeInteger(stream);
		break;

	case GpType::E_Long:
		return DeserializeLong(stream);
		break;

	case GpType::E_Float:
		return DeserializeFloat(stream);
		break;

	case GpType::E_String:
		return DeserializeString(stream);
		break;

	case GpType::E_Dictionary:
		return DeserializeDictionary(stream);
		break;

	}
}

void* Protocol::DeserializeByte(Stream& stream)
{
	Byte data;
	stream >> &data;
	return &data;
}

void* Protocol::DeserializeBoolean(Stream& stream)
{
	bool data;
	stream >> &data;
	return &data;
}

void* Protocol::DeserializeShort(Stream& stream)
{
	Int16 data;
	stream >> &data;
	return &data;
}

void* Protocol::DeserializeInteger(Stream& stream)
{
	Int32 data;
	stream >> &data;
	return &data;
}

void* Protocol::DeserializeLong(Stream& stream)
{
	Int64 data;
	stream >> &data;
	return &data;
}

void* Protocol::DeserializeFloat(Stream& stream)
{
	float data;
	stream >> &data;
	return &data;
}

void* Protocol::DeserializeString(Stream& stream)
{
	string data;
	stream >> &data;
	return &data;
}

Hashtable* Protocol::DeserializeHashtable(Stream& stream)
{
	Hashtable* hash = new Hashtable();
	int len = (int)DeserializeShort(stream);
	for (int index = 0; index, len; ++index)
	{
		pair<char, void*> key;
		key.first = (char)Deserialize(stream, GpType::E_Byte);
		key.second = Deserialize(stream, key.first);

		pair<char, void*> object;
		object.first = (char)Deserialize(stream, GpType::E_Byte);
		object.second = Deserialize(stream, object.first);
		hash->insert(make_pair(key, object));
	}

	return hash;
}



EventData* Protocol::DeserializeEventData(Stream& stream,EventData* object)
{

}

Dictionary* Protocol::DeserializeParameterTable(Stream& stream, Dictionary* target)
{
	short num;
    stream >> &num;
	Dictionary* dictionary = target != nullptr ? target : new Dictionary();

	for (int index = 0; index < (int)num; ++index)
	{
		Byte key;
		stream >> &key;

		pair<Byte, void*> obj;
		obj.first = (Byte)Deserialize(stream, GpType::E_Byte);
		obj.second = Deserialize(stream, obj.first);

		dictionary->insert(make_pair(key, obj));

	}

	return dictionary;
}

Dictionary* Protocol::DeserializeDictionary(Stream& stream)
{
	Byte type1 = (Byte)Deserialize(stream,GpType::E_Byte);
	Byte type2 = (Byte)Deserialize(stream,GpType::E_Byte);
	int len = (int)this->DeserializeShort(stream);
}



 



