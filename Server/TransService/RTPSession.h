#ifndef _ST_RTP_SESSION_H
#define _ST_RTP_SESSION_H

#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <zlib.h>

#include <netinet/udp.h>
#include <netinet/tcp.h>

#include "Global.h"
#include "Command.h"

class stBaseRoom;

class stRTPSession: public stDisableCopy
{
 public:
	stRTPSession();

	~stRTPSession();

	bool Init(stBaseRoom * p_room, WORD port);

	void Final();

	int GetSockData();

	int GetSockCtrl();

	void OnRTPData();

	void OnRTCPData();

 private:
	Cmd::stTaskId GenTaskId(struct sockaddr_in & addr);
	
	int m_stSockData;

	int m_stSockCtrl;

	stBaseRoom * m_pRoom;

	char m_stRecvBuf[64 * 1024];

	int m_iRecvCount;
};



#endif
