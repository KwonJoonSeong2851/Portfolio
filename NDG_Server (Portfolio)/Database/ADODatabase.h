#pragma once
#include "../stdafx.h"
#include <Ole2.h>
#include <comdef.h>
#include <comutil.h>
#include <conio.h>

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF","EndOfFile")
#include "Database.h"

typedef ADODB::_ConnectionPtr dbConnectionPtr;
typedef ADODB::_CommandPtr commandPtr;
typedef ADODB::_RecordsetPtr recordPtr;

class ADODatabase :public Database
{
	dbConnectionPtr m_dbConnection;
	wstr_t m_connectionStr;
	wstr_t m_dbName;

	Thread* m_thread;

public:
	ADODatabase();
	virtual ~ADODatabase();

	HRESULT SetConnectionTimeOut(long second);
	void ComError(const WCHAR* actionName, _com_error& e);

	bool Connect(const WCHAR* provider, const WCHAR* serverName, const WCHAR* dbName, const WCHAR* id, const WCHAR* password);
	bool Connect(const WCHAR* serverName, const WCHAR* dbName, const WCHAR* id, const WCHAR* password);
	bool Connect();
	bool Connected();
	bool Disconnect();

private:
	void Execute();
	void Process();
	void Run();
};