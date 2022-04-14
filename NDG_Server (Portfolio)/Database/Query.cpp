#pragma once
#include "../stdafx.h"
#include "QueryStatement.h"
#include "QueryRecord.h"
#include "Query.h"
#include "ADODatabase.h"
#include "DBManager.h"

Query::Query()
{
	m_statement = new QueryStatement();
}

Query::~Query()
{
	m_record.Close();
	SAFE_DELETE(m_statement);
}

void Query::SetResult(recordPtr record)
{
	m_record.SetRecord(record);
}

QueryRecord& Query::GetResult()
{
	return m_record;
}

void Query::SetStatement(QueryStatement* statement)
{
	m_statement = statement;
}

QueryStatement* Query::GetStatement()
{
	return m_statement;
}

