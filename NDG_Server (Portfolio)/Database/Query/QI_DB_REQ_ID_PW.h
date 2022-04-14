#pragma once
#include "../../stdafx.h"


class QI_DB_REQ_ID_PW : public Query
{
public:

	oid_t clientId;

	const WCHAR* Procedure()
	{
		return L"call p_AccountData_Select(";
	}

	QI_DB_REQ_ID_PW()
	{
		m_statement->SetQuery(this->Procedure(), QUERY_CALL_BACK);
	}

	~QI_DB_REQ_ID_PW()
	{
		PK_I_ANS_ID_PW iPacket;
		iPacket.accountId = (UInt64)0;
		iPacket.result = FALSE;

		if (!m_record.IsEof())
		{
			m_record.MoveFirst();
		}

			int oidAccount = 0;
			if (m_record.Get("Account", oidAccount))
			{
				iPacket.accountId = oidAccount;
				iPacket.result = TRUE;
			}
			else
			{
				SLOG(L"QI_DB_REQ_ID_PW : this query [%s] have error", this->Procedure());
			}




		SLOG(L"Query : ID/PW Result = %d", iPacket.result);
		Session* clientSession = SessionManager::GetInstance().GetSession(clientId);
		clientSession->SendPacket(&iPacket);
	}
};