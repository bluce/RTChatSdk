#ifndef _ST_BASEROOM_H
#define _ST_BASEROOM_H

#include "Global.h"
#include "Timer.h"
#include "CommandQueue.h"
#include "Command.h"
#include "RTPSession.h"
#include "RTPUser.h"
#include "FreeUser.h"
#include "QueueUser.h"
#include "RTCPUser.h"
#include "Thread.h"
#include "NullCmd.h"
#include "internal.pb.h"
#include "game.pb.h"

class stBaseRoom: public stThread, public stCmdParse
{
 public:
	typedef std::map<defSSRCId, stRTPUser * > defRTPUserMap;
	typedef std::map<defSSRCId, stRTPUser * >::iterator defRTPUserMapIter;
	typedef std::map<defSSRCId, stRTPUser * >::value_type defRTPUserMapValue;

	typedef __gnu_cxx::hash_map<std::string, stRTCPUser *, str_hash> defRTCPUserMap;
	typedef __gnu_cxx::hash_map<std::string, stRTCPUser *, str_hash >::iterator defRTCPUserMapIter;
	typedef __gnu_cxx::hash_map<std::string, stRTCPUser *, str_hash >::value_type defRTCPUserMapValue;

	~stBaseRoom();

	const defRoomId GetRoomId();

	Cmd::enRoomType GetRoomType() const;

	int GetSockData();

	int GetSockCtrl();

	stRTPUser * GetRTPUser(defSSRCId ssrc_id);

	stRTCPUser * GetRTCPUser(const char * c_name);

	bool Init(Cmd::stTaskId & task_id);

	void OnRecvRTCPData();

	void OnRecvRTPData();

	bool IsRegisterRTP(defSSRCId ssrc_id, struct sockaddr_in & _addr);

	void AddRTPUser(defSSRCId ssrc_id, struct sockaddr_in &addr);

	void ClearPing(const char * c_name);

	bool IsRegisterRTCP(const char * c_name, defSSRCId ssrc_id, struct sockaddr_in & addr);

	void AddRTCPUser(const char * cname, const WORD sub_type_len, const defSSRCId ssrc_id, struct sockaddr_in & _addr);

	virtual void SendRTPData(QWORD task_id, const char * recv_buf, const int recv_len) = 0;

	void InitRTPUser(stRTCPUser * p_rtcp_user);

	void FindInitRTCP(stRTPUser * p_rtp_user);

	bool CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size);

	virtual void Final();

	void RemoveRTPUser(defSSRCId ssrc_id);

	void SetLeave(defTempId temp_id);

	void Run();

	static stRTCPUser * NewRTCPUser(stBaseRoom * p_room, const char * c_name, defSSRCId ssrc_id, struct sockaddr_in & _addr);
	
 protected:
	stBaseRoom(Cmd::enRoomType _type);

	void AddDataEpoll(int epoll_fd, uint32_t evts, void * p_data);

	void AddCtrlEpoll(int epoll_fd, uint32_t evts, void * p_data);
	
	stRTPSession m_stRTPSession;

	defRoomId m_dwRoomId;

	defRTPUserMap m_stRTPUse;

	defRTCPUserMap m_stRTCPUse;

	int m_iEpollDatafd;

	int m_iEpollCtrlfd;

	std::vector<struct epoll_event > m_EpollDataEventVec;

	std::vector<struct epoll_event > m_EpollCtrlEventVec;

	Cmd::enRoomType roomtype;
};

#endif
