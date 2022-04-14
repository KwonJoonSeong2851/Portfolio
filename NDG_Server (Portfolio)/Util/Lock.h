#pragma once
#include "../stdafx.h"

//-------------------------------------------------//
/*
Lock Class

LockSafeScope
lock ��û�� ���� Lock�� �ɶ� �������¿� ���� �� �ִ��� �˻�

LockManager
Lock�� �Ҵ�� ������ Lock�� ID �߱�, �����ϱ� �����ϱ� ����
Lock�� ����� Thread���� ���� ���鼭 Lock ����Ŭ�� ������� �˻��ϰ� �ɸ��� �������� ����
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