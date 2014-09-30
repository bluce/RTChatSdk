#include "AssertEx.h"
#include "RoomManager.h"
#include "AssignPort.h"
#include "game.pb.h"
#include "QueueRoom.h"
#include "FreeRoom.h"
#include "TwoRoom.h"
#include "OnlyRoom.h"

stRoomManager * stRoomManager::m_pInstance = NULL;


stRoomManager::stRoomManager()
{
	
}


stRoomManager::~stRoomManager()
{
	
}



bool stRoomManager::AddRoom(stBaseRoom * p_room)
{
	bool ret_code = false;
	if(p_room)
	{
		std::pair<defRoomMapIter, bool > insert_code;
		m_stRWLock.wrlock();
		insert_code = m_stRoomMap.insert(defRoomMapValue(p_room->GetRoomId(), p_room));
		ret_code = insert_code.second;
		if(ret_code)
		{
			Global::logger->info("[game_lobby_room_add] add a room to room manager list successful <roomid=%llu>",
								 p_room->GetRoomId());
		}
		else
		{
			Global::logger->fatal("[game_lobby_room_add] add a room to room manager list failed <roomid=%llu>",
								 p_room->GetRoomId());
			stAssert(0);
		}
		
		m_stRWLock.unlock();
	}
	
	return ret_code;
}


bool stRoomManager::RemoveRoom(stBaseRoom * p_room)
{
	bool ret_code = false;
	
	if(p_room)
	{
		m_stRWLock.wrlock();
	
		defRoomMapIter iter = m_stRoomMap.find(p_room->GetRoomId());
		if(iter != m_stRoomMap.end())
		{
			m_stRoomMap.erase(iter);
			ret_code = true;
			Global::logger->info("[game_room_remove] remove room from manager list <roomid=%llu>", p_room->GetRoomId());

		}
	
		m_stRWLock.unlock();
	}
	
	return ret_code;
}


stBaseRoom * stRoomManager::GetRoom(const defRoomId & room_id)
{
	stBaseRoom * p_room = NULL;
	
	m_stRWLock.wrlock();
	
	defRoomMapIter iter = m_stRoomMap.find(room_id);
	if(iter != m_stRoomMap.end())
	{
		p_room = iter->second;
	}
	
	m_stRWLock.unlock();
	
	return p_room;
}


QWORD stRoomManager::CreateRoom(WORD room_type)
{
	DWORD port = stAssignPort::GetInstance().GetPort();
	if(port == 0)
	{
		stAssert(0);
		return 0;
	}
	
	Cmd::stTaskId temp_id = 0;
	temp_id.server_id = GetChannelIDFromArgs();
	temp_id.order_id = port;

	stBaseRoom * p_room = NULL;
	
	if(room_type == Cmd::ROOM_TYPE_QUEUE)
	{
		p_room = new stQueueRoom();
	}
	else if(room_type == Cmd::ROOM_TYPE_FREE)
	{
		p_room = new stFreeRoom();
	}
	else if(room_type == Cmd::ROOM_TYPE_TWO)
	{
		p_room = new stTwoRoom();
	}
	else if(room_type == Cmd::ROOM_TYPE_ONLY)
	{
		p_room = new stOnlyRoom();
	}

	if(p_room->Init(temp_id))
	{
		if(AddRoom(p_room))
		{
			Global::logger->info("[create_room] create a room successful <roomid=%llu,ip=%s,port=%llu>",(QWORD)temp_id,
						 Global::g_strServiceIP.c_str(), port);
			return temp_id;
		}
	}

	delete p_room;
	p_room = NULL;
	stAssert(0);
	return 0;
}
