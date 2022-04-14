#pragma once
#include "../stdafx.h"


//-------------------------------------------------//
/*
     Thread 클래스

	 ThreadManager Thread관리 클래스
	 Thread 생성,검색 및 ThreadPool 관리
*/
//-------------------------------------------------//


#define MAKE_THREAD(className, process) (new Thread(new thread_t(&className##::##process, this), L#className))
#define GET_CURRENT_THREAD_ID() std::hash<std::thread::id>()(std::this_thread::get_id())

class Lock;
typedef std::function<void(void*)> ThreadFunction;


class Thread
{
	size_t      m_id;
	wstr_t      m_name;
	thread_t*   m_thread;
	Lock*       m_lock;

public:
	Thread(thread_t* thread, wstr_t name);
	~Thread();

	size_t GetId();
	wstr_t& GetName();

	void SetLock(Lock* lock);
	Lock* GetLock();
};



//#define THREAD_POOL_HASHMAP

class ThreadManager : public Singleton<ThreadManager>
{
private:
#ifdef THREAD_POOL_HASHMAP
	std::hash_map<size_t, Thread*> m_threadPool;
#else 
	std::map <size_t, Thread*> m_threadPool;
#endif


public:
	~ThreadManager();

	void Put(Thread* thread);
	void Remove(size_t id);
	Thread* At(size_t id);

};

