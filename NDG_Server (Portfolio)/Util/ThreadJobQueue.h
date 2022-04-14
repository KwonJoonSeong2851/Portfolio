#pragma once
#include "../stdafx.h"
#include <queue>
#include <stdexcept>

//-------------------------------------------------//
/*

     jobqueue swap class

*/
//-------------------------------------------------//

template <typename T>

class ThreadJobQueue
{
private:
	enum
	{
		WRITE_QUEUE,
		READ_QUEUE,
		MAX_QUEUE,
	};

	std::queue<T>       m_queue[MAX_QUEUE];

	std::queue<T>*      m_writeQueue;
	std::queue<T>*      m_readQueue;

	Lock                m_lock;

public:
	ThreadJobQueue(const WCHAR* name): m_lock(name)
	{
		m_writeQueue = &m_queue[WRITE_QUEUE];
		m_readQueue = &m_queue[READ_QUEUE];
	}

	~ThreadJobQueue()
	{
		m_readQueue->empty();
		m_writeQueue->empty();
	}

	inline void Push(const T& t)
	{
		SAFE_LOCK(m_lock);
		m_writeQueue->push(t);
	}

	inline bool Pop(T& t)
	{
		SAFE_LOCK(m_lock);
		size_t size = this->size();
		if (size == 0)
		{
			return false;
		}
		if (m_readQueue->empty())
		{
			this->Swap();
		}

		t = m_readQueue->front();
		m_readQueue->pop();
		return true;
	}

	inline void Swap()
	{
		SAFE_LOCK(m_lock);
		if (m_writeQueue == &m_queue[WRITE_QUEUE])
		{
			m_writeQueue = &m_queue[READ_QUEUE];
			m_readQueue = &m_queue[WRITE_QUEUE];
		}
		else
		{
			m_writeQueue = &m_queue[WRITE_QUEUE];
			m_readQueue = &m_queue[READ_QUEUE];
		}
	}

	inline bool IsEmpty()
	{
		return m_readQueue->empty();
	}

	inline size_t size()
	{
		SAFE_LOCK(m_lock);
		size_t size = (size_t)(m_queue[WRITE_QUEUE].size() + m_queue[READ_QUEUE].size());
		return size;
	}

};