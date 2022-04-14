#pragma once
#include "../stdafx.h"
#include "ADODatabase.h""
#include "QueryRecord.h""
#include "QueryStatement.h"

QueryStatement::QueryStatement()
{
	m_paramCount = 0;
	m_query.clear();
}

QueryStatement::~QueryStatement()
{
}

void QueryStatement::SetQuery(const WCHAR* query, QUERY_TYPE type)
{
	m_query = query;
	m_type = type;
}

const WCHAR* QueryStatement::GetQuery()
{
	return m_query.c_str();
}

QUERY_TYPE QueryStatement::GetType()
{
	return m_type;
}

template<typename T>
void QueryStatement::AddArg(const WCHAR* fmt, T value)
{
	array<WCHAR, DB_PARAM_SIZE> buffer;
	snwprintf(buffer, fmt, value);

	if (m_paramCount++)
	{
		m_query += L", ";
	}
	else
	{
		m_query += L" ";
	}
	m_query += buffer.data();
}



void QueryStatement::AddParam(const CHAR* value)
{
	this->AddArg(L"'%S'", value);
}

void QueryStatement::AddParam(const WCHAR* value)
{
	this->AddArg(L"'%s'", value);
}


void QueryStatement::AddParam(INT32* value)
{
	this->AddArg(L"'%d'", value);
}


void QueryStatement::AddParam(UINT32* value)
{
	this->AddArg(L"'%u'", value);
}


void QueryStatement::AddParam(INT64* value)
{
	this->AddArg(L"'%lld'", value);
}


void QueryStatement::AddParam(UINT64* value)
{
	this->AddArg(L"'%llu'", value);
}


void QueryStatement::AddParam(FLOAT* value)
{
	this->AddArg(L"'%f'", value);
}
 

void QueryStatement::AddParam(DOUBLE* value)
{
	this->AddArg(L"'%lf'", value);
}

void QueryStatement::AddOutParam(const CHAR* value)
{
	this->AddArg(L"@%S", value);
}

void QueryStatement::EndParam()
{
	m_query += L")";
}
