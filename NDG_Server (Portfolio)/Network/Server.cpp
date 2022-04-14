#include "../stdafx.h"
#include "Server.h"

Server::Server(ContentsProcess* contentsProcess)
{
	SLOG(L"Server : Initialize network base");

	m_contentsProcess = contentsProcess;
	m_status = SERVER_STOP;

	xml_t config;
	if (!loadConfig(&config))
	{
		return;
	}

	this->Initialize(&config);
}

Server::~Server()
{
	ShutdownServer();

	m_status = SERVER_STOP;
	SAFE_DELETE(m_contentsProcess);

	SLOG(L"Server : End network base");
}

void Server::Initialize(xml_t* config)
{
	TerminalManager::GetInstance().Run(this);

	TaskManager::GetInstance();

	xmlNode_t* root = config->FirstChildElement("App")->FirstChildElement("Server");
	if (!root)
	{
		SLOG(L"Server : Not exist server setting");
		return;
	}

	//IP 설정
	xmlNode_t* element = root->FirstChildElement("IP");
	strcpy_s(m_ip, element->GetText());

	//Port 설정
	element = root->FirstChildElement("Port");
	sscanf_s(element->GetText(), "%d", &m_port);

	//Thread 개수 설정
	element = root->FirstChildElement("ThreadCount");
	sscanf_s(element->GetText(), "%d", &m_workerThreadCount);

	m_status = SERVER_INITIALZE;

	SLOG(L"Server : IO worker thread count : %d", m_workerThreadCount);

	root = config->FirstChildElement("App");
	element = root->FirstChildElement("Name");

	SLOG(L"Server : Server Start!!!")
}

SERVER_STATUS& Server::GetStatus()
{
	return m_status;
}

void Server::PutPackage(Package* package)
{
	m_contentsProcess->PutPackage(package);
}
