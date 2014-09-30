#include "TwoRoom.h"

stTwoRoom::stTwoRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port):
	stBaseRoom(task_id, room_id, _type, r_ip, r_port)
{

}


stTwoRoom::~stTwoRoom()
{
	
}


bool stTwoRoom::IsFull()
{
	if(m_stUserMap.size() > 1)
	{		
		return true;
	}
	
	return false;
}
