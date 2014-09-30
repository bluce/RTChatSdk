#ifndef _ST_FREEUSER_H
#define _ST_FREEUSER_H

#include "RTCPUser.h"


const BYTE MAX_LISTEN_NUM = 3;

enum enChannelType
{
	CHANNEL_TYPE_MIN = 0,	
	CHANNEL_TYPE_SEND = 1,	
	CHANNEL_TYPE_RECV = 2,
	CHANNEL_TYPE_MAX = 3,
};


class stFreeUser: public stRTCPUser
{
 public:
	stFreeUser(stBaseRoom * p_room, const char * c_name,
			   defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id);

	~stFreeUser();

	enChannelType GetChannelType() const;

	const char * GetDestName();

	void SetDestName(const char * _name);

	virtual bool Init();
	
	void AddListenChannel(const char * _channel);

	void AddBeListenChannel(const char * _channel);

	void DelListenChannel(const char * _channel);

	void DelBeListenChannel(const char * _channel);

	void GetFreeRecvChannel(stFreeUser * p_dest_user);

	void RemoveMe();

	virtual void Terminate();

	virtual void Leave();

	void BroadCast(const char * send_buf, const unsigned int buf_len);


 private:
	
	enChannelType m_enChannelType;

	std::set<std::string > m_stListeners;

	std::set<std::string > m_stBeListen;
		
	std::string m_strDestName;
};


#endif
