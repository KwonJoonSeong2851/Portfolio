#pragma once
#include "../stdafx.h"
#include "Server.h"


#define MAX_IOCP_THREAD SIZE_64

class Thread;
class Server;
class IOCPServer : public Server, public Singleton<IOCPServer>
{
	SOCKET                      m_listenSocket;
	HANDLE                      m_iocp;
	Thread*                     m_acceptThread;
	std::array<Thread*, SIZE_64>     m_workerThread;

private:
	bool CreateListenSocket();

	static DWORD WINAPI AcceptThread(LPVOID serverPtr);
	static DWORD WINAPI WorkerThread(LPVOID serverPtr);


public:
	IOCPServer(ContentsProcess* contentProcess);
	virtual ~IOCPServer();

	bool Run();
	SOCKET GetListenSocket();
	HANDLE GetIocp();
	void OnAccept(SOCKET accepter, SOCKADDR_IN addrInfo);
};