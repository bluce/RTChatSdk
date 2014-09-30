#ifndef _ST_RTCPUSER_H
#define _ST_RTCPUSER_H


#include <sys/socket.h>
#include <arpa/inet.h>

#include "Global.h"


enum enRTCPUserType
{
	RTCPUSER_TYPE_NONE = 0,
	RTCPUSER_TYPE_QUEUE = 1,
	RTCPUSER_TYPE_FREE = 2,
	RTCPUSER_TYPE_TWO = 3,
	RTCPUSER_TYPE_ONLY = 4,
};


class stBaseRoom;

class stRTCPUser
{
 public:
	stRTCPUser(stBaseRoom * p_room, const char * c_name,
			   defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id, enRTCPUserType _type);

	~stRTCPUser();

	enRTCPUserType GetUserType() const;

	const char * GetCName();
	
	void SetSSRCId(const defSSRCId ssrc_id);

	void SetAddr(const sockaddr_in & _addr);

	defSSRCId GetSSRCId() const;

	void SetTempId(const defTempId temp_id);

	defTempId GetTempId() const;

	bool IsTerminate();

	virtual void Terminate();

	virtual void Leave();

	bool IsLeave();

	void SetRTPUserId(defSSRCId _id);

	defSSRCId GetRTPUserId() const;

	virtual bool Init();

	bool IsInit();

	void ClearPing();

	virtual bool Update(QWORD cur_time);
	
	void SendData(const char * msg, const unsigned int msg_len);

	void SendPingCmd();

	void StopSendActive();

 protected:
	stBaseRoom * m_pRoom;

 private:

	defSSRCId ssrcid;

	defTempId tempid;

	char ip[16];
	
	WORD port;

	std::string m_strCName;

	struct sockaddr_in addr;

	bool m_blTerminate;

	bool m_blIsLeave;

	defSSRCId rtpuserid;

	bool m_blPing;

	QWORD ping_time;
	
	enRTCPUserType m_enUserType;
};


#endif
