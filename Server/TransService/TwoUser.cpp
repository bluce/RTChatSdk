#include "TwoUser.h"
#include "BaseRoom.h"

stTwoUser::stTwoUser(stBaseRoom * p_room, const char * c_name,
					 defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id):
	stRTCPUser(p_room, c_name, ssrc_id, _addr, temp_id, RTCPUSER_TYPE_TWO)
{
	
}


stTwoUser::~stTwoUser()
{
	
}

