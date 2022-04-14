#include "TerminalManager.h"

TerminalManager::TerminalManager()
{
}

TerminalManager::~TerminalManager()
{
	for (auto itr : m_terminalPool)
	{
		auto terminal = itr.second;
		SAFE_DELETE(terminal);
	}
}

void TerminalManager::Initialize(xml_t* config)
{
	xmlNode_t* root = config->FirstChildElement("App")->FirstChildElement("Terminal");
	xmlHandle_t terminalNode = TiXmlHandle(root);

	if (!root)
	{
		SLOG(L"TerminalManager : not exist terminal setting !!!");
		return;
	}

	xmlNode_t* element = terminalNode.FirstChildElement().Element();

	while (element)
	{
		array<WCHAR, _MAX_PATH> terminalName;
		StrConvA2W((char*)element->Value(), terminalName.data(), terminalName.max_size());

		Terminal* terminal = new Terminal(m_server, terminalName.data());
		terminal->Initialize(element);
		this->Put(terminalName.data(), terminal);

		element = element->NextSiblingElement();
	}

	SLOG(L"### Terminal Set ###");
}

void TerminalManager::Put(wstr_t serverName, Terminal* terminal)
{
	m_terminalPool.insert(make_pair(serverName, terminal));
}

Terminal* TerminalManager::GetTerminal(wstr_t name)
{
	return m_terminalPool.at(name);
}

bool TerminalManager::IsTerminal(const char* ip)
{
	for (auto terminal : m_terminalPool)
	{
		if (!strcmp(terminal.second->GetIp(), ip))
		{
			return true;
		}
	}
	return false;
}

void TerminalManager::Run(Server* server)
{
	m_server = server;
	xml_t config;
	if (!loadConfig(&config))
	{
		return;
	}
	this->Initialize(&config);
}
