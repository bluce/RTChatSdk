#include "FreeRoom.h"

stFreeRoom::stFreeRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port):
	stBaseRoom(task_id, room_id, _type, r_ip, r_port)
{

}


stFreeRoom::~stFreeRoom()
{
	
}


bool stFreeRoom::IsFull()
{
	if(m_stUserMap.size() > 3)
	{		
		return true;
	}
	
	return false;
}
