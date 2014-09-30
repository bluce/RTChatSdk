
#include "TransTaskManager.h"

stTransTaskManager * stTransTaskManager::m_pInstance = NULL;

void stTransTaskManager::AddTask(stTransTask * p_task)
{
	if(p_task)
	{
		m_stRWLock.wrlock();
		m_stTaskMap.insert(defTransTaskMapValue(p_task->GetTransId(), p_task));
		Global::logger->debug("[trans_task] a trans server start <transid=%u>", p_task->GetTransId());
		m_stRWLock.unlock();
	}
}


void stTransTaskManager::RemoveTask(stTransTask * p_task)
{
	if(p_task)
	{
		m_stRWLock.wrlock();
		defTransTaskMapIter iter = m_stTaskMap.find(p_task->GetTransId());
		if(iter != m_stTaskMap.end())
		{
			m_stTaskMap.erase(iter);
		}
		m_stRWLock.unlock();
	}
}

stTransTask * stTransTaskManager::GetTransTask(const WORD & zone_id)
{
	m_stRWLock.wrlock();
	defTransTaskMapIter iter = m_stTaskMap.find(zone_id);
	if(iter != m_stTaskMap.end())
	{
		m_stRWLock.unlock();
		return iter->second;
	}
	m_stRWLock.unlock();
	return NULL;
}


void stTransTaskManager::Broadcast(const char * p_send, const unsigned int send_size)
{
	if(m_stTaskMap.size() == 0)
	{
		return;
	}
	
	defTransTaskMapIter iter = m_stTaskMap.begin();
	for(; iter != m_stTaskMap.end(); iter++)
	{
		iter->second->SendCmd(p_send, send_size);
	}
}

const DWORD stTransTaskManager::Size() 
{
	m_stRWLock.rdlock();
	DWORD num = m_stTaskMap.size();
	m_stRWLock.unlock();
	return num;
}

void stTransTaskManager::DealQueueCmd()
{
	if(m_stTaskMap.size() == 0)
		return;
	defTransTaskMapIter iter = m_stTaskMap.begin();
	for(; iter != m_stTaskMap.end(); iter++)
	{
		iter->second->Get(1000);
	}
}


stTransTask *  stTransTaskManager::GetMinTask()
{
	if(m_stTaskMap.size() == 0)
	{
		return NULL;
	}

	defTransTaskMapIter iter = m_stTaskMap.begin();
	stTransTask * p_min_task = iter->second;
	stTransTask * p_task  = NULL;
	for(; iter != m_stTaskMap.end(); iter++)
	{
		p_task  = iter->second;
		if(! p_task)
		{
			stAssert(0);
			continue;
		}
		if(p_min_task->GetRoomCount() > p_task->GetRoomCount())
		{
			p_min_task = p_task;
			p_task = NULL;
		}
	}

	return p_min_task;
}
