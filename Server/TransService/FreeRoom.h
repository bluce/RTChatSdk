#ifndef _ST_FREEROOM_H
#define _ST_FREEROOM_H

#include "BaseRoom.h"

class stFreeRoom: public stBaseRoom
{
 public:
	stFreeRoom();
	
	~stFreeRoom();

	void AddSendChannel(stFreeUser * p_free_user);

	void AddRecvChannel(stFreeUser * p_src_user, const char * cname, const char * dest_name);

	void RemoveRecvChannel(stFreeUser * p_free_user);

	void SendRTPData(QWORD task_id, const char * recv_buf, const int recv_len);

 private:
	void BroadCastAddListen(stFreeUser * p_dest_user);

	void NotifyAllSendToMe(stFreeUser * p_src_user);
};

#endif
