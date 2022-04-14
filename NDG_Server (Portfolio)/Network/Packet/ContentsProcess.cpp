#
#include "../../stdafx.h"
#include "ContentsProcess.h"




ContentsProcess::ContentsProcess()
{
	xml_t config;
	if (!loadConfig(&config))
	{
		return;
	}
	this->Initialize(&config);
}

ContentsProcess::~ContentsProcess()
{
	SAFE_DELETE(m_packageQueue);

	for (auto thread : m_threadPool)
	{
		SAFE_DELETE(thread);
	}
	m_runFuncTable.clear();
}

void ContentsProcess::Initialize(xml_t* config)
{
	xmlNode_t* root = config->FirstChildElement("App")->FirstChildElement("Contents");
	if (!root)
	{
		SERRLOG(L"ContentsProcess : not exist process setting !!!");
		return;
	}

	xmlNode_t* element = root->FirstChildElement("ThreadCount");
	int processCount = 0;
	sscanf_s(element->GetText(), "%d", &processCount);

	if (MAX_PACKET_THREAD_ < processCount)
	{
		SERRLOG(L"ContentesProcess : processThread Limit[%d], but config setting [%d]", MAX_PACKET_THREAD_, processCount);
		return;
	}

	m_packageQueue = new ThreadJobQueue<Package*>(L"ContentsProcessQueue");
	for (int i = 0; i < processCount; ++i)
	{
		m_threadPool[i] = MAKE_THREAD(ContentsProcess, Process);
	}
	this->RegistDefaultPacketFunc();
}

void ContentsProcess::RegistDefaultPacketFunc()
{
	
}

void ContentsProcess::PutPackage(Package* package)
{
	m_packageQueue->Push(package);
}

void ContentsProcess::Run(Package* package)
{
	PacketType type = package->m_packet->Type();
	RunFunc runFunction = m_runFuncTable.at(type);

	if (runFunction == nullptr)
	{
		SLOG(L"ContentsProcess  : invaild packet runFunction. type[%d]", type);
		package->m_session->OnClose();
		return;
	}

	SLOG(L"ContentsProcess : [%d] packet run", type);
	runFunction(package->m_session, package->m_packet);
}

void ContentsProcess::Execute()
{
	Package* package = nullptr;
	if (m_packageQueue->Pop(package) == false)
	{
		return;
	}
	this->Run(package);
	SAFE_DELETE(package);
}

void ContentsProcess::Process()
{
	while (_shutdown == false)
	{
		this->Execute();
		CONTEXT_SWITCH;
	}
}

