#include "DBManager.h"

DBManager::DBManager()
{
	xml_t config;
	if (!loadConfig(&config))
	{
		return;
	}

	this->Initialize(&config);
}

DBManager::~DBManager()
{
	SAFE_DELETE(m_queryPool);

	for (auto db : m_dbPool)
	{
		db->Disconnect();
		SAFE_DELETE(db);
	}
}



void DBManager::Initialize(xml_t* config)
{
	xmlNode_t* root = config->FirstChildElement("App")->FirstChildElement("DataBase");
	if (!root)
	{
		SLOG(L"DBManager : not exist database setting");
		return;
	}

	xmlNode_t* elem = root->FirstChildElement("ThreadCount");
	sscanf_s(elem->GetText(), "%d", &m_workerCount);

	array<WCHAR, SIZE_256> tmp;
	elem = root->FirstChildElement("ServerName");
	StrConvA2W((char*)elem->GetText(), tmp.data(), tmp.max_size());
	m_serverName = tmp.data();

	elem = root->FirstChildElement("DB");
	StrConvA2W((char*)elem->GetText(), tmp.data(), tmp.max_size());
	m_dbName = tmp.data();

	elem = root->FirstChildElement("Login");
	StrConvA2W((char*)elem->GetText(), tmp.data(), tmp.max_size());
	m_login = tmp.data();

	elem = root->FirstChildElement("Password");
	StrConvA2W((char*)elem->GetText(), tmp.data(), tmp.max_size());
	m_password = tmp.data();

	m_queryPool = new ThreadJobQueue<Query*>(L"DBQueueJob");

	for (int i = 0; i < m_workerCount; ++i)
	{
		array<WCHAR, SIZE_128> patch = { 0, };
		ADODatabase* adodb = new ADODatabase();
		m_dbPool.push_back(adodb);
	}
}

size_t DBManager::RunQueryCount()
{
	return m_queryPool->size();
}

void DBManager::PushQuery(Query* query)
{
	m_queryPool->Push(query);
}

bool DBManager::PopQuery(Query** query)
{
	return m_queryPool->Pop(*query);
}

void DBManager::Run()
{
	for (auto db : m_dbPool)
	{
		if (db->GetState() != DB_STOP)
		{
			continue;
		}

		if (!db->Connect(m_serverName.c_str(), m_dbName.c_str(), m_login.c_str(), m_password.c_str()))
		{
			SERRLOG(L"DBManager : db[%s] connection error", m_dbName.c_str());
		}
		db->Run();
	}
}


