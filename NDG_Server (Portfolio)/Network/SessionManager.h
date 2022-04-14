#pragma once
#include "../stdafx.h"
#include "Session.h"

//-------------------------------------------------//
/*
     Session 관리 클래스

	
*/
//-------------------------------------------------//



#define SESSION_CAPACITY (5000)

class SessionManager : public Singleton<SessionManager>
{
	typedef std::list<Session*> SessionList;

	SessionList m_sessionList;
	int m_sessionCount;
	int m_maxConnection;
	Lock m_lock;
	oid_t m_idSeed;

	typedef std::function<void(SessionList* sessionList, wstr_t* arg)> cmdFunc;
	std::hash_map<wstr_t, cmdFunc> m_serverCommand;

public:
	SessionManager();
	//SessionManager(int maxConnection);
	~SessionManager();

	oid_t CreateSessionId();

	bool AddSession(Session* session);
	bool CloseSession(Session* session);
	void ForceCloseSession(Session* session);


	std::list<Session*>& GetSessionList();
	Session* GetSession(oid_t id);

	Lock& GetLock();

	void RunCommand(wstr_t cmd);
	void CommandFucInitialize();
};