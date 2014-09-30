#include "GateTaskManager.h"

stGateTaskManager * stGateTaskManager::m_pInstance = NULL;

void stGateTaskManager::AddTask(stGateTask * p_task)
{
	if(p_task)
	{
		m_stRWLock.wrlock();
		m_stTaskMap.insert(defGateTaskMapValue(p_task->GetTaskId(), p_task));
		Global::logger->debug("[gate_task] a gate server start  <serverid=%u>", p_task->GetTaskId());
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
			Global::logger->debug("[gate_task] remove a gate task <id=%u>", p_task->GetTaskId());
			m_stTaskMap.erase(iter);
		}
		m_stRWLock.unlock();
	}
}

stGateTask * stGateTaskManager::GetGateTask(const WORD & zone_id)
{
	m_stRWLock.wrlock();
	defGateTaskMapIter iter = m_stTaskMap.find(zone_id);
	if(iter != m_stTaskMap.end())
	{
		m_stRWLock.unlock();
		return iter->second;
	}
	m_stRWLock.unlock();
	return NULL;
}


void stGateTaskManager::Broadcast(const char * p_send, const unsigned int send_size)
{
	if(m_stTaskMap.size() == 0)
	{
		return;
	}
	
	defGateTaskMapIter iter = m_stTaskMap.begin();
	for(; iter != m_stTaskMap.end(); iter++)
	{
		iter->second->SendCmd(p_send, send_size);
	}
}


const DWORD stGateTaskManager::Size() 
{
	m_stRWLock.rdlock();
	DWORD num = m_stTaskMap.size();
	m_stRWLock.unlock();
	return num;
}

void stGateTaskManager::DealQueueCmd()
{
	if(m_stTaskMap.size() == 0)
		return;
	defGateTaskMapIter iter = m_stTaskMap.begin();
	for(; iter != m_stTaskMap.end(); iter++)
	{
		iter->second->Get(1000);
	}
}
