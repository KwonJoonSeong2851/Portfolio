#include "DBProcess.h"

DBProcess::DBProcess()
{
	this->RegistSubPacketFunc();
}

void DBProcess::RegistSubPacketFunc()
{
	m_runFuncTable.insert(make_pair(E_I_DB_REQ_ID_PW, &DBProcess::IPacket_ID_PW));
}

class Query_ID_PW : public Query
{
public:
	oid_t m_clientId;

	const WCHAR* Procedure()
	{
		return L"p_AccountData_Select";
	}

	//실행할 쿼리 설정
	Query_ID_PW()
	{
		m_statement->SetQuery(this->Procedure(), QUERY_CALL_BACK);
	}

	~Query_ID_PW()
	{
		PK_I_DB_ANS_ID_PW iPacket;
		iPacket.m_clientId = (UInt64)m_clientId;
		iPacket.m_result = FALSE;

		if (!m_record.IsEof())
		{
			m_record.MoveFirst();
		}

		while (!m_record.IsEof())
		{
			array<WCHAR, DB_PARAM_SIZE> buffer;

			if (m_record.Get("id", buffer.data()))
			{
				SLOG(L"id : %s", buffer.data());
				iPacket.result = TRUE;
			}
			else
			{
				SLOG(L"DBProcess : this query [%s] have error", this->Procedure());
				break;
			}
			m_record.MoveNext();
		}
	}


};

