#ifndef _STGATETASKMANAGER_H
#define _STGATETASKMANAGER_H

#include "Global.h"
#include "RWLock.h"
#include "GateTask.h"

class stGateTaskManager
{
 public:
	typedef std::map<defTempId, stGateTask * > defGateTaskMap;
	typedef std::map<defTempId, stGateTask * >::iterator defGateTaskMapIter;
	typedef std::map<defTempId, stGateTask * >::value_type defGateTaskMapValue;
	
	static stGateTaskManager & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stGateTaskManager();
		}
		return * m_pInstance;
	}

	static void DelInstance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
		}
		m_pInstance = NULL;
	}

	void AddTask(stGateTask * p_task);

	void RemoveTask(stGateTask * p_task);

	stGateTask * GetGateTask(const defTempId & task_id);
	
 private:
	
	static stGateTaskManager * m_pInstance;

	defGateTaskMap  m_stTaskMap;

	stRWLock m_stRWLock;
};

#endif
