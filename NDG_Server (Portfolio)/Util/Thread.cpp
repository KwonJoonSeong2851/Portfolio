#include "Thread.h"
#include "Lock.h"
#include "../stdafx.h"

Thread::Thread(thread_t* thread, wstr_t name)
{
	m_name = name;
	m_thread = thread;
	m_id = std::hash<std::thread::id>()(m_thread->get_id());

	ThreadManager::GetInstance().Put(this);
}


Thread::~Thread()
{
	m_thread->join();
	SAFE_DELETE(m_thread);
	SAFE_DELETE(m_lock);
}

size_t Thread::GetId()
{
	return m_id;
}

wstr_t& Thread::GetName()
{
	return m_name;
}

void Thread::SetLock(Lock* lock)
{
	m_lock = lock;
}

Lock* Thread::GetLock()
{
	return m_lock;
}

//-------------------------------------------------//

ThreadManager::~ThreadManager()
{
	for (auto thread : m_threadPool)
	{
		SAFE_DELETE(thread.second);
	}
}

void ThreadManager::Put(Thread* thread)
{
	std::pair<size_t, Thread*> node(thread->GetId(), thread);
	m_threadPool.insert(node);
	SLOG(L"Create Thread : Id[0x%X] Name[%s], Pool Size[%d]", thread->GetId(), thread->GetName().c_str(), m_threadPool.size());
}


void ThreadManager::Remove(size_t id)
{
	auto iter = m_threadPool.find(id);
	if (iter == m_threadPool.end())
	{
		return;
	}
	auto thread = iter->second;
	m_threadPool.erase(iter);
}

Thread* ThreadManager::At(size_t id)
{
	if (m_threadPool.empty())
	{
		return nullptr;
	}
	auto iter = m_threadPool.find(id);
	if (iter == m_threadPool.end())
	{
		return nullptr;
	}
	auto thread = iter->second;
	return thread;
}