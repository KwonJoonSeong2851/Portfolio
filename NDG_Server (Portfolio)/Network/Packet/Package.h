#pragma once
#include "../../stdafx.h"

class Session;
class Package
{
public:
	Session* m_session;
	Packet* m_packet;

	Package(Session* session, Packet* packet)
	{
		m_session = session;
		m_packet = packet;
	}

	~Package()
	{
		m_session = nullptr;
		SAFE_DELETE(m_packet);
	}
};