#include "InfoUserManager.h"
#include "GateTask.h"


stInfoUserManager * stInfoUserManager::m_pInstance = NULL;


bool stInfoUserManager::AddUser(stInfoUser * p_user)
{
	bool ret_code = false;
	if(p_user)
	{
		std::pair<defInfoUserMapIter, bool> insert_code;
		insert_code = m_stUserMap.insert(defInfoUserMapValue(p_user->GetTempId(), p_user));
		
		if(insert_code.second)
		{
			Global::logger->info("[info_user_add] add user to manager list successful <tempid=%llu>",
								 p_user->GetTempId());
		}
		else
		{
			Global::logger->fatal("[info_user_add] add user to manager list failed <tempid=%llu>",
								 p_user->GetTempId());
			stAssert(0);
		}
		
		ret_code = insert_code.second;
	}
	else
	{
		stAssert(0);
	}
	
	return ret_code;
	
}


void stInfoUserManager::RemoveUser(const defTempId temp_id)
{
	if(m_stUserMap.size() == 0)
	{
		return;
	}
	
	defInfoUserMapIter iter = m_stUserMap.find(temp_id);
	if(iter != m_stUserMap.end())
	{
		Global::logger->debug("[info_user_remove] remove a user <tempid=%llu>", temp_id);
		delete iter->second;
		iter->second = NULL;
		m_stUserMap.erase(iter);
	}
}



void stInfoUserManager::RemoveGateService(WORD task_id)
{
	if(m_stUserMap.size() == 0)
	{
		return;
	}
	
	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_usre = iter->second;
		if(p_usre && p_usre->GetTaskId() == task_id)
		{
			Global::logger->debug("[info_user_remove] remove a user gateservice crash <taskid=%u,tempid=%llu>", task_id,
								  p_usre->GetTempId());
			p_usre->Terminate();
		}
	}
}


stInfoUser * stInfoUserManager::GetUser(const defTempId temp_id)
{
	stInfoUser * p_user = NULL;
	
	if(m_stUserMap.size() == 0)
	{
		return p_user;
	}
	
	
	defInfoUserMapIter iter = m_stUserMap.find(temp_id);
	if(iter != m_stUserMap.end())
	{
		p_user = iter->second;
	}
	
	return p_user;
}



const DWORD stInfoUserManager::Size() 
{
	DWORD num = m_stUserMap.size();
	return num;
}

stInfoUser * stInfoUserManager::GetRandUser(const defTempId temp_id)
{
	if(m_stUserMap.size() == 0)
	{
		return NULL;
	}
	
	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_usre = iter->second;
		if(p_usre)
		{
			if(p_usre->GetTempId() != temp_id)
			{
				return p_usre;
			}
		}
	}

	return NULL;
}


stInfoUser * stInfoUserManager::GetNotInRoomUser(const defTempId temp_id)
{
	if(m_stUserMap.size() == 0)
	{
		return NULL;
	}
	
	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_usre = iter->second;
		if(p_usre && p_usre->GetTempId() != temp_id && !p_usre->IsInRoom())
		{
			return p_usre;
		}
	}

	return NULL;
}

