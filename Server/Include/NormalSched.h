#ifndef _ST_NORMALSCHED_H
#define _ST_NORMALSCHED_H

#include "ThreadGroup.h"
#include "PollThread.h"

class stServiceTaskSched;


class stNormalThread: public stPollThread
{
 public:
	stNormalThread(stServiceTaskSched * sched, const char * name);

	bool Init();

	void Final();

	void Run();

   	void RemoveTask(stServiceTask * p_task);
	
 protected:
	
	void AddTaskToList(stServiceTask * p_task);
	
 private:

	stServiceTaskSched * m_pSched;
};



class stNormalSched
{
 public:

	stNormalSched(stServiceTaskSched * sched, const WORD thread_num);

	bool AddTask(stServiceTask * p_task);

	bool Init();

	void Final();
	
	int Size();
	
 private:

	stServiceTaskSched * m_pSched;

	stThreadGroup m_stThreadGroup;
};

#endif
