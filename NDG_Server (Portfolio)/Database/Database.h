#pragma once
#include "../stdafx.h"

typedef enum
{
	DB_STOP,
	DB_STANDBY,
	DB_RUNNING,
}DB_STATE;


class Database
{
protected:
	DB_STATE m_state;

public:
	Database() {}
	virtual ~Database() {}

	virtual bool Connect(const WCHAR* serverName, const WCHAR* dbName, const WCHAR* id, const WCHAR* password) = 0;
	virtual bool Connect() = 0;
	virtual bool Connected() = 0;
	virtual bool Disconnect() = 0;

	virtual void Run() = 0;
	DB_STATE& GetState() { return m_state; }
};