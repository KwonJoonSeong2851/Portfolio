#pragma once
#include "../stdafx.h"

//-------------------------------------------------//
/*
Lock Class

LockSafeScope
lock 요청이 오면 Lock을 걸때 교착상태에 빠질 수 있는지 검사

LockManager
Lock이 할당될 때마다 Lock의 ID 발급, 추적하기 쉽게하기 위해
Lock과 연결된 Thread들을 따라 가면서 Lock 사이클을 만드는지 검사하고 걸리면 교착상태 판정
*/
//-------------------------------------------------//

class Lock
{
private:
	lock_t        m_mutex;
	wstr_t        m_name;
	size_t        m_lockId;
	size_t        m_threadId;

	wstr_t        m_lockingFile;
	int           m_lockingLine;


public:
	Lock(const WCHAR* name);
	virtual ~Lock();

	const WCHAR* Name();
	size_t LockId();

	lock_t& Mutex();
	void lock(LPCWSTR fileName, int lineNo);
	void unLock();
	void SetThreadId(size_t id);
	size_t ThreadId();
	
};

class LockSafeScope
{
	Lock* m_lock;

public:
	LockSafeScope(Lock* lock, LPCWSTR fileName, int lineNo);
	~LockSafeScope();
};


#define SAFE_LOCK(lock)   LockSafeScope __lockSafe(&lock, _W(__FILE__), __LINE__);

class LockManager : public Singleton<LockManager>
{
private:
	size_t    m_idSeed;

public:
	LockManager();
	Lock* SearchLockCycle(Lock* neLock);
	Lock* CheckDeadLock(Lock* lock);

	size_t GeneralId();
};