#ifndef _ST_QUEUE_ROOM_H
#define _ST_QUEUE_ROOM_H

#include "BaseRoom.h"

class stQueueRoom: public stBaseRoom
{
 public:
	stQueueRoom();
	
	~stQueueRoom();

	void SendRTPData(defSSRCId ssrc_id, const char * recv_buf, const int recv_len);
};


#endif
