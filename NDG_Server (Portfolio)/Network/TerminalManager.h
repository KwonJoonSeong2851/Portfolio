#pragma once
#include "../stdafx.h"


class Terminal;
class Server;
class TerminalManager : public Singleton<TerminalManager>
{
	std::hash_map<wstr_t, Terminal*> m_terminalPool;
	Server* m_server;

public:
	TerminalManager();
	virtual ~TerminalManager();

	void Initialize(xml_t* config);

	void Put(wstr_t serverName, Terminal* terminal);
	Terminal* GetTerminal(wstr_t name);

	bool IsTerminal(const char* ip);
	void Run(Server* server);
};