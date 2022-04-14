#pragma once
#include "../stdafx.h"
#include "ADODatabase.h"

#include "QueryRecord.h"
#include "QueryStatement.h"


class Query
{
protected:
	QueryStatement* m_statement;
	QueryRecord m_record;

public:
	Query();
	virtual ~Query();

	void SetResult(recordPtr record);
	QueryRecord& GetResult();

	void SetStatement(QueryStatement* statement);
	QueryStatement* GetStatement();
};