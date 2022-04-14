#include "../stdafx.h"
#include "Lock.h"
#include "Thread.h"

#ifdef _DEBUG
#define STERN_MODE
#endif


const int INVALID_LINE = -1;

Lock::Lock(const WCHAR* name)
{
	m_lockId = LockManager::GetInstance().GeneralId();
	m_name = name;

	m_lockingFile.clear();
	m_lockingLine = INVALID_LINE;
}

Lock::~Lock()
{
	m_name.clear();
}

const WCHAR* Lock::Name()
{
	return m_name.c_str();
}

size_t Lock::LockId()
{
	return m_lockId;
}

lock_t& Lock::Mutex()
{
	return m_mutex;
}

void Lock::lock(LPCWSTR fileName, int lineNo)
{
	m_mutex.lock();
	m_lockingFile = fileName;
	m_lockingLine = lineNo;
}

void Lock::unLock()
{
	m_mutex.unlock();
	
	m_lockingFile.clear();
	m_lockingLine = INVALID_LINE;
}

void Lock::SetThreadId(size_t id)
{
	m_threadId = id;
}

size_t Lock::ThreadId()
{
	return m_threadId;
}



//------------------------------------------------------//
//데드락 감지

LockSafeScope::LockSafeScope(Lock* lock, LPCWSTR fileName, int lineNo)
{
	if (lock == nullptr)
	{
		return;
	}


	if (_shutdown == true)
	{
		return;
	}

	m_lock = lock;
	Lock* deadLock = LockManager::GetInstance().CheckDeadLock(m_lock);

	if (deadLock != nullptr)
	{

		//교착상태에 빠졌을때
#ifdef STERN_MODE
		SERRLOG(L"! [%s]lock and [%s]lock is dead detecting !!!", deadLock->Name(), lock->Name());
#else
		std::lock(m_lock->Mutex(), deadLock->Mutex());
#endif
		return;
	}

	m_lock->lock(fileName, lineNo);
	lock->SetThreadId(GET_CURRENT_THREAD_ID());
}


LockSafeScope::~LockSafeScope()
{
	if (!m_lock)
	{
		return;
	}

	if (_shutdown == true)
	{
		return;
	}

	m_lock->unLock();
	m_lock->SetThreadId(0);
}


//----------------------------------------------------------------//
LockManager::LockManager()
{
	m_idSeed = 0;
}

Lock* LockManager::SearchLockCycle(Lock* newLock)
{
	//list 따라 lock 이름 비교
	Thread* thread = ThreadManager::GetInstance().At(GET_CURRENT_THREAD_ID());
	if (!thread)
	{
		return nullptr;
	}

	std::vector<Lock*> trace;
	trace.push_back(newLock);

	Lock* deadLock = nullptr;
	while (true)
	{
		Lock* threadLock = thread->GetLock();
		if (threadLock == nullptr)
		{
			break;
		}

		if (threadLock->LockId() == trace[0]->LockId())
		{
			deadLock = threadLock;
			break;
		}

		trace.push_back(threadLock);
		thread = ThreadManager::GetInstance().At(threadLock->ThreadId());
		if (!thread)
		{
			break;
		}
	}

	trace.empty();
	return deadLock;
}

Lock* LockManager::CheckDeadLock(Lock* newLock)
{
	Lock* deadLock = this->SearchLockCycle(newLock);
	if (deadLock)
	{
		return deadLock;
	}

	return nullptr;
}

size_t LockManager::GeneralId()
{
	size_t id = m_idSeed++;
	return id;
}





