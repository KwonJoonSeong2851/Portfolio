#include "stdafx.h"
#include <locale.h>

void ServerProcess()
{
	shared_ptr<Server> mainServer(new IOCPServer(new MainProcess()));
	
	if (!mainServer->Run())
	{
		SLOG(L"Error :main server start fail");
		return;
	}


}

int _tmain(int argc, _TCHAR* argv[])
{
	//�����ڵ� ��� ����
	_wsetlocale(LC_ALL, L"korean");

	DBManager::GetInstance().Run();

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	shared_ptr<Thread> serverThread(new Thread(new thread_t(ServerProcess), L"Server"));
	
	return 0;
}