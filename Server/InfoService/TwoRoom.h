#ifndef _ST_TWOROOM_H
#define _ST_TWOROOM_H

#include "BaseRoom.h"

class stTwoRoom: public stBaseRoom
{
 public:
	stTwoRoom(WORD task_id, QWORD room_id, Cmd::enRoomType roomtype, const char * r_ip , const unsigned int r_port);

	~stTwoRoom();

	virtual bool IsFull();
};

#endif
