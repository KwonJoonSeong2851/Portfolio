#pragma once
#include "../../stdafx.h"

class QI_DB_REQ_USERINFO : public Query
{

private:

public:
	oid_t clientId;

	const WCHAR* Procedure()
	{
		return L"call p_Select_User(";
	}

	QI_DB_REQ_USERINFO()
	{
		m_statement->SetQuery(this->Procedure(), QUERY_CALL_BACK);
	}

	~QI_DB_REQ_USERINFO()
	{
		PK_I_ANS_USERINFO iPacket;
		bool result = true;


		if (!m_record.IsEof())
		{
			m_record.MoveFirst();
		}

			long account = 0;
			if (m_record.Get("Account", account))
			{
				iPacket.account = account;
			}
			else result = false;

			array<WCHAR, DB_PARAM_SIZE> buffer;
			if (m_record.Get("id", buffer.data()))
			{
				array<CHAR, DB_PARAM_SIZE> idBuf;
				StrConvW2A(buffer.data(), idBuf.data(), idBuf.size());
				iPacket.id = idBuf.data();
				buffer.fill(NULL);
			}
			else result = false;

			
			//array<CHAR, DB_PARAM_SIZE> tempNickBuf;
			if (m_record.Get("nickname", buffer.data()))
			{
				iPacket.nickName = buffer.data();
			}
			else result = false;



			int level = 0;
			if (m_record.Get("level", level))
			{
				iPacket.level = level;
			}
			else result = false;


			long experience = 0;
			if (m_record.Get("experience", experience))
			{
				iPacket.experience = experience;
			}
			else result = false;


			int win = 0;
			if (m_record.Get("win", win))
			{
				iPacket.win = win;
			}
			else result = false;


			int lose = 0;
			if (m_record.Get("lose", lose))
			{
				iPacket.lose = lose;
			}
			else result = false;

			iPacket.result = result;


		
		
			if (result == false)
			{
				SLOG(L"QI_DB_REQ_ID_PW : this query [%s] have error", this->Procedure());
			}



	
		SLOG(L"Query : REQ_UserInfo UserID:%S, NickName:%s Result = %d", iPacket.id.c_str(),iPacket.nickName.c_str(),iPacket.result);
		Session* clientSession = SessionManager::GetInstance().GetSession(clientId);
		clientSession->SendPacket(&iPacket);
	}
};
