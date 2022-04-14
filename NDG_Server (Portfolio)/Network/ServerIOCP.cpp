#include "../stdafx.h"
#include "ServerIOCP.h"
#include "IOCPSession.h"

IOCPServer::IOCPServer(ContentsProcess* contentProcess) :Server(contentProcess)
{
}

IOCPServer::~IOCPServer()
{
	::closesocket(m_listenSocket);
}

bool IOCPServer::CreateListenSocket()
{
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, NULL, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_listenSocket == INVALID_SOCKET)
	{
		SERRLOG(L"IOCPServer : listenSocket fail !!!, %d",WSAGetLastError());
		return false;
	}

	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((u_short)m_port);
	//inet_pton(AF_INET, m_ip, &(serverAddr.sin_addr));
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int reUseAddr = 1;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reUseAddr, (int)sizeof(reUseAddr));

	int retval = ::bind(m_listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
	{
		SERRLOG(L"IOCPServer : Bind fail!!!");
		return false;
	}

	const int BACK_SOCKETS = 5;
	retval = ::listen(m_listenSocket, BACK_SOCKETS);
	if (retval == SOCKET_ERROR)
	{
		SERRLOG(L"IOCPServer : listen fail!!!");
		return false;
	}

	array<char, SIZE_64> ip;
	inet_ntop(AF_INET, &(serverAddr.sin_addr), ip.data(), ip.size());
	SLOG(L"IOCPServer : server listen socket created, ip: %S, port: %d", ip.data(), m_port);
	return true;
}

bool IOCPServer::Run()
{
	if (MAX_IOCP_THREAD < m_workerThreadCount)
	{
		SERRLOG(L"IOCPServer : workerThread limit[%d], config setting [%d]", MAX_IOCP_THREAD, m_workerThreadCount);
		return false;
	}

	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_workerThreadCount);
	if (m_iocp == nullptr)
	{
		return false;
	}

	this->CreateListenSocket();

	m_acceptThread = MAKE_THREAD(IOCPServer, AcceptThread);
	for (int i = 0; i < m_workerThreadCount; ++i)
	{
		m_workerThread[i] = MAKE_THREAD(IOCPServer, WorkerThread);
	}
	this->m_status = SERVER_READY;

	while (!_shutdown)
	{
		wstring cmdLine;
		std::getline(std::wcin, cmdLine);

		SLOG(L"Input was: %s", cmdLine.c_str());
		SessionManager::GetInstance().RunCommand(cmdLine);
	}
	return true;
}

void IOCPServer::OnAccept(SOCKET accepter, SOCKADDR_IN addrInfo)
{
	IOCPSession* session = new IOCPSession();
	if (session == nullptr)
	{
		SLOG(L"IOCPServer : Accept session create fail");
		return;
	}

	if (!session->OnAccept(accepter, addrInfo))
	{
		SAFE_DELETE(session);
		return;
	}

	session->m_ioData[IO_READ].Clear();

	HANDLE handle = CreateIoCompletionPort((HANDLE)accepter, this->GetIocp(), (ULONG_PTR)&(*session), NULL);

	if (!handle)
	{
		SAFE_DELETE(session);
		return;
	}

	SLOG(L"IOCPServer : Client accept form [%S]", session->GetClientAddress().c_str());
	session->RecvStandBy();

	SessionManager::GetInstance().AddSession(session);
}




DWORD WINAPI IOCPServer::WorkerThread(LPVOID serverPtr)
{
	IOCPServer* server = (IOCPServer*)serverPtr;

	while (!_shutdown)
	{
		IoData* ioData = nullptr;
		IOCPSession* session = nullptr;
		DWORD transferSize;

		BOOL ret = GetQueuedCompletionStatus(server->GetIocp(), &transferSize, (PULONG_PTR)&session, (LPOVERLAPPED*)&ioData, INFINITE);
		if (!ret)
		{
			continue;
		}

		if (session == nullptr)
		{
			SLOG(L"IOCPServer : socket data broken");
		}

		if (transferSize == 0)
		{
			SessionManager::GetInstance().CloseSession(session);
			continue;
		}

		switch (ioData->GetType())
		{
		case IO_WRITE:
			session->OnSend((size_t)transferSize);
			continue;

		case IO_READ:
		{
			Package* package = session->OnRecv((size_t)transferSize);
			if (package != nullptr)
			{
				server->PutPackage(package);
			}
		}
		continue;

		case IO_ERROR:
			SessionManager::GetInstance().CloseSession(session);
			continue;
		}
	}
	return 0;
}


DWORD WINAPI IOCPServer::AcceptThread(LPVOID serverPtr)
{
	IOCPServer* server = (IOCPServer*)serverPtr;

	while (!_shutdown)
	{
		SOCKET acceptSocket = INVALID_SOCKET;
		SOCKADDR_IN recvAddr;
		static int addrLen = sizeof(recvAddr);
		acceptSocket = WSAAccept(server->GetListenSocket(), (struct sockaddr*)& recvAddr, & addrLen, NULL, 0);

		if (acceptSocket == SOCKET_ERROR)
		{
			if (!server->GetStatus() == SERVER_STOP)
			{
				SLOG(L"IOCPServer : Accept fail");
				break;
			}
		}

		server->OnAccept(acceptSocket, recvAddr);
		if (server->GetStatus() != SERVER_READY)
		{
			break;
		}
	}
	return 0;
}





SOCKET IOCPServer::GetListenSocket()
{
	return m_listenSocket;
}

HANDLE IOCPServer::GetIocp()
{
	return m_iocp;
}


