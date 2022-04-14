#pragma once
#include "../stdafx.h"
#include "ADODatabase.h"


typedef enum
{
	QUERY_NOT_RETURN,
	QUERY_WAIT_RETURN,
	QUERY_CALL_BACK,
}QUERY_TYPE;

class QueryStatement
{
	ADODB::_CommandPtr m_command;
	QUERY_TYPE m_type;

	wstr_t m_query;
	int m_paramCount;

public:
	QueryStatement();
	~QueryStatement();

	void SetQuery(const WCHAR* query, QUERY_TYPE type = QUERY_NOT_RETURN);

	const WCHAR* GetQuery();
	QUERY_TYPE GetType();

	//-----------------------------------------------------------//
	template<typename T>
	void AddArg(const WCHAR* fmt, T value);

	void AddParam(const CHAR* value);
	void AddParam(const WCHAR* value);
	void AddParam(INT32* value);
	void AddParam(INT64* value);
	void AddParam(UINT32* value);
	void AddParam(UINT64* value);
	void AddParam(FLOAT* value);
	void AddParam(DOUBLE* value);

	void AddOutParam(const CHAR* value);
	void EndParam();


};