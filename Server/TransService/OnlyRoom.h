#ifndef _ST_ONLY_ROOM_H
#define _ST_ONLY_ROOM_H

#include "BaseRoom.h"

class stOnlyRoom: public stBaseRoom
{
 public:
	stOnlyRoom();
	
	~stOnlyRoom();

	virtual void SendRTPData(QWORD task_id, const char * recv_buf, const int recv_len);
};


#endif
