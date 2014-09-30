#ifndef _ST_POLLTHREAD_H
#define _ST_POLLTHREAD_H

#include <list>
#include <queue>
#include <deque>
#include "Global.h"
#include "Thread.h"
#include "ServiceTask.h"

class stPollThread: public stThread
{
 public:
	virtual ~stPollThread(){}

	stPollThread(const char * name): stThread(name)
	{
		m_dwTaskCount = 0;
	}

	virtual bool Init();

	int Size()
	{
		return m_dwTaskCount;
	}

	virtual void AddTask(stServiceTask * p_task)
	{
		m_stRWLock.wrlock();
		m_TaskQueue.push(p_task);
		m_stRWLock.unlock();
	}

	
	virtual void Final() = 0;

	virtual void RemoveTask(stServiceTask * p_task) = 0;

 protected:

	void GetTaskFromQueue();

	virtual void AddTaskToList(stServiceTask * p_task) = 0;

	int m_iEpollfd;

	std::vector<struct epoll_event > m_EpollEventVec;

	std::list< stServiceTask * > m_TaskList;

	std::queue<stServiceTask *, std::deque<stServiceTask * > > m_TaskQueue;

	DWORD m_dwTaskCount;

	stRWLock m_stRWLock;
	
	stRTime m_stCurrentTime;
};

#endif
