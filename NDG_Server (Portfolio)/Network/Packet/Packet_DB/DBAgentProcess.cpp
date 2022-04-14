#include "../../../stdafx.h"
#include "DBAgentProcess.h"
#include "../../../Database/Query/QI_DB_REQ_ID_PW.h"


DBAgentProcess::DBAgentProcess()
{
	this->RegistSubPacketFunc();
}

void DBAgentProcess::RegistSubPacketFunc()
{
#define INSERT_PACKET_PROCESS(type) m_runFuncTable.insert(make_pair(E_##type, &DBAgentProcess::##type))

	INSERT_PACKET_PROCESS(I_DB_REQ_ID_PW);
	INSERT_PACKET_PROCESS(I_DB_REQ_LOAD_DATA);
}

void DBAgentProcess::I_DB_REQ_ID_PW(Session* session, Packet* rowPacket)
{
	PK_I_DB_REQ_ID_PW* packet = (PK_I_DB_REQ_ID_PW*)rowPacket;

	QI_DB_REQ_ID_PW* query = new QI_DB_REQ_ID_PW();
	query->clientId = packet->clientId;

	QueryStatement* statement = query->GetStatement();
	statement->AddParam((char*)packet->id.c_str());
	statement->AddParam((char*)packet->password.c_str());

	DBManager::GetInstance().PushQuery(query);
}

void DBAgentProcess::I_DB_REQ_LOAD_DATA(Session* session, Packet* rowPacket)
{
	PK_I_DB_REQ_LOAD_DATA* packet = (PK_I_DB_REQ_LOAD_DATA*)rowPacket;

	//QI_DB_REQ_LOAD_DATA* query = new QI_DB_REQ_LOAD_DATA();
	
}

