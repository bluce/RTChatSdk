#include "OnlyUser.h"
#include "BaseRoom.h"

stOnlyUser::stOnlyUser(stBaseRoom * p_room, const char * c_name,
		   defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id):
	stRTCPUser(p_room, c_name, ssrc_id, _addr, temp_id, RTCPUSER_TYPE_ONLY)
{
	m_blIsActive = false;
}


stOnlyUser::~stOnlyUser()
{
	
}


bool stOnlyUser::Update(QWORD cur_time)
{
	bool ret_code = stRTCPUser::Update(cur_time);
	if(GetRTPUserId() != 0 && ! m_blIsActive)
	{	
		StopSendActive();
		m_blIsActive = true;
	}

	return ret_code;
}
