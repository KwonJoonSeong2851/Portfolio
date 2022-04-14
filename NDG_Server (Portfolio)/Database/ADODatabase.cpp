#include "../stdafx.h"
#include "ADODatabase.h"
#include "QueryStatement.h"
#include "DBManager.h"

ADODatabase::ADODatabase()
{
	::CoInitialize(NULL);
	m_state = DB_STOP;

	m_dbConnection.CreateInstance(__uuidof(ADODB::Connection));
	if (m_dbConnection == NULL)
	{
		SERRLOG(L"ADODatabase : Database init fail !");
	}
	const int TIME_OUT = 30;
	this->SetConnectionTimeOut(TIME_OUT);
}

ADODatabase::~ADODatabase()
{
	this->Disconnect();
	if (m_dbConnection)
	{
		m_dbConnection.Release();
	}
	SAFE_DELETE(m_thread);
	::CoUninitialize();
}

HRESULT ADODatabase::SetConnectionTimeOut(long second)
{
	if (!m_dbConnection)
	{
		return S_FALSE;
	}
	return m_dbConnection->put_ConnectionTimeout(second);
}

void ADODatabase::ComError(const WCHAR* actionName, _com_error& e)
{
	SLOG(L"ADODatabase : [%s]DB [%s] fail [%S]", m_dbName.c_str(), actionName, (WCHAR*)e.Description());
	cout << "AODatabase" << e.Description() << endl;
}

bool ADODatabase::Connect(const WCHAR* provider, const WCHAR* serverName, const WCHAR* dbName, const WCHAR* id, const WCHAR* password)
{
	array<WCHAR, SIZE_128> buffer;
	snwprintf(buffer, L"Provider=%s; Server=%s; Database =%s; Uid=%s; Pwd=%s;", provider, serverName, dbName, id, password);
	m_connectionStr = buffer.data();
	SLOG(L"ADODatabase : DB try connection provider = %s", m_dbName.c_str(), provider);
	return this->Connect();
}

bool ADODatabase::Connect(const WCHAR* serverName, const WCHAR* dbName, const WCHAR* id, const WCHAR* password)
{
	m_dbName = dbName;

	if (this->Connect(L"SQLNCL11", serverName, dbName, id, password))
	{
		return true;
	}
	false;
}

bool ADODatabase::Connect()
{
	if (!m_dbConnection)
	{
		return false;
	}

	try
	{
		//HRESULT hr = m_dbConnection->Open(m_connectionStr.c_str(), _T(""), _T(""), NULL);
		HRESULT hr = m_dbConnection->Open(
			"Driver = { MySQL ODBC 8.0 Unicode Driver}; DSN=Test; Server = 49.50.174.122;Port=1433;Database=GameDB;Uid=GameUser;Pwd=1234" , _T(""), _T(""), NULL);
		//Driver = { MySQL ODBC 5.4 Driver }; SERVER = 49.50.174.122; Port = 1433; DATABASE = GameDB; UID = GameUser; PWD = 1234; Option = 3;
		if (SUCCEEDED(hr))
		{
			m_dbConnection->PutCursorLocation(ADODB::adUseClient);
			SLOG(L"ADODatabase [%s]DB connection success", m_dbName.c_str());
			m_state = DB_STANDBY;
			return true;
		}
	}
	catch (_com_error & e)
	{
		this->ComError(L"ADODatabase Connection",e);
	}
	return false;

}

bool ADODatabase::Connected()
{
	return m_dbConnection->State != ADODB::adStateClosed ? true : false;
}

bool ADODatabase::Disconnect()
{
	if (m_dbConnection)
		return false;

	if (m_state == DB_STOP)
		return true;

	try
	{
		this->Execute();

		if(!m_dbConnection)
		{
			return true;
		}

		m_dbConnection->Close();
		m_state = DB_STOP;

		m_connectionStr.clear();
		m_dbName.clear();
		SLOG(L"ADODatabase : database close");
		return true;
	}
	catch (...)
	{
		SLOG(L"ADODatabase : Database[%s] disconnect fail", m_dbName.c_str());
	}
	return false;
}

void ADODatabase::Execute()
{
	if (DBManager::GetInstance().RunQueryCount() == 0)
	{
		return;
	}

	Query* query = nullptr;
	if (DBManager::GetInstance().PopQuery(&query) == false)
	{
		return;
	}

	QueryStatement* statement = query->GetStatement();

	const WCHAR* sqlQuery = statement->GetQuery();
	try
	{
		m_state = DB_RUNNING;
		QueryRecord record;

		ADODB::_CommandPtr command;
	
		command.CreateInstance(__uuidof(ADODB::Command));
		command->ActiveConnection = m_dbConnection;
		command->CommandType = ADODB::adCmdText;
		command->CommandText = sqlQuery;
		_variant_t  resultVal;


		switch (statement->GetType())
		{
		case QUERY_NOT_RETURN:
			record = (recordPtr&)(command->Execute(&resultVal, NULL, ADODB::adCmdText | ADODB::adExecuteNoRecords));
			break;

		case QUERY_WAIT_RETURN:
			record = (recordPtr&)command->Execute(&resultVal, NULL, ADODB::adCmdText | ADODB::adExecuteRecord);
			break;

		case QUERY_CALL_BACK:
			record = (recordPtr&)command->Execute(&resultVal, NULL, ADODB::adCmdText | ADODB::adAsyncFetchNonBlocking);
			break;
		}

		if (record.IsEof())
		{
			int quertResultVal = atol((char*)((_bstr_t)resultVal));
			if (quertResultVal < 1)
			{
				SLOG(L"ADODDatabase : [%s] have error code [%d]", sqlQuery, quertResultVal);
			}
			else
			{
				record.SetResultVal(quertResultVal);
			}
		}

		query->GetResult() = record;
		m_state = DB_STANDBY;

		SLOG(L"ADODDatabase : Run query [%s] result [%d]", sqlQuery, record.ResultVal());
	}
	catch (_com_error & e)
	{
		this->ComError(L"execute", e);
	}

	SAFE_DELETE(query);
}

void ADODatabase::Process()
{
	while (_shutdown == false)
	{
		if (!this->Connected())
		{
			SLOG(L"ADODatabase : db[%s] connection disconnect", m_dbName.c_str());
			ASSERT(FALSE);
		}
		this->Execute();

		CONTEXT_SWITCH;
	}
}

void ADODatabase::Run()
{
	m_thread = MAKE_THREAD(ADODatabase, Process);
}



