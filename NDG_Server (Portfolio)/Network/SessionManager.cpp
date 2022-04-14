#include "../stdafx.h"
#include "SessionManager.h"
#include "ServerIOCP.h"

SessionManager::SessionManager() : m_lock(L"SessionManager")
{
	m_idSeed = 1;
	m_maxConnection = SESSION_CAPACITY;
	this->CommandFucInitialize();
}



SessionManager::~SessionManager()
{
	vector<Session*> removeSessionVec;
	removeSessionVec.resize(m_sessionList.size());

	std::copy(m_sessionList.begin(), m_sessionList.end(), removeSessionVec.begin());

	for (auto session : removeSessionVec)
	{
		session->OnClose();
	}
	m_sessionList.clear();
}

list<Session*>& SessionManager::GetSessionList()
{
	return m_sessionList;
}

oid_t SessionManager::CreateSessionId()
{
	return m_idSeed++;
}

bool SessionManager::AddSession(Session* session)
{
	SAFE_LOCK(m_lock);
	auto findSession = std::find(m_sessionList.begin(), m_sessionList.end(), session);

	if (findSession != m_sessionList.end())
	{
		return false;
	}

	if (m_sessionCount > m_maxConnection)
	{
		SLOG(L"SessionManager : session so busy. count[%d]", m_sessionCount);
		return false;
	}

	session->SetId(this->CreateSessionId());
	m_sessionList.push_back(session);
	++m_sessionCount;

	return true;
}

bool SessionManager::CloseSession(Session* session)
{
	SAFE_LOCK(m_lock);
	if (session == nullptr)
	{
		return false;
	}

	auto findSession = std::find(m_sessionList.begin(), m_sessionList.end(), session);
	if (findSession != m_sessionList.end())
	{
		Session* delSession = *findSession;
		SLOG(L"SessionManager : deleted close by client [%S]", delSession->GetClientAddress().c_str());
		::closesocket(delSession->GetSocket());

		m_sessionList.remove(delSession);
		--m_sessionCount;
		SAFE_DELETE(delSession);
		return true;
	}
	return false;
}

void SessionManager::ForceCloseSession(Session* session)
{
	SAFE_LOCK(m_lock);
	if (!session)
		return;


	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 0;

	::setsockopt(session->GetSocket(), SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));
	this->CloseSession(session);
}



Session* SessionManager::GetSession(oid_t id)
{
	SAFE_LOCK(m_lock);
	Session* findSession = nullptr;

	for (auto session : m_sessionList)
	{
		if (session->GetId() == id)
		{
			findSession = session;
			break;
		}
	}
	return findSession;
}

Lock& SessionManager::GetLock()
{
	return m_lock;
}

void SessionManager::RunCommand(wstr_t cmdLine)
{
	std::size_t found = cmdLine.find(L' ');
	wstr_t command;
	wstr_t arg;

	if (found != wstr_t::npos)
	{
		command = cmdLine.substr(0, found);
		arg = cmdLine.substr(found);
	}
	else
	{
		command = cmdLine;
	}

	auto cmdFunc = m_serverCommand.at(command);
	if (cmdFunc)
	{
		cmdFunc(&m_sessionList, &arg);
	}

}

void SessionManager::CommandFucInitialize()
{
	////auto notiyFunc = [](SessionList* sessionList, wstr_t* arg)
	////{
	////	auto eachFunc = [arg](void* atom)
	////	{
	////		Session* session = (Session*)atom;
	////		if (session->GetType() == SESSION_TYPE_TERMINAL)
	////		{
	////			return;
	////		}

	////		array<CHAR, SIZE_256> tmpBuf;
	////		StrConvW2A((WCHAR*)arg->c_str(), tmpBuf.data(), tmpBuf.size());

	////		PK_S_ANS_CHATTING retPacket;
	////		retPacket.m_id = "Server";
	////		retPacket.m_text = "* Notity :";
	////		retPacket.m_text += tmpBuf.data();

	////		session->SendPacket(&retPacket);
	////	};

	////	for (auto session : *sessionList)
	////	{
	////		eachFunc(session);
	////	}
	//};

	////auto kickoffFunc = [](SessionList* sessionList, wstr_t* arg)
	////{
	////	vector<Session*> removeSessionVec;

	////	auto eachFunc = [&removeSessionVec, arg](void* atom)
	////	{
	////		Session* session = (Session*)atom;
	////		if (session->GetType() == SESSION_TYPE_TERMINAL)
	////		{
	////			return;
	////		}

	////		PK_S_ANS_CHATTING retPacket;
	////		retPacket.m_id = "Server";
	////		retPacket.m_text = "SessionManager : kick off by server";
	////		session->SendPacket(&retPacket);

	////		removeSessionVec.push_back(session);
	////	};

	////	for (auto session : *sessionList)
	////	{
	////		eachFunc(session);
	////	}

	////	for (auto session : removeSessionVec)
	////	{
	////		session->OnClose();
	////	}
	//};

	//auto exitFunc = [](SessionList* sessionList, wstr_t* arg)
	//{
	//	_shutdown = true;
	//};

	//m_serverCommand.insert(make_pair(L"/notify", notiyFunc));
	//m_serverCommand.insert(make_pair(L"/kickoff", kickoffFunc));
	//m_serverCommand.insert(make_pair(L"exit", exitFunc));

}
