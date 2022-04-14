#include "../stdafx.h"
#include "Session.h"
#include "ServerIOCP.h"


Session::Session()
{
	m_type = SESSION_TYPE_CLIENT;
	this->UpdateHeartBeat();
}

Session::~Session()
{
	this->OnClose();
}

bool Session::SetSocketOpt()
{
	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;
	keepAliveSet.keepalivetime = 3000;
	keepAliveSet.keepaliveinterval = 3000;

	DWORD dwBytes;

	if (WSAIoctl(m_socketData.socket_, SIO_KEEPALIVE_VALS, &keepAliveSet, sizeof(keepAliveSet), &returned, sizeof(returned), &dwBytes, NULL, NULL) != 0)
	{
		return false;
	}

	return true;
}

bool Session::OnAccept(SOCKET socket, SOCKADDR_IN addrInfo)
{
	m_socketData.socket_ = socket;
	int addrLen = sizeof(SOCKADDR);
	getpeername(m_socketData.socket_, (struct sockaddr*) & m_socketData.addrInfo_, &addrLen);
	m_socketData.addrInfo_ = addrInfo;

	if (!this->SetSocketOpt())
	{
		return false;
	}

	return true;
}

void Session::OnClose()
{
	SessionManager::GetInstance().CloseSession(this);
}

SOCKET& Session::GetSocket()
{
	return m_socketData.socket_;
}

str_t Session::GetClientAddress()
{
	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(m_socketData.addrInfo_.sin_addr), ip.data(), ip.size());
	return ip.data();
}

oid_t Session::GetId()
{
	return m_id;
}

void Session::SetId(oid_t id)
{
	m_id = id;
}

int8_t Session::GetType()
{
	return m_type;
}

void Session::SetType(int8_t type)
{
	m_type = type;
}

tick_t Session::HeartBeat()
{
	return m_lastHeartBeat;
}

void Session::UpdateHeartBeat()
{
	m_lastHeartBeat = CLOCK.SystemTick();
}

void Session::SetUser(User* user)
{
	m_user = user;
}

User* Session::GetUser()
{
	return m_user;
}
