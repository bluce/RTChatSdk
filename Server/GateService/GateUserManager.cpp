#include "GateTask.h"
#include "InfoClient.h"
#include "GateUserManager.h"

stGateUserManager * stGateUserManager::m_pInstance = NULL;

void stGateUserManager::AddUser(stGateUser * p_user)
{
	if(p_user)
	{
		m_stRWLock.wrlock();
		Global::logger->debug("add a new user <%llu>", p_user->GetTempId());
		m_stUserMap.insert(defGateUserMapValue(p_user->GetTempId(), p_user));
		m_stRWLock.unlock();
	}
}


stGateUser * stGateUserManager::GetUser(const defTempId & user_id)
{
	m_stRWLock.wrlock();
	defGateUserMapIter iter = m_stUserMap.find(user_id);
	if(iter != m_stUserMap.end())
	{
		m_stRWLock.unlock();
		return iter->second;
	}
	m_stRWLock.unlock();
	return NULL;
}



void stGateUserManager::Update(QWORD cur_time)
{
	m_stRWLock.wrlock();
	if(m_stUserMap.size() == 0)
	{
		m_stRWLock.unlock();
		return;
	}

	defGateUserMapIter iter,next;
	next= m_stUserMap.begin();
	for(iter = next; iter != m_stUserMap.end(); iter = next)
	{
		if(next != m_stUserMap.end())
		{
			next++;
		}
		
		stGateUser * p_user = iter->second;
		if(p_user)
		{
			if(p_user->IsTerminate())
			{
				delete p_user;
				p_user = NULL;
				m_stUserMap.erase(iter);
			}
		}
	}

	if(m_stUserMap.size() == 0)
	{
		m_stRWLock.unlock();
		return;
	}

	iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stGateUser * p_user = iter->second;
		if(p_user)
		{
			if(p_user->CheckTerminte(cur_time))
			{
				Cmd::cmdUnRegisterUser info_cmd;
				info_cmd.set_tempid(p_user->GetTempId());
				stInfoClient::GetInstance().SendCmd(p_user->GetTempId(), Cmd::enUnRegisterUser, &info_cmd, info_cmd.ByteSize());
				Global::logger->debug("[remove_gateuser] notify infoservice remove user <taskid=%llu,uid=%s>",
									  p_user->GetTempId(),
									  p_user->GetUniqueId());

				p_user->Terminate();
			}
		}
	}

	
	m_stRWLock.unlock();
	return;
}



