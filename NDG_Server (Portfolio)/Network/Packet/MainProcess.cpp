#include "../../../stdafx.h"
#include "MainProcess.h"
#include "../../../Database/Query/QI_DB_REQ_ID_PW.h"
#include "../../../Database/Query/QI_DB_REQ_SIGNIN.h"
#include "../../../Database/Query/QI_DB_REQ_USERINFO.h"
#include "../../../Database/Query/QI_DB_REQ_GAMERESULT.h"
#include "../../../Database/Query/QI_DB_REQ_TUTORIAL.h"
#include <atlstr.h>

MainProcess::MainProcess()
{
	this->RegistSubPacketFunc();
}

void MainProcess::RegistSubPacketFunc()
{
#define INSERT_PACKET_PROCESS(type) m_runFuncTable.insert(make_pair(E_##type, &MainProcess::##type))

	INSERT_PACKET_PROCESS(C_REQ_ID_PW);
	INSERT_PACKET_PROCESS(C_REQ_SIGNIN);
	INSERT_PACKET_PROCESS(C_REQ_USERINFO);
	INSERT_PACKET_PROCESS(C_REQ_GAMERESULT);
	INSERT_PACKET_PROCESS(C_REQ_CHATTING);
	INSERT_PACKET_PROCESS(C_REQ_GET_TUTORIAL);
	INSERT_PACKET_PROCESS(C_REQ_SET_TUTORIAL);
}


void MainProcess::C_REQ_ID_PW(Session* session, Packet* rowPacket)
{


	PK_C_REQ_ID_PW* packet = (PK_C_REQ_ID_PW*)rowPacket;

	QI_DB_REQ_ID_PW* query = new QI_DB_REQ_ID_PW();
	query->clientId = session->GetId ();

	QueryStatement* statement = query->GetStatement();
	statement->AddParam((char*)packet->id.c_str());
	statement->AddParam((char*)packet->password.c_str());
	statement->EndParam();
	DBManager::GetInstance().PushQuery(query);

}

void MainProcess::C_REQ_SIGNIN(Session* session, Packet* rowPacket)
{
	PK_C_REQ_SIGNIN* packet = (PK_C_REQ_SIGNIN*)rowPacket;

	QI_DB_REQ_SIGNIN* query = new QI_DB_REQ_SIGNIN();
	query->clientId = session->GetId();


	array<WCHAR, SIZE_256> nickName;




	int len = MultiByteToWideChar(CP_UTF8, 0, packet->userNickName.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, packet->userNickName.c_str(), -1, nickName.data(), len);


	QueryStatement* statement = query->GetStatement();
	statement->AddParam((char*)packet->userId.c_str());
	statement->AddParam((char*)packet->userPwd.c_str());
	statement->AddParam(nickName.data());

	
	statement->EndParam();
	DBManager::GetInstance().PushQuery(query);


}

void MainProcess::C_REQ_USERINFO(Session* session, Packet* rowPacket)
{
	PK_C_REQ_USERINFO* packet = (PK_C_REQ_USERINFO*)rowPacket;

	QI_DB_REQ_USERINFO* query = new QI_DB_REQ_USERINFO();
	query->clientId = session->GetId(); 

	QueryStatement* statement = query->GetStatement();
	statement->AddParam((UINT64*)packet->account);
	statement->EndParam();
	DBManager::GetInstance().PushQuery(query);
}

void MainProcess::C_REQ_GAMERESULT(Session* session, Packet* rowPacket)
{
	PK_C_REQ_GAMERESULT* packet = (PK_C_REQ_GAMERESULT*)rowPacket;
	QI_DB_REQ_GAMERESULT* query = new QI_DB_REQ_GAMERESULT();
	query->clientId = session->GetId();

	QueryStatement* statement = query->GetStatement();
	statement->AddParam((UINT64*)packet->account);
	statement->AddParam((UINT32*)packet->gameResult);
	statement->AddParam((UINT64*)packet->experience);
	statement->EndParam();
	DBManager::GetInstance().PushQuery(query);
}




void MainProcess::C_REQ_CHATTING(Session* session, Packet* rowPacket)
{
	PK_C_REQ_CHATTING* packet = (PK_C_REQ_CHATTING*)rowPacket;
	PK_S_ANS_CHATTING sendPacket;

	array<WCHAR, SIZE_256> buff;

	int len = MultiByteToWideChar(CP_UTF8, 0, packet->m_id.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, packet->m_id.c_str(), -1, buff.data(), len);
	sendPacket.m_id = buff.data(); 
	buff.fill(NULL);

	len = MultiByteToWideChar(CP_UTF8, 0, packet->m_text.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, packet->m_text.c_str(), -1, buff.data(), len);
	sendPacket.m_text = buff.data();

	
	sendPacket.characterType = packet->characterType;
	sendPacket.team = packet->team;
	sendPacket.session = packet->session;
	sendPacket.roomNumber = packet->roomNumber;


	SAFE_LOCK(SessionManager::GetInstance().GetLock());
	auto playerList = SessionManager::GetInstance().GetSessionList();

	SLOG(L"Player list length: %d", playerList.size());

	for (auto player : playerList)
	{
		player->SendPacket(&sendPacket);
	}

	return;
}
void MainProcess::C_REQ_GET_TUTORIAL(Session* session, Packet* rowPacket)
{
	PK_C_REQ_GET_TUTORIAL* packet = (PK_C_REQ_GET_TUTORIAL*)rowPacket;

	QI_DB_REQ_GET_TUTORIAL* query = new QI_DB_REQ_GET_TUTORIAL();
	query->clientId = session->GetId();


	QueryStatement* statement = query->GetStatement();
	statement->AddParam((UINT64*)packet->accountId);
	statement->EndParam();
	DBManager::GetInstance().PushQuery(query);


	SLOG(L"TUTORIAL Player Account: %u", packet->accountId);
	
	cout << packet->accountId << endl;
}



void MainProcess::C_REQ_SET_TUTORIAL(Session* session, Packet* rowPacket)
{
	PK_C_REQ_SET_TUTORIAL* packet = (PK_C_REQ_SET_TUTORIAL*)rowPacket;

	QI_DB_REQ_SET_TUTORIAL* query = new QI_DB_REQ_SET_TUTORIAL();
	query->clientId = session->GetId();

	QueryStatement* statement = query->GetStatement();
	statement->AddParam((UInt64*)packet->accountId);
	statement->EndParam();
	DBManager::GetInstance().PushQuery(query);

}


