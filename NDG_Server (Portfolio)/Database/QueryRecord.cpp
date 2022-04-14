#include "../stdafx.h"
#include "QueryRecord.h"

QueryRecord::QueryRecord()
{
	m_record.CreateInstance(__uuidof(ADODB::Recordset));
}

QueryRecord::~QueryRecord()
{
	if (m_record == nullptr)
	{
		return;
	}

	m_record.Release();
	m_record = nullptr;
}

void QueryRecord::ErrorReport(_com_error& e)
{
	SLOG(L"QueryRecord : error = %s", e.Description());
	cout  << e.Description() <<endl;
}


recordPtr& QueryRecord::GetResultRecord()
{
	return m_record;
}

void QueryRecord::operator=(QueryRecord& lvalue)
{
	m_record = lvalue.GetResultRecord();
}

void QueryRecord::operator=(recordPtr& lvalue)
{
	m_record = lvalue;
}

void QueryRecord::SetRecord(recordPtr record)
{
	m_record = record;
}

bool QueryRecord::Opened()
{
	return m_record->State == ADODB::adStateOpen ? true : false;
}

void QueryRecord::Close()
{
	try
	{
		if (m_record != nullptr && this->Opened())
			m_record->Close();
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
	}
}

bool QueryRecord::IsEof()
{
	if (m_record == nullptr)
	{
		return true;
	}

	try 
	{
		return m_record->EndOfFile ? true : false;
	}
	catch(_com_error &e)
	{
		this->ErrorReport(e);
	}
	return false;
}

int QueryRecord::ResultVal()
{
	return m_resultVal;
}

void QueryRecord::SetResultVal(int result)
{
	m_resultVal = result;
}

HRESULT QueryRecord::MoveNext()
{
	try
	{
		return m_record->MoveNext();
	}
	catch (_com_error &e)
	{
		this-> ErrorReport(e);
	}
	return S_FALSE;
}

HRESULT QueryRecord::MovePrevious()
{
	try
	{
		return m_record->MovePrevious();
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
	}
	return S_FALSE;
}

HRESULT QueryRecord::MoveFirst()
{
	try
	{
		return m_record->MoveFirst();
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
	}
	return S_FALSE;
}

HRESULT QueryRecord::MoveLast()
{
	try
	{
		return m_record->MoveLast();
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
	}
	return S_FALSE;
}

bool QueryRecord::Get(const char* fieldName, char* fieldValue)
{
	try
	{
		_variant_t vtValue;
		vtValue = m_record->Fields->GetItem(fieldName)->GetValue();
		sprintf_s(fieldValue, DB_PARAM_SIZE, "%s", (LPCSTR)((_bstr_t)vtValue.bstrVal));
		return true;
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
		SLOG(L"QueryRecord : error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::Get(const char* fieldName,  wchar_t* fieldValue)
{
	try
	{
		_variant_t vtValue;
		vtValue = m_record->Fields->GetItem(fieldName)->GetValue();
		swprintf_s(fieldValue, DB_PARAM_SIZE, L"%s", (LPWSTR)((_bstr_t)vtValue.bstrVal));
		return true;
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
		SLOG(L"QueryRecord : error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::Get(const char* fieldName,  int& fieldValue)
{
	try
	{
		_variant_t vtValue;
		vtValue = m_record->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.intVal;
		return true;
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
		SLOG(L"QueryRecord : error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::Get(const char* fieldName, float& fieldValue)
{
	try
	{
		_variant_t vtValue;
		vtValue = m_record->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.fltVal;
		return true;
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
		SLOG(L"QueryRecord : error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::Get(const char* fieldName, double& fieldValue)
{
	try
	{
		_variant_t vtValue;
		vtValue = m_record->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.dblVal;
		return true;
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
		SLOG(L"QueryRecord : error query field : %S", fieldName);
	}
	return false;
}

bool QueryRecord::Get(const char* fieldName, long& fieldValue)
{
	try
	{
		_variant_t vtValue;
		vtValue = m_record->Fields->GetItem(fieldName)->GetValue();
		fieldValue = vtValue.lVal;
		return true;
	}
	catch (_com_error & e)
	{
		this->ErrorReport(e);
		SLOG(L"QueryRecord : error query field : %S", fieldName);
	}
	return false;
}

