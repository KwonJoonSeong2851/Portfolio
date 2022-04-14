#pragma once
#include "../stdafx.h"

typedef enum TERMINAL_STATUS
{
	TERMINAL_STOP,
	TERMINAL_READY,
};

class Server;
class TerminalSession;
class Terminal
{
protected:
	Server* m_server;
	wstr_t m_name;
	TERMINAL_STATUS m_status;

	char m_ip[16];
	int m_port;

	TerminalSession m_session;

	Thread* m_processThread;

public:
	Terminal(Server* server, wstr_t name);
	virtual ~Terminal();
	TERMINAL_STATUS& GetStatus();

	void Initialize(xmlNode_t* config);
	void SendPacket(Packet* packet);
	const char* GetIp();

private:
	void ConnectProcess();
	void Run();
};