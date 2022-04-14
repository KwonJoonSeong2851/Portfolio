#pragma once
#include "../stdafx.h"
#include "SessionManager.h"


typedef enum SERVER_STATUS
{
	SERVER_STOP,
	SERVER_INITIALZE,
	SERVER_READY,
};


class Package;
class ContentsProcess;
class Server
{
protected:
	char m_ip[16];
	int m_port;
	int m_workerThreadCount;

	SERVER_STATUS m_status;
	ContentsProcess* m_contentsProcess;


public:
	Server(ContentsProcess* contentsProcess);
	virtual ~Server();

	virtual void Initialize(xml_t *config);

	virtual bool Run() = 0;

	SERVER_STATUS& GetStatus();

	void PutPackage(Package* package);
};