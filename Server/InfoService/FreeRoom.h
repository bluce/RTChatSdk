#ifndef _ST_FREEROOM_H
#define _ST_FREEROOM_H

#include "BaseRoom.h"

class stFreeRoom: public stBaseRoom
{
 public:
	stFreeRoom(WORD task_id, QWORD room_id, Cmd::enRoomType roomtype, const char * r_ip , const unsigned int r_port);

	~stFreeRoom();

	virtual bool IsFull();
};

#endif
