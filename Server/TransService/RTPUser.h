#ifndef _ST_RTPUSER_H
#define _ST_RTPUSER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "Global.h"

class stBaseRoom;


class stRTPUser: public stDisableCopy
{
 public:
	stRTPUser(stBaseRoom * p_room, defSSRCId ssrc_id, struct sockaddr_in & _addr);
	
	virtual ~stRTPUser();

	defSSRCId GetSSRCId() const;

	bool IsInit();

	void Terminate();

	bool IsTerminate();

	void Leave();

	bool IsLeave();

	void SetAddr(const sockaddr_in & _addr);

	void SendData(const char * msg, const unsigned int msg_len);

	void SetRTCPName(const char * c_name);
	
	const char * GetRTCPName();
	
 protected:
	
	defSSRCId ssrcid;	

	struct sockaddr_in addr;
	
	stBaseRoom * m_pRoom;

	bool m_blInit;

	bool m_blTerminate;

	bool m_blIsLeave;

	std::string m_strRTCPName;
};


#endif
