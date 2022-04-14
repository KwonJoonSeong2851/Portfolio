#pragma once


#include "../../stdafx.h"


class QI_DB_REQ_GET_TUTORIAL : public Query
{
public:

	oid_t clientId;

	const WCHAR* Procedure()
	{
		return L"call p_GetTutorial(";
	}

	QI_DB_REQ_GET_TUTORIAL()
	{
		m_statement->SetQuery(this->Procedure(), QUERY_CALL_BACK);
	}

	~QI_DB_REQ_GET_TUTORIAL()
	{
		PK_S_ANS_GET_TUTORIAL packet;
		

		if (!m_record.IsEof())
		{
			m_record.MoveFirst();
		}
		Int32 result;
		m_record.Get("IsTutorial", result);

		packet.m_isTutorial = result;
			
		

		SLOG(L"Query : GetTutorial = %d",result);
		Session* clientSession = SessionManager::GetInstance().GetSession(clientId);
		clientSession->SendPacket(&packet);
	}
};


class QI_DB_REQ_SET_TUTORIAL : public Query
{
public:

	oid_t clientId;

	const WCHAR* Procedure()
	{
		return L"call p_SetTutorial(";
	}

	QI_DB_REQ_SET_TUTORIAL()
	{
		m_statement->SetQuery(this->Procedure(), QUERY_NOT_RETURN);
	}

	~QI_DB_REQ_SET_TUTORIAL()
	{
		SLOG(L"Query : SetTutorial");
	}
};