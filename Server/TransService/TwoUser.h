#ifndef _ST_TWOUSER_H
#define _ST_TWOUSER_H

#include "RTCPUser.h"


class stTwoUser: public stRTCPUser
{
 public:
	stTwoUser(stBaseRoom * p_room, const char * c_name,
			   defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id);

	~stTwoUser();
};


#endif
