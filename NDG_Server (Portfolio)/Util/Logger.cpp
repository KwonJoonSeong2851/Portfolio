#include "Logger.h"
#include "../stdafx.h"

LogPrintf::LogPrintf()
{
	printf("Log Create: printf log mode \n");
}

void LogPrintf::Log(const WCHAR* logStr)
{
	printf("%ws", logStr);
}


LogFile::LogFile(xml_t* config)
{
	xmlNode_t* root = config->FirstChildElement("App")->FirstChildElement("Log");
	xmlNode_t* element = root->FirstChildElement("Path");

	std::array<WCHAR, _MAX_PATH> logFilePath;
	StrConvA2W((char*)element->GetText(), logFilePath.data(), logFilePath.max_size());
	printf("Log Create : [%ws]file log mode \n", logFilePath.data());
	this->Initialize(logFilePath.data());
}


LogFile::~LogFile()
{
	m_fs.close();
	m_fs.clear();

	std::size_t found = m_fileName.find(L".log");
	if (found == wstr_t::npos)
	{
		return;
	}

	wstr_t closeFileName = m_fileName.substr(0, found);
	closeFileName += CLOCK.NowTime(L"%Y%m%d-%H%M%S.log");
	_wrename(m_fileName.c_str(), closeFileName.c_str());
}


void LogFile::Initialize(const WCHAR* logFileName)
{
	m_fileName = logFileName;
	m_fs.open(logFileName, std::ios::out | std::ios::trunc);

	if (m_fs.bad())
	{
		printf("! LogFile Error : file open fail. \n");
		assert(false);
	}
}

void LogFile::Log(const WCHAR* logStr)
{
	printf("%ws", logStr);

	m_fs << logStr;
	m_fs.flush();
}


LogWriter::LogWriter()
{
	m_base = nullptr;
}

LogWriter::~LogWriter()
{
	m_prefix.clear();
	SAFE_DELETE(m_base);
}

void LogWriter::SetLogger(LogBase* base, const WCHAR* logPrefix)
{
	m_prefix.clear();
	m_prefix = logPrefix;

	if (m_base)
	{
		LogBase* old = m_base;
		m_base = nullptr;
		old->UnInitialize();

		SAFE_DELETE(old);
	}

	m_base = base;
	m_base->Initialize();
}

LogBase* LogWriter::Logger()
{
	return m_base;
}

void LogWriter::Log(const WCHAR* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	this->Log(fmt, args);
	va_end(args);
}

void LogWriter::Log(const WCHAR* fmt, va_list args)
{
	wstr_t logMessage = CLOCK.NowTimeWithMilliSec();
	size_t threadId = GET_CURRENT_THREAD_ID();

	logMessage += L"\t";

	Thread* thread = ThreadManager::GetInstance().At(threadId);
	if (thread)
	{
		logMessage += thread->GetName();
	}
	else
	{
		logMessage += m_prefix;
	}



	array<WCHAR, SIZE_8 * 2> threadIdStr;
	snwprintf(threadIdStr, L"0x%X", threadId);

	logMessage += L":";
	logMessage += threadIdStr.data();
	logMessage += L"\t";

	array<WCHAR, SIZE_1024> logStr;
	vswprintf_s(logStr.data(), logStr.size(), fmt, args);

	logMessage += logStr.data();
	logMessage += L"\n";
	m_base->Log(logMessage.c_str());
}



SystemLog::SystemLog()
{
	xml_t config;
	if (!loadConfig(&config))
	{
		printf("!!! SystemLog : have not config file \n");
		exit(0);
		return;
	}
	this->Initialize(&config);
}

SystemLog::~SystemLog()
{
}

void SystemLog::Initialize(xml_t* config)
{
	xmlNode_t* root = config->FirstChildElement("App")->FirstChildElement("Log");
	if (!root)
	{
		printf("SystemLog : not exist log setting");
		LogBase* base = new LogPrintf();
		m_logWrite.SetLogger(base, L"NDG_Server");
		return;
	}
	xmlNode_t* element = root->FirstChildElement("Path");

	array<WCHAR, SIZE_256> tmp;
	element = root->FirstChildElement("Prefix");
	StrConvA2W((Char*)element->GetText(), tmp.data(), tmp.max_size());
	wstr_t prefix = tmp.data();


	LogBase* base;
	element = root->FirstChildElement("Type");
	const char* type = (char*)element->GetText();

	if (!strcmp(type, "WithFile"))
	{
		base = new LogFile(config);
	}
	else
		base = new LogPrintf();

	m_logWrite.SetLogger(base, prefix.c_str());
}

void SystemLog::Log(const WCHAR* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	m_logWrite.Log(fmt, args);
	va_end(args);
}
