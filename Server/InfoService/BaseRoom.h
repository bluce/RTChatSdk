#ifndef _ST_BASEROOM_H
#define _ST_BASEROOM_H

#include "Global.h"
#include "AssertEx.h"
#include "internal.pb.h"
#include "game.pb.h"
#include "Type.h"
#include "GateTask.h"
#include "NullCmd.h"

class stInfoUser;

class stBaseRoom
{
 public:
	typedef std::map<defRoomId, stInfoUser * > defInfoUserMap;
	typedef std::map<defRoomId, stInfoUser * >::iterator defInfoUserMapIter;
	typedef std::map<defRoomId, stInfoUser * >::value_type defInfoUserMapValue;

	stBaseRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port);

	const QWORD GetRoomId();

	const char * GetRoomIp();

	WORD GetRoomPort() const;

	WORD GetTaskId() const;

	const Cmd::enRoomType GetRoomType();

	virtual bool IsFull();
	
	virtual bool Init();

	virtual void Update();

	virtual bool EnterRoom(stInfoUser * p_user);

	virtual void LeaveRoom(stInfoUser * p_user);

	virtual void SendReConnectInfo(stInfoUser * p_user);

	void SetMaster(stInfoUser * p_user);

	defTempId GetMasterId() const;

	bool IsTerminate();

	void Terminate();

	void KickAllUser();

	void NotifyAllToMe(stInfoUser * p_user,
					   Cmd::cmdNotifySomeEnterRoom::enEnterReason = Cmd::cmdNotifySomeEnterRoom::ENTER_REASON_NEW);
	
	void NotifyMeToAll(stInfoUser * p_user);

	bool BroadCast(Cmd::enCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd,const defCmdLen cmd_size);

	bool BroadCast(const char * p_cmd, const defCmdLen cmd_len);

	bool SendCmdToTrans(defCmdId cmd_id, const ::google::protobuf::Message * send_cmd, const defCmdLen cmd_size);

	WORD Size() const;

 protected:
	stRTime m_stRTime;

	defInfoUserMap m_stUserMap;
		
	WORD taskid;
	
	std::string ip;
	
	unsigned int port;
	
	defRoomId roomid;

	defTempId masterid;

	Cmd::enRoomType roomtype;

	bool m_blIsTerminate;
};

#endif
