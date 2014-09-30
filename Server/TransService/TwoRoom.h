#ifndef _ST_TWO_ROOM_H
#define _ST_TWO_ROOM_H

#include "BaseRoom.h"

class stTwoRoom: public stBaseRoom
{
 public:
	stTwoRoom();
	
	~stTwoRoom();

	virtual void SendRTPData(QWORD task_id, const char * recv_buf, const int recv_len);
};


#endif
