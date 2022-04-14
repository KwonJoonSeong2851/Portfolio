#include "Task.h"
#include "../stdafx.h"


TaskNode::TaskNode(Work* workObject, int freqSec, int durationSec)
{
	m_workObject = workObject;
	m_freqSec = freqSec;
	m_durationSec = durationSec;
	this->NextTick();
}

TaskNode::~TaskNode()
{
	SAFE_DELETE(m_workObject);
}

void TaskNode::NextTick()
{
	m_nextTick = NOW_TICK + (tick_t)m_freqSec;
}

bool TaskNode::Expired()
{
	if (m_workObject == nullptr)
	{
		return true;
	}

	if (m_durationSec != TICK_INFINITY)
	{
		if (m_durationSec < NOW_TICK)
		{
			return true;
		}
	}
	return false;
}

void TaskNode::Tick()
{
	if (m_nextTick < NOW_TICK)
	{
		m_workObject->Tick();
	}
}


//-------------------------------------------------//


Task::Task(int id)
{
	m_id = id;
}

Task::~Task()
{
	for (auto node : m_taskList)
	{
		SAFE_DELETE(node);
	}
}

void Task::Add(TaskNode* taskNode)
{
	m_taskList.push_back(taskNode);
}

void Task::Remove(TaskNode* taskNode)
{
	m_taskList.remove(taskNode);
}

void Task::Process()
{
	while (!_shutdown)
	{
		std::vector<TaskNode*> deleteNodes;
		for (auto taskNode : m_taskList)
		{
			if (taskNode->Expired())
			{
				deleteNodes.push_back(taskNode);
				continue;
			}
			taskNode->Tick();
		}

		for (auto node : deleteNodes)
		{
			this->Remove(node);
		}
		CONTEXT_SWITCH;
	}
}

void Task::Run()
{
	m_thread = MAKE_THREAD(Task, Process);
}

TaskManager::TaskManager()
{
	xml_t config;
	if (!loadConfig(&config))
	{
		return;
	}
	this->Initialize(&config);
}

void TaskManager::Initialize(xml_t* config)
{
	xmlNode_t* root = config->FirstChildElement("App")->FirstChildElement("Task");
	if (!root)
	{
		SLOG(L"!! TaskManager : not exist task setting");
		return;
	}

	xmlNode_t* element = root->FirstChildElement("ThreadCount");
	sscanf_s(element->GetText(), "%d", &m_threadCount);

	for (int i = 0; i < m_threadCount; ++i)
	{
		Task* task = new Task(i);
		m_taskPool.push_back(task);
		task->Run();
	}

	SLOG(L"TaskManager : task thread, [%d] maked", m_threadCount);
}

TaskManager::~TaskManager()
{
	for (auto task : m_taskPool)
	{
		SAFE_DELETE(task);
	}
}

void TaskManager::Add(Work* workObject, int freqSec, int durationSec)
{
	static int nodeCount = 0;

	TaskNode* node = new TaskNode(workObject, freqSec, durationSec);
	int index = nodeCount % m_threadCount;
	Task* task = m_taskPool[index];
	task->Add(node);

	++nodeCount;
}