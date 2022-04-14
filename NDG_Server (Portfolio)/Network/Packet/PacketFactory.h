#pragma once
#include "../../stdafx.h"
#include "PacketHeader.h"
#include "PacketClass.h"


class PacketFactory : public Singleton<PacketFactory>
{
public:
	Packet* GetPacket(Int64 packetType)
	{
		switch (packetType)
		{
		case E_C_REQ_CHATTING:
			return new PK_C_REQ_CHATTING();
		case E_S_ANS_CHATTING:
			return new PK_S_ANS_CHATTING();
		case E_C_REQ_ID_PW:
			return new PK_C_REQ_ID_PW();
		case E_I_ANS_ID_PW:
			return new PK_I_ANS_ID_PW();
		case E_C_REQ_SIGNIN:
			return new PK_C_REQ_SIGNIN();
		case E_I_ANS_SIGNIN:
			return new PK_I_ANS_SIGNIN();
		case E_C_REQ_USERINFO:
			return new PK_C_REQ_USERINFO();
		case E_I_ANS_USERINFO:
			return new PK_I_ANS_USERINFO();
		case E_C_REQ_GAMERESULT:
			return new PK_C_REQ_GAMERESULT();
		case E_C_REQ_GET_TUTORIAL:
			return new PK_C_REQ_GET_TUTORIAL();
		case E_S_ANS_GET_TUTORIAL:
			return new PK_S_ANS_GET_TUTORIAL();
		case E_C_REQ_SET_TUTORIAL:
			return new PK_C_REQ_SET_TUTORIAL();
		


			
			 
		}
		return nullptr;
	}
}; 