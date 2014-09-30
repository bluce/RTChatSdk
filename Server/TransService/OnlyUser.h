#ifndef _ST_ONLYUSER_H
#define _ST_ONLYUSER_H

#include "RTCPUser.h"


class stOnlyUser: public stRTCPUser
{
 public:
	stOnlyUser(stBaseRoom * p_room, const char * c_name, defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id);

	~stOnlyUser();

	virtual bool Update(QWORD cur_time);

 private:
	bool m_blIsActive;
};


#endif
