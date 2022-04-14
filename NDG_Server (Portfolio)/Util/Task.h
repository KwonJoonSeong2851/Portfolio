#pragma once
#include "../stdafx.h"
#include <list>


//-------------------------------------------------//
/*

        작업 관리

*/
//-------------------------------------------------//


#define TICK_INFINITY   0
class Work;
class Thread;
class TaskNode
{
    Work*    m_workObject;
    int      m_freqSec;
    int      m_durationSec;

    tick_t   m_nextTick;

public:
    TaskNode(Work* workObject, int freqSec, int durationSec);
    ~TaskNode();

    void NextTick();
    bool Expired();

    void Tick();
};



//-------------------------------------------------//

class Task
{
    std::list<TaskNode*>    m_taskList;
    Thread*                m_thread;
    int                    m_id;


public:
    Task(int id);
    ~Task();

    void Add(TaskNode* taskNode);
    void Remove(TaskNode* taskNode);

    void Process();
    void Run();
};

//-------------------------------------------------//

class TaskManager : public Singleton<TaskManager>
{
    int                    m_threadCount;
    std::vector<Task*>     m_taskPool;

public:
    TaskManager();
    virtual ~TaskManager();
    void Initialize(xml_t* config);
    void Add(Work* workObject, int freqSec, int durationSec);
};