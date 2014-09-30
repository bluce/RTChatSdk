#include "QueueManager.h"
#include "BaseRoomManager.h"
#include "InfoUserManager.h"
#include "QueueRoom.h"

stQueueManager::~stQueueManager()
{
	
}


stQueueManager::stQueueManager()
{
	micorder = 1;
}


void stQueueManager::Init(stQueueRoom * p_room)
{
	m_pRoom = p_room;
}


bool stQueueManager::IsHaveMic(stInfoUser * p_user)
{
	if(!p_user)
	{
		stAssert(0);
		return true;
	}

	if(m_stMicQueue.size() == 0)
	{
		return false;
	}

	defMicQueueMapIter iter = m_stMicQueue.begin();
	for(; iter != m_stMicQueue.end(); iter++)
	{
		if(iter->second->m_stTempId == p_user->GetTempId())
		{
			return true;
		}
	}
	
	return false;
}


bool stQueueManager::AddQueueUser(stInfoUser * p_user)
{
	if(IsHaveMic(p_user))
	{
		return false;
	}

	stQueueInfo * p_info = new stQueueInfo(p_user->GetTempId(), p_user->GetUniqueId());
	m_stMicQueue.insert(defMicQueueMapValue(micorder++, p_info));
	
	return true;
}


bool stQueueManager::DelQueueUser(stInfoUser * p_user)
{
	if(!p_user)
	{
		return false;
	}

	if(m_stMicQueue.empty())
	{
		return false;
	}

	defMicQueueMapIter iter = m_stMicQueue.begin();
	for(; iter != m_stMicQueue.end(); iter++)
	{
		if(iter->second->m_stTempId == p_user->GetTempId())
		{
			delete iter->second;
			m_stMicQueue.erase(iter);
			return true;
		}
	}
	
	return true;
}


stQueueInfo * stQueueManager::GetFirstMic()
{
	stQueueInfo * p_info = NULL;
	if(m_stMicQueue.size() == 0)
	{
		return p_info;
	}

	defMicQueueMapIter iter = m_stMicQueue.begin();
	p_info = iter->second;
	m_stMicQueue.erase(iter);
	return p_info;
}


bool stQueueManager::IsEmpty()
{
	return m_stMicQueue.empty();
}



void stQueueManager::NotifyMicQueueToUser(stInfoUser * p_user)
{
	if(m_stMicQueue.size() == 0 || !p_user)
	{
		return;
	}
	
	Cmd::cmdNotifyMicQueue send_cmd;

	defMicQueueMapIter iter = m_stMicQueue.begin();
	for(; iter != m_stMicQueue.end(); iter++)
	{
		stQueueInfo * info = iter->second;
		if(info)
		{
			Cmd::stMicInfo * p_info = send_cmd.add_info();
			p_info->set_tempid(info->m_stTempId);
			p_info->set_uniqueid(info->uniqueid);
		}
		else
		{
			stAssert(0);
		}
	}
	
	p_user->SendCmd(Cmd::enNotifyMicQueue, &send_cmd, send_cmd.ByteSize());

}


void stQueueManager::BroadCastMicQueue()
{
	if(m_stMicQueue.size() == 0)
	{
		return;
	}
	
	Cmd::cmdNotifyMicQueue send_cmd;

	defMicQueueMapIter iter = m_stMicQueue.begin();
	for(; iter != m_stMicQueue.end(); iter++)
	{
		stQueueInfo * info = iter->second;
		if(info)
		{
			Cmd::stMicInfo * p_info = send_cmd.add_info();
			p_info->set_tempid(info->m_stTempId);
			p_info->set_uniqueid(info->uniqueid);
		}
		else
		{
			stAssert(0);
		}
	}
	

	if(m_pRoom)
	{
		m_pRoom->BroadCast(Cmd::enNotifyMicQueue, &send_cmd, send_cmd.ByteSize());
	}
	else
	{
		stAssert(0);
	}
}
