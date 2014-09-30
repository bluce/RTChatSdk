#include "GateTaskManager.h"

stGateTaskManager * stGateTaskManager::m_pInstance = NULL;

void stGateTaskManager::AddTask(stGateTask * p_task)
{
	if(p_task)
	{
		m_stRWLock.wrlock();
		m_stTaskMap.insert(defGateTaskMapValue(p_task->GetTaskId(), p_task));
		m_stRWLock.unlock();
	}
}


void stGateTaskManager::RemoveTask(stGateTask * p_task)
{
	if(p_task)
	{
		m_stRWLock.wrlock();
		defGateTaskMapIter iter = m_stTaskMap.find(p_task->GetTaskId());
		if(iter != m_stTaskMap.end())
		{
			m_stTaskMap.erase(iter);
		}
		m_stRWLock.unlock();
	}
}

stGateTask * stGateTaskManager::GetGateTask(const defTempId & task_id)
{
	m_stRWLock.wrlock();
	defGateTaskMapIter iter = m_stTaskMap.find(task_id);
	if(iter != m_stTaskMap.end())
	{
		m_stRWLock.unlock();
		return iter->second;
	}
	m_stRWLock.unlock();
	return NULL;
}



