#ifndef _ST_QUEUEUSER_H
#define _ST_QUEUEUSER_H

#include "RTCPUser.h"


class stQueueUser: public stRTCPUser
{
 public:
	stQueueUser(stBaseRoom * p_room, const char * c_name,
			   defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id);

	~stQueueUser();

	virtual bool Update(QWORD cur_time);

 private:
	bool m_blIsActive;
};


#endif
