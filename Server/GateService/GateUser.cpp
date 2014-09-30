#include "GateTask.h"
#include "GateTaskManager.h"
#include "GateUser.h"


stGateUser::stGateUser(const defTempId temp_id, const char * uid)
{
	m_blIsTerminate = false;
	m_blIsConnect = true;
	tempid = temp_id;
	m_qdDisTime = 0;
	memset(uniqueid, 0, sizeof(uniqueid));
	strncpy(uniqueid, uid, sizeof(uniqueid) - 1 );
}


defTempId stGateUser::GetTempId() const
{
	return tempid;
}


stGateTask * stGateUser::GetTask()
{
	stGateTask * p_task = NULL;
	
	m_TaskLock.wrlock();
	p_task = stGateTaskManager::GetInstance().GetGateTask(taskid);
	m_TaskLock.unlock();
	
	return p_task;
}


void stGateUser::SetTask(stGateTask * p_task)
{
	m_TaskLock.wrlock();
	stGateTask * p_old_task = stGateTaskManager::GetInstance().GetGateTask(taskid);
	if(p_old_task)
	{
		p_old_task->Terminate();
	}

	if(p_task)
	{
		taskid = p_task->GetTaskId();
		Global::logger->debug("->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>set taskid=%llu", taskid);
	}
	else
	{
		taskid = 0;
	}
	
	m_TaskLock.unlock();
}



bool stGateUser::IsConnect()
{
	bool ret_code = false;
	m_stLock.wrlock();
	ret_code = m_blIsConnect;
	m_stLock.unlock();
	return ret_code;
}


void stGateUser::Connect()
{
	m_stLock.wrlock();
	m_blIsConnect = true;
	m_stLock.unlock();
}


void stGateUser::DisConnect(defTempId _id, QWORD _time)
{
	stGateTask * p_task = GetTask();
	if(!p_task)
	{
		Global::logger->debug("why not find this task <%llu>", taskid);
		return;
	}
	
	m_stLock.wrlock();
	Global::logger->debug("->>>>>>>>>>>>>>>>>>disconnect %llu,%llu", _id, p_task->GetTaskId());
	if(_id == p_task->GetTaskId())
	{
		m_qdDisTime = _time + 60 * 1000;
		SetTask(NULL);
		m_blIsConnect = false;
	}
	
	m_stLock.unlock();
}


void stGateUser::Terminate()
{
	m_blIsTerminate = true;
}


bool stGateUser::IsTerminate()
{
	return m_blIsTerminate;
}


bool stGateUser::CheckTerminte(QWORD _cur)
{
	m_stLock.wrlock();
	if(m_blIsConnect == false)
	{
		if(_cur > m_qdDisTime)
		{
			m_stLock.unlock();
			return true;
		}
	}
	
	m_stLock.unlock();
	return false;
}



const char * stGateUser::GetUniqueId()
{
	return uniqueid;
}
