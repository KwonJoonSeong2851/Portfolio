#pragma once
#include "../../stdafx.h"
#include "PacketHeader.h"
#include "Stream.h"

class Room;

class Packet
{
public:
	virtual PacketType Type() = 0;
	virtual void Serialize(Stream& stream) { stream << (Int64)this->Type(); }
	virtual void Deserialize(Stream& stream) {}
};

class PK_I_NOTIFY_TERMINAL : public Packet
{
public:
	PacketType Type() { return E_I_NOTIFY_TERMINAL; }
};

class PK_C_REQ_ID_PW : public Packet
{
public:
	PacketType Type() { return E_C_REQ_ID_PW; }

	std::string id;
	std::string password;

	void Serialize(Stream& stream)
	{
		stream << (Int64)this->Type();
		stream << id;
		stream << password;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &id;
		stream >> &password;
	}

};

class PK_I_ANS_ID_PW : public Packet
{
public:
	PacketType Type() { return E_I_ANS_ID_PW; }



	UInt64 accountId;
	bool result;


	void Serialize(Stream& stream)
	{
		stream << (Int64)this->Type();
		stream << accountId;
		stream << result;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &accountId;
		stream >> &result;
	}
};

class PK_C_REQ_SIGNIN : public Packet
{
public:
	PacketType Type() { return E_C_REQ_SIGNIN; }

	std::string userId;
	std::string userPwd;
	std::string userNickName;

	void Serialize(Stream& stream)
	{
		stream << (Int64)this->Type();
		stream << userId;
		stream << userPwd;
		stream << userNickName;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &userId;
		stream >> &userPwd;
		stream >> &userNickName;
	}
};



class PK_I_ANS_SIGNIN : public Packet
{
public:
	PacketType Type() { return E_I_ANS_SIGNIN; }

	UINT32 result;

	void Serialize(Stream& stream)
	{
		stream << (Int64)this->Type();
		stream << result;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &result;
	}
};

class PK_C_REQ_CHATTING : public Packet
{
public:
	PacketType Type() { return E_C_REQ_CHATTING; }

	Int32 characterType;
	Int32 team;
	Int32 session;
	Int32 roomNumber;

	std::string m_id;
	std::string m_text;

	void Serialize(Stream& stream)
	{
		stream << (INT64)this->Type();
		stream << characterType;
		stream << team;
		stream << session;
		stream << roomNumber;

		stream << m_id;
		stream << m_text;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &characterType;
		stream >> &team;
		stream >> &session;
		stream >> &roomNumber;

		stream >> &m_id;
		stream >> &m_text;
		
	}
};

class PK_S_ANS_CHATTING : public Packet
{
public:
	PacketType Type() { return E_S_ANS_CHATTING; }

	Int32 characterType;
	Int32 team;
	Int32 session;
	Int32 roomNumber;

	std::wstring m_id;
	std::wstring m_text;

	void Serialize(Stream& stream)
	{
		stream << (INT64)this->Type();
		stream << characterType;
		stream << team;
		stream << session;
		stream << roomNumber;

		stream << m_id;
		stream << m_text;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &characterType;
		stream >> &team;
		stream >> &session;
		stream >> &roomNumber;

		stream >> &m_id;
		stream >> &m_text;
	}
};

class PK_S_ANS_TEST : public Packet
{
public:
	PacketType Type() { return E_S_ANS_TEST; }

	std::string m_testText; 

	void Serialize(Stream& stream)
	{
		stream << (INT64)this->Type();
		stream << m_testText;
	}

	void Desrialize(Stream& stream)
	{
		stream >> &m_testText;
	}
};


class PK_C_REQ_GET_TUTORIAL : public Packet
{
public:
	PacketType Type() { return E_C_REQ_GET_TUTORIAL; }

	UInt64 accountId;

	void Serialize(Stream& stream)
	{
		stream << (INT64)this->Type();
		stream << accountId;
	}

	void Desrialize(Stream& stream)
	{
		stream >> &accountId;
	}
};

class PK_S_ANS_GET_TUTORIAL : public Packet
{
public:
	PacketType Type() { return E_S_ANS_GET_TUTORIAL; }

	bool m_isTutorial;

	void Serialize(Stream& stream)
	{
		stream << (INT64)this->Type();
		stream << m_isTutorial;
	}

	void Desrialize(Stream& stream)
	{
		stream >> &m_isTutorial;
	}
};

class PK_C_REQ_SET_TUTORIAL : public Packet
{
public:
	PacketType Type() { return E_C_REQ_SET_TUTORIAL; }
	UInt64 accountId;

	void Serialize(Stream& stream)
	{
		stream << (INT64)this->Type();
		stream << accountId;
	}

	void Desrialize(Stream& stream)
	{
		stream >> &accountId;
	}
};

struct InGame_UserInfo
{
public:
	UInt64 account;
	string id;
	string nickName;
	UInt32 level;
	UInt64 experience;
	UInt32 win;
	UInt32 lose;
};

class PK_C_REQ_USERINFO : public Packet
{
public:
	PacketType Type() { return E_C_REQ_USERINFO; }

	UInt64 account;

	void Serialize(Stream& stream)
	{
		stream << (Int64)this->Type();
		stream << account;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &account;
	}
};

class PK_I_ANS_USERINFO : public Packet
{
public:
	PacketType Type() { return E_I_ANS_USERINFO; }

	bool result;
	UInt64 account;
	string id;
	wstring nickName;
	UInt32 level;
	UInt64 experience;
	UInt32 win;
	UInt32 lose;



	void Serialize(Stream& stream)
	{
		stream << (Int64)this->Type();
		stream << account;
		stream << id;
		stream << nickName;
		stream << level;
		stream << experience;
		stream << win;
		stream << lose;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &account;
		stream >> &id;
		stream >> &nickName;
		stream >> &level;
		stream >> &experience;
		stream >> &win;
		stream >> &lose;
	}
};

class PK_C_REQ_GAMERESULT : public Packet
{
public:
	PacketType Type() { return E_C_REQ_GAMERESULT; }

	UInt64 account;
	UInt32 gameResult;
	UInt64 experience;

	void Serialize(Stream& stream)
	{
		stream << (Int64)this->Type();
		stream << account;
		stream << gameResult;
		stream << experience;
	}

	void Deserialize(Stream& stream)
	{
		stream >> &account;
		stream >> &gameResult;
		stream >> &experience;
	}

};

