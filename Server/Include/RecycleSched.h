#ifndef _ST_RECYCLESCHED_H
#define _ST_RECYCLESHCED_H

#include "PollThread.h"
#include "ThreadGroup.h"

class stServiceTaskSched;


class stRecycleThread: public stPollThread
{
 public:

	stRecycleThread(stServiceTaskSched * sched, const char * name):
	stPollThread(name), m_pSched(sched){}

	bool Init();

	void Final();

	void Run();

   	void RemoveTask(stServiceTask * p_task);
	
 protected:
	
	void AddTaskToList(stServiceTask * p_task);

 private:

	stServiceTaskSched * m_pSched;
};


class stRecycleSched
{
 public:
	stRecycleSched(stServiceTaskSched * sched, const WORD thread_num):
	m_pSched(sched), m_stThreadGroup(thread_num){}
	
	bool Init();

	void Final();

	int Size();

  	bool  AddTask(stServiceTask * p_task);
	
 private:
	stServiceTaskSched * m_pSched;

	stThreadGroup m_stThreadGroup;
	
};

#endif
