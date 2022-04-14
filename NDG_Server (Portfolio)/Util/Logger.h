#pragma once
#include "../stdafx.h"
#include <fstream>
#include "Singleton.h"
#include "Thread.h"

//-------------------------------------------------//
/*

         로그 출력 및 저장 클래스
 

*/
//-------------------------------------------------//




#define SLOG(arg, ...)  SystemLog::GetInstance().Log(arg, __VA_ARGS__);
#define SERRLOG(arg, ...) SystemLog::GetInstance().Log(arg,__VA_ARGS__); :: ExitProcess(0);


class LogBase
{
public:
	LogBase() {}
	virtual ~LogBase() {}
	virtual void Initialize() {}
	virtual void UnInitialize() {}
	virtual void Log(const WCHAR* logStr) = 0;
};


//-------------------------------------------------//

class LogPrintf : public LogBase
{
public:
	LogPrintf();
	void Log(const WCHAR* logStr);
};


//-------------------------------------------------//

class LogFile : public LogBase
{
private:
	std::wfstream     m_fs;
	wstr_t            m_fileName;

public:
	LogFile(xml_t* config);
	virtual ~LogFile();

	void Initialize() {}
	void Initialize(const WCHAR* logFileName);
	void Log(const WCHAR* logStr);
};


//-------------------------------------------------//

class LogWriter
{
private:
	LogBase* m_base;
	wstr_t   m_prefix;
	
public:
	LogBase* Logger();

	LogWriter();
	virtual ~LogWriter();

	void SetLogger(LogBase* base, const WCHAR* logPrefix);
	
	void Log(const WCHAR* fmt, ...);
	void Log(const WCHAR* fmt, va_list args);
	
};

//-------------------------------------------------//

class SystemLog : public Singleton<SystemLog>
{
private:
	LogWriter m_logWrite;
public:
	SystemLog();
	virtual ~SystemLog();

	void Initialize(xml_t* config);
	void Log(const WCHAR* fmt, ...);
};