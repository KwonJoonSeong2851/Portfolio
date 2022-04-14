#include "../stdafx.h"
#include "Terminal.h"
#include "Packet/PacketAnalyer.h"

Terminal::Terminal(Server* server, wstr_t name)
{
	m_server = server;
	m_name = name;
}

Terminal::~Terminal()
{
	m_status = TERMINAL_STOP;
}

TERMINAL_STATUS& Terminal::GetStatus()
{
	return m_status;
}

void Terminal::Initialize(xmlNode_t* config)
{
	xmlNode_t* element;

	element = config->FirstChildElement("IP");
	strcpy_s(m_ip, element->GetText());

	element = config->FirstChildElement("Port");
	sscanf_s(element->GetText(), "%d", &m_port);

	this->Run();
}

void Terminal::SendPacket(Packet* packet)
{
	if (m_status == TERMINAL_READY)
	{
		m_session.SendPacket(packet);
	}
}

const char* Terminal::GetIp()
{
	return m_ip;
}

void Terminal::ConnectProcess() 
{
CONNECT_START:
	int tryCount = 0;

	while (_shutdown == false)
	{
		if (m_session.ConnectTo(m_ip, m_port))
		{
			break;
		}
		SLOG(L"Terminal : try connect [%s] server[%s]:[%d]... [%d]", m_name.c_str(), m_ip, m_port, tryCount++);
		Sleep(1000);
		m_status = TERMINAL_READY;

		//터미널 세션 알림
		PK_I_NOTIFY_TERMINAL terminalPacket;
		this->SendPacket(&terminalPacket);

		SLOG(L"Terminal : [%s] connect [%S]:[%d] ready", m_name.c_str(), m_ip, m_port);
		while (_shutdown == false)
		{
			Package* package = m_session.OnRecv(0);

			if (package == nullptr)
			{
				SLOG(L"Terminal : [%s] disconnected !!!", m_name.c_str());
				m_session.OnClose();
				goto CONNECT_START;
			}

			m_server->PutPackage(package);
		}
	}
}

void Terminal::Run()
{
	m_processThread = MAKE_THREAD(Terminal, ConnectProcess);
}
