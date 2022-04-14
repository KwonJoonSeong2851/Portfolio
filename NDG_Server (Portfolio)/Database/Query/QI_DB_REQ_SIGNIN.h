#pragma once
#include "../../stdafx.h"


class QI_DB_REQ_SIGNIN : public Query
{
public:
	oid_t clientId;

	const WCHAR* Procedure()
	{
		return L"call p_User_Insert_v5(";
	}

	QI_DB_REQ_SIGNIN()
	{
		m_statement->SetQuery(this->Procedure(), QUERY_CALL_BACK);
	}

	~QI_DB_REQ_SIGNIN()
	{
		PK_I_ANS_SIGNIN iPacket;
		
		
		if (!m_record.IsEof())
		{
			m_record.MoveFirst();
		}


		INT32 result;
		m_record.Get("result", result);
		iPacket.result = result;

			
		


		SLOG(L"Query : Result = %d", iPacket.result);
		Session* clientSession = SessionManager::GetInstance().GetSession(clientId);
		clientSession->SendPacket(&iPacket);
	}
};
