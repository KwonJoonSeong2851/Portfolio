#pragma once
#pragma once
#include "../../stdafx.h"


class QI_DB_REQ_GAMERESULT : public Query
{
public:

	oid_t clientId;

	const WCHAR* Procedure()
	{
		return L"call p_User_GameResult(";
	}

	QI_DB_REQ_GAMERESULT()
	{
		m_statement->SetQuery(this->Procedure(), QUERY_NOT_RETURN);
	}

	~QI_DB_REQ_GAMERESULT()
	{
		SLOG(L"Query : GameResult");
	}
};