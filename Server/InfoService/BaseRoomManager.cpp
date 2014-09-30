#include "InfoUserManager.h"
#include "BaseRoomManager.h"
#include "GateTask.h"
#include "QueueRoom.h"
#include "FreeRoom.h"
#include "TwoRoom.h"
#include "OnlyRoom.h"

stBaseRoomManager * stBaseRoomManager::m_pInstance = NULL;


bool stBaseRoomManager::AddRoom(stBaseRoom * p_room)
{
	bool ret_code = false;
	if(p_room)
	{
		std::pair<defBaseRoomMapIter, bool> insert_code;
		insert_code = m_stRoomMap.insert(defBaseRoomMapValue(p_room->GetRoomId(), p_room));
		
		if(insert_code.second)
		{
			Global::logger->info("[info_room_add] add room to manager list successful <tempid=%llu>",
								 p_room->GetRoomId());
		}
		else
		{
			Global::logger->fatal("[info_room_add] add room to manager list failed <tempid=%llu>",
								 p_room->GetRoomId());
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


void stBaseRoomManager::RemoveRoom(const QWORD temp_id)
{
	if(m_stRoomMap.size() == 0)
	{
		return;
	}
	
	defBaseRoomMapIter iter = m_stRoomMap.find(temp_id);
	if(iter != m_stRoomMap.end())
	{
		Global::logger->debug("[info_room_remove] remove a room <tempid=%llu>", temp_id);
		delete iter->second;
		iter->second = NULL;
		m_stRoomMap.erase(iter);
	}
}



void stBaseRoomManager::RemoveTransService(const WORD task_id)
{
	if(m_stRoomMap.size() == 0)
	{
		return;
	}

	defBaseRoomMapIter iter = m_stRoomMap.begin();
	for(; iter != m_stRoomMap.end(); iter++)
	{
		stBaseRoom * p_room = iter->second;
		if(p_room && p_room->GetTaskId() == task_id)
		{
			Global::logger->debug("[info_room_remove] remove a room trans service crash <taskid=%llu, roomid=%llu>",task_id,
								  p_room->GetRoomId());
			p_room->Terminate();
		}
	}
}



stBaseRoom * stBaseRoomManager::GetRoom(const QWORD temp_id)
{
	stBaseRoom * p_room = NULL;
	
	if(m_stRoomMap.size() == 0)
	{
		return p_room;
	}
	
	
	defBaseRoomMapIter iter = m_stRoomMap.find(temp_id);
	if(iter != m_stRoomMap.end())
	{
		p_room = iter->second;
	}
	
	return p_room;
}



stBaseRoom * stBaseRoomManager::CreateRoom(WORD task_id,
								   QWORD room_id, Cmd::enRoomType room_type, const char * r_ip , const unsigned int r_port)
{
	stBaseRoom * p_room = NULL;
	if(room_type == Cmd::ROOM_TYPE_QUEUE)
	{
		p_room = new stQueueRoom(task_id, room_id, room_type, r_ip, r_port);
	}
	else if(room_type == Cmd::ROOM_TYPE_FREE)
	{
		p_room = new stFreeRoom(task_id, room_id, room_type, r_ip, r_port);
	}
	else if(room_type == Cmd::ROOM_TYPE_TWO)
	{
		p_room = new stTwoRoom(task_id, room_id, room_type, r_ip, r_port);
	}
	else if(room_type == Cmd::ROOM_TYPE_ONLY)
	{
		p_room = new stOnlyRoom(task_id, room_id, room_type, r_ip, r_port);
	}	

	
	return p_room;
}



const DWORD stBaseRoomManager::Size() 
{
	DWORD num = m_stRoomMap.size();
	return num;
}


bool stBaseRoomManager::GetRoomInfos(Cmd::cmdNotifyRoomList & send_cmd)
{
	if(m_stRoomMap.size() == 0)
	{
		return false;
	}
	
	defBaseRoomMapIter iter, next;
	next = m_stRoomMap.begin();
	for(iter=next; iter != m_stRoomMap.end(); iter=next)
	{
		if(next != m_stRoomMap.end())
		{
			next++;
		}
			
		stBaseRoom * p_room = iter->second;
		if(p_room)
		{
			Cmd::stRoomInfo * p_info = send_cmd.add_info();
			p_info->set_roomid(p_room->GetRoomId());
			p_info->set_roomtype(p_room->GetRoomType());
			p_info->set_num(p_room->Size());
		}
	}
	
	return true;
}


void stBaseRoomManager::Update()
{
	if(m_stRoomMap.size() == 0)
	{
		return;
	}
	
	defBaseRoomMapIter iter, next;
	next = m_stRoomMap.begin();
	for(iter=next; iter != m_stRoomMap.end(); iter=next)
	{
		if(next != m_stRoomMap.end())
		{
			next++;
		}
				
		stBaseRoom * p_room = iter->second;
		if(p_room)
		{
			if(p_room->IsTerminate())
			{
				Global::logger->info("[info_room_del] delete room from manager list <roomid=%llu>", iter->first);
				
				p_room->KickAllUser();
				
				delete (iter->second);
				iter->second = NULL;
				m_stRoomMap.erase(iter);
			}
			else
			{
				p_room->Update();
			}
		}
		else
		{
			stAssert(0);
		}
	}
}
