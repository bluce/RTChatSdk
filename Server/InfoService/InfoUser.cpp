#include "InfoUser.h"
#include "BaseRoom.h"
#include "GateTaskManager.h"
#include "BaseRoomManager.h"
#include "InfoUserManager.h"

stInfoUser::stInfoUser(QWORD temp_id, const char * unique_id, WORD task_id)
{
	roomid = 0;
	tempid = temp_id;
	taskid = task_id;
	memset(uniqueid, 0, sizeof(uniqueid));
	strncpy(uniqueid, unique_id, sizeof(uniqueid) - 1 );
	power = Cmd::POWER_TYPE_NORMAL;
}


void stInfoUser::SetPower(Cmd::enPowerType _power)
{
	Global::logger->debug("<%llu> get power <%u>", GetTempId(), _power);
	power = _power;
}


Cmd::enPowerType stInfoUser::GetPower() const
{
	return power;
}


const QWORD stInfoUser::GetTempId()
{
	return tempid;
}


const char * stInfoUser::GetUniqueId()
{
	return uniqueid;
}



WORD stInfoUser::GetTaskId() const 
{
	return taskid;
}


bool stInfoUser::IsInRoom()
{
	if(roomid != 0)
	{
		return true;
	}
	
	return false;
}


bool stInfoUser::SendCmd(defCmdId cmd_id, const ::google::protobuf::Message * send_cmd, const defCmdLen cmd_size)
{
	stGateTask * p_task = stGateTaskManager::GetInstance().GetGateTask(taskid);
	if(!p_task)
	{
		Global::logger->debug("->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>not find task <id=%llu>", taskid);
		stAssert(0);
		return false;
	}
	
	return p_task->SendCmd(tempid, cmd_id, send_cmd, cmd_size);
}


bool stInfoUser::SendCmd(const char * p_cmd, const defCmdLen cmd_len)
{
	stGateTask * p_task = stGateTaskManager::GetInstance().GetGateTask(taskid);
	if(!p_task)
	{
		stAssert(0);
		return false;
	}
	
	return p_task->SendCmd(p_cmd, cmd_len);
}


defRoomId stInfoUser::GetRoomId()
{
	return roomid;
}


void stInfoUser::EnterRoom(stBaseRoom * p_room)
{
	roomid = p_room->GetRoomId();
}


void stInfoUser::LeaveRoomNoNotify()
{
	roomid = 0;
}


void stInfoUser::LeaveRoom()
{
	stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(roomid);
	if(p_room)
	{
		p_room->LeaveRoom(this);
		Cmd::cmdRemoveRoomUser send_cmd;
		send_cmd.set_roomid(roomid);
		send_cmd.set_tempid(tempid);
		p_room->SendCmdToTrans(Cmd::enRemoveRoomUser, &send_cmd, send_cmd.ByteSize());
		Global::logger->debug("[leave_room] user leave room notify transservice <roomid=%llu,tempid=%llu>", roomid, tempid);
		SetPower(Cmd::POWER_TYPE_NORMAL);
	}
	
	roomid = 0;
}


void stInfoUser::Terminate()
{
	Cmd::cmdUnRegisterUser send_cmd;
	send_cmd.set_tempid(tempid);
	SendCmd(Cmd::enUnRegisterUser, &send_cmd, send_cmd.ByteSize());
	LeaveRoom();
	stInfoUserManager::GetInstance().RemoveUser(tempid);
}
