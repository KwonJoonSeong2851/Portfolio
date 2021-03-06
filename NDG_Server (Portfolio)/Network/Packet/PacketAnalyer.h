#pragma once
#include "../../stdafx.h"
#include "PacketClass.h"
#include "PacketFactory.h"


class PacketAnalyzer : public Singleton<PacketAnalyzer>
{
public:
	inline Packet* Analyzer(const char* rowPacket, size_t size)
	{
		size_t offset = 0;
		PacketType type[1] = { (PacketType)0, };

		memcpy_s((void*)type, sizeof(type), (void*)rowPacket, sizeof(type));
		offset += sizeof(type);

		Packet* packet = PacketFactory::GetInstance().GetPacket(type[0]);
		if (packet)
		{
			if (offset < size)
			{
				Stream stream((UCHAR*)(rowPacket + offset), size - offset);
				packet->Deserialize(stream);
			}
		}

		return packet;
	}
};
