#ifndef _ST_INFOUSER_H
#define _ST_INFOUSER_H

#include "public.pb.h"
#include "Global.h"
#include "AssertEx.h"
#include "Type.h"
#include "GateTask.h"
#include "NullCmd.h"

class stBaseRoom;

class stInfoUser
{
 public:
	stInfoUser(defTempId tempid, const char * unique_id, WORD task_id);

	const defTempId GetTempId();

	WORD GetTaskId() const;

	bool SendCmd(defCmdId cmd_id, const ::google::protobuf::Message * send_cmd, const defCmdLen cmd_size);

	bool SendCmd(const char * p_cmd, const defCmdLen cmd_len);

	void EnterRoom(stBaseRoom * p_room);

	void LeaveRoom();

	void LeaveRoomNoNotify();

	defRoomId GetRoomId();

	bool IsInRoom();

	void Terminate();

	const char * GetUniqueId();

	void SetPower(Cmd::enPowerType _power);

	Cmd::enPowerType GetPower() const;

 private:
	
	defTempId tempid;

	WORD taskid;

	defRoomId roomid;

	Cmd::enPowerType power;

	char uniqueid[UNIQUE_LEN];
};

#endif
