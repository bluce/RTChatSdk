#ifndef _ST_TASKQUEUE_H
#define _ST_TASKQUEUE_H

#include <queue>
#include "Global.h"
#include "ServiceTask.h"

typedef std::pair<volatile bool, stServiceTask *  > defTaskQueue;

const DWORD g_dwTaskSize = 6000;

class stTaskQueue
{
 public:
	stTaskQueue();

	~stTaskQueue();
	
	bool Put(stServiceTask * p_task);

	stServiceTask * Get();

 private:

	void Final();

	defTaskQueue TaskQueue[g_dwTaskSize];

	unsigned int m_dwReadAddr;
	
	unsigned int m_dwWriteAddr;
};

#endif
