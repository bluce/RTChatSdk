#include "InfoUserManager.h"
#include "BaseRoomManager.h"
#include "GateTaskManager.h"
#include "TransTaskManager.h"
#include "NullCmd.h"
#include "QueueRoom.h"
#include "FreeRoom.h"
#include "TwoRoom.h"
#include "OnlyRoom.h"

stGateTask::stGateTask(const int sock, const struct sockaddr_in * addr): stServiceTask(sock, addr)
{
	Resize(SVR_CMDSIZE);
	m_blInit = false;
}


void stGateTask::SetTaskId(const WORD & task_id)
{
	m_wdTaskId = task_id;
	stGateTaskManager::GetInstance().AddTask(this);	
	m_blInit = true;
}


const WORD stGateTask::GetTaskId()
{
	return m_wdTaskId;
}


bool stGateTask::IsInit()
{
	return m_blInit;
}


bool stGateTask::SendCmd(defTempId temp_id, defCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size)
{
	if(cmd_id == 0)
	{
		return false;
	}
	
	char buf[cmd_size + sizeof(Cmd::stProtoBufCmd)];
	memset(buf, 0, sizeof(buf));
	Cmd::stProtoBufCmd * p_send_cmd = (Cmd::stProtoBufCmd *)buf;
	p_send_cmd->cmdid = cmd_id;
	p_send_cmd->cmdlen = cmd_size;
	if(cmd_size)
	{
		Cmd::Serialize(p_send_cmd, pt_null_cmd);
	}

	char send_buf[p_send_cmd->len()+sizeof(Cmd::stInternalCmd)];
	memset(send_buf, 0, sizeof(send_buf));
	Cmd::stInternalCmd * p_info_cmd = (Cmd::stInternalCmd *)send_buf;
	p_info_cmd->tempid = temp_id;
	p_info_cmd->cmdlen = p_send_cmd->len();
	memcpy(p_info_cmd->data, buf, p_info_cmd->cmdlen);

	return SendCmd(send_buf, p_info_cmd->len());
}


bool stGateTask::SendCmd(const char * p_send_cmd, const int cmd_size)
{
	return stServiceTask::SendCmd(p_send_cmd, cmd_size);
}


int stGateTask::RecycleConn()
{
	return 1;
}


void stGateTask::AddTaskToManager()
{
	
}


void stGateTask::RemoveFromManager()
{
	Global::logger->warn("[gate_service] gate service crash <taskid=%llu>", m_wdTaskId);
	stGateTaskManager::GetInstance().RemoveTask(this);
	/// 删除此网关用户
	stInfoUserManager::GetInstance().RemoveGateService(m_wdTaskId);
}


bool stGateTask::CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size)
{
	const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);

	defCmdId cmd_id = p_recv_cmd->cmdid;

	if(cmd_id == Cmd::enRegisterSvr)
	{
		Cmd::cmdRegisterSvr recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		SetTaskId(recv_cmd.svrid());
	}
	else
	{
		if(IsInit())
		{
			Put(pt_null_cmd, cmd_size);
		}
		else
		{
			Global::logger->fatal("[add_task] not init but send cmd <cmdid=%u,taskid=%llu>", cmd_id, GetTaskId());
			stAssert(0);
			Terminate();
		}
	}
	
	return true;
}



bool stGateTask::CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size)
{
	const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
	
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);

	defTempId tempid = p_info_cmd->tempid;

	defCmdId cmd_id = p_recv_cmd->cmdid;

	Global::logger->debug("[recv_cmd] gatetask recv a cmd <tempid=%llu, id=%u,len=%u>",tempid, cmd_id, cmd_size);

	/// 注册用户
	if(cmd_id == Cmd::enRegisterUser)
	{
		Cmd::cmdRegisterUser recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);

		/// 重链
		if(recv_cmd.isrec())
		{
			stInfoUser * p_old_user = stInfoUserManager::GetInstance().GetUser(tempid);
			if(p_old_user)
			{
				if(p_old_user->IsInRoom())
				{
					stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(p_old_user->GetRoomId());
					if(p_room)
					{
						
						p_room->SendReConnectInfo(p_old_user);
					}
				}
			}
			else
			{
				Global::logger->debug("not find this user <tempid=%llu>", tempid);
				stAssert(0);
			}
		}
		else
		{
			stInfoUser * p_user = new stInfoUser(tempid, recv_cmd.uniqueid().c_str(), GetTaskId());
			stInfoUserManager::GetInstance().AddUser(p_user);
			return true;
		}
	}

	///删除用户
	if(cmd_id == Cmd::enUnRegisterUser)
	{
		stInfoUser * p_user = stInfoUserManager::GetInstance().GetUser(tempid);
		if(! p_user)
		{
			return true;
		}
		Global::logger->debug("[remove_usre] remove a user from gatetask terminate <tempid=%llu>", tempid);
		p_user->Terminate();
		return true;
	}

	
	stInfoUser * p_user = stInfoUserManager::GetInstance().GetUser(tempid);
	if(! p_user)
	{
		stAssert(0);
		return true;
	}
		

	/// 请求创建房间 
	if(cmd_id == Cmd::enRequestCreateRoom)
	{
		if(p_user->IsInRoom())
		{
			Global::logger->debug("[create_room] user in room but send create <tempid=%llu, roomid=%llu>",
								  tempid, p_user->GetRoomId());
			//p_user->Terminate();
			//stAssert(0);
			return true;
		}

		Cmd::cmdRequestCreateRoom recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		if(!(recv_cmd.type() > Cmd::ROOM_TYPE_MIN && recv_cmd.type() < Cmd::ROOM_TYPE_MAX))
		{
			Global::logger->warn("[create_room] request create room but type=%u", recv_cmd.type());
			//stAssert(0);
			//p_user->Terminate();
			return true;
		}

		if(!(recv_cmd.reason() > Cmd::ROOM_REASON_MIN && recv_cmd.reason() < Cmd::ROOM_REASON_MAX))
		{
			Global::logger->warn("[create_room] request create room reason but reason=%u", recv_cmd.reason());
			//stAssert(0);
			//p_user->Terminate();
			return true;
		}
		
		stTransTask * p_task = stTransTaskManager::GetInstance().GetMinTask();
		if(p_task)
		{
			p_task->SendCmd((const char * )pt_null_cmd, cmd_size);
		}
		else
		{
			stAssert(0);
			Cmd::cmdNotifyCreateResult send_cmd;
			send_cmd.set_isok(false);
			SendCmd(tempid, Cmd::enNotifyCreateResult, &send_cmd, send_cmd.ByteSize());
			return true;
		}
	}

	/// 请求加入房间 
	else if(cmd_id == Cmd::enRequestEnterRoom)
	{
		if(p_user->IsInRoom())
		{
			Global::logger->debug("[enter_room] user in room but send enter <tempid=%llu, roomid=%llu>",
								  tempid, p_user->GetRoomId());
			
			//p_user->Terminate();
			
			//stAssert(0);
			return true;
		}
		
		Cmd::cmdRequestEnterRoom recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(recv_cmd.roomid());
		if(!p_room)
		{
			stAssert(0);
			Cmd::cmdNotifyEnterResult send_cmd;
			send_cmd.set_result(Cmd::cmdNotifyEnterResult::ENTER_RESULT_NOEXITS);
			SendCmd(tempid, Cmd::enNotifyEnterResult, &send_cmd, send_cmd.ByteSize());
			return true;
		}
		
		if(! p_room->IsFull())
		{
			Cmd::cmdNotifyEnterResult send_cmd;
			send_cmd.set_result(Cmd::cmdNotifyEnterResult::ENTER_RESULT_OK);
			send_cmd.set_roomid(p_room->GetRoomId());
			send_cmd.set_type(p_room->GetRoomType());
			send_cmd.set_ip(p_room->GetRoomIp());
			send_cmd.set_port(p_room->GetRoomPort());
			SendCmd(tempid, Cmd::enNotifyEnterResult, &send_cmd, send_cmd.ByteSize());
			Global::logger->debug("[enter_room_return] <tempid=%llu, ip=%s,port=%u>", tempid, send_cmd.ip().c_str(),
								  send_cmd.port());

			p_room->EnterRoom(p_user);
			
			p_user->EnterRoom(p_room);
			
			return true;
		}
		else
		{
			Cmd::cmdNotifyEnterResult send_cmd;
			send_cmd.set_result(Cmd::cmdNotifyEnterResult::ENTER_RESULT_FULL);
			SendCmd(tempid, Cmd::enNotifyEnterResult, &send_cmd, send_cmd.ByteSize());
			Global::logger->debug("[enter_room] enter room but full <roomid=%llu,tempid=%llu>", recv_cmd.roomid(), tempid);
		}
		
		return true;
	}

	/// 离开房间
	else if(cmd_id == Cmd::enRequestLeaveRoom)
	{
		if(! p_user->IsInRoom())
		{
			Global::logger->debug("[leave_room] user not in room but send leave <tempid=%llu>", tempid);
			//p_user->Terminate();
			//stAssert(0);
			return true;
		}
		
		p_user->LeaveRoom();
		return true;
	}

	/// 房间列表
	else if(cmd_id == Cmd::enRequestRoomList)
	{
		
		Cmd::cmdNotifyRoomList send_cmd;

		/// 可以优化,不要每次都序列化一次
		if(stBaseRoomManager::GetInstance().GetRoomInfos(send_cmd))
		{
			SendCmd(tempid, Cmd::enNotifyRoomList, &send_cmd, send_cmd.ByteSize());
		}
		
		return true;
	}

	/// 请求麦序
	else if(cmd_id == Cmd::enRequestJoinMicQueue)
	{
		if(! p_user->IsInRoom())
		{
			Global::logger->debug("[enter_room] user in room but send enter <tempid=%llu>",tempid);
			//p_user->Terminate();
			//stAssert(0);
			return true;
		}

		stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(p_user->GetRoomId());
		if(!p_room)
		{
			//p_user->Terminate();
			//stAssert(0);
			return true;
		}
		
		if(p_room->GetRoomType() == Cmd::ROOM_TYPE_QUEUE)
		{
			stQueueRoom * p_queue_room = (stQueueRoom *)p_room;
			p_queue_room->AddQueueUser(p_user);
		}
		else if(p_room->GetRoomType() == Cmd::ROOM_TYPE_ONLY)
		{
			if(p_user->GetPower() < Cmd::POWER_TYPE_MASTER)
			{
				Global::logger->debug("request mic but power less error");
				return true;
			}
			
			stOnlyRoom * p_only_room = (stOnlyRoom *)p_room;
			p_only_room->AddQueueUser(p_user);
		}
		else
		{
			stAssert(0);
			return false;
		}
	}

	/// 离开麦序
	else if(cmd_id == Cmd::enRequestLeaveMicQueue)
	{
		Global::logger->debug("[leave_mic] leave mic queue <%llu>", p_user->GetTempId());
		if(!p_user->IsInRoom())
		{
			//p_user->Terminate();
			//stAssert(0);
			return true;
		}

		stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(p_user->GetRoomId());
		if(!p_room)
		{
			//p_user->Terminate();
			//stAssert(0);
			return true;
		}
		
		if(p_room->GetRoomType() == Cmd::ROOM_TYPE_QUEUE)
		{
			stQueueRoom * p_queue_room = (stQueueRoom *)p_room;
			p_queue_room->DelQueueUser(p_user);
		}
		else if(p_room->GetRoomType() == Cmd::ROOM_TYPE_ONLY)
		{
			stOnlyRoom * p_only_room = (stOnlyRoom *)p_room;
			p_only_room->DelQueueUser(p_user);
		}
		else
		{
			stAssert(0);
			return false;
		}
	}

	/// 返回随机聊天
	else if(cmd_id == Cmd::enReturnRandChat)
	{
		Cmd::cmdReturnRandChat recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		if(!recv_cmd.isok())
		{
			Cmd::cmdReturnRandChat send_cmd;
			send_cmd.set_isok(false);
			//SendCmd(recv_cmd.tempid(), Cmd::enReturnRandChat, &send_cmd, send_cmd.ByteSize());
			stInfoUser * p_user = stInfoUserManager::GetInstance().GetUser(recv_cmd.tempid());
			if(p_user)
			{
				p_user->SendCmd(Cmd::enReturnRandChat, &send_cmd, send_cmd.ByteSize());
			}
			
			return true;
		}
	}

	/// 请求漂流瓶
	else if(cmd_id == Cmd::enRequestRandPlay)
	{

		stInfoUser * p_dest_user = stInfoUserManager::GetInstance().GetRandUser(tempid);
		if(p_dest_user)
		{
			Cmd::cmdRequestRandPlay recv_cmd;
			recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
			
			Cmd::cmdNotifyRandPlay send_cmd;
			send_cmd.set_tempid(p_user->GetTempId());
			send_cmd.set_url(recv_cmd.url());
			p_dest_user->SendCmd(Cmd::enNotifyRandPlay, &send_cmd, send_cmd.ByteSize());
			Global::logger->debug("find any user <%llu>", p_dest_user->GetTempId());
			return true;
		}
		else
		{
			Global::logger->debug("not find any user");
		}
		
		return true;
	}

	/// 请求更新权限
	else if(cmd_id == Cmd::enRequestUpdatePower)
	{
		if(! p_user->IsInRoom())
		{
			return true;
		}

		stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(p_user->GetRoomId());
		if(!p_room)
		{
			return true;
		}

		Cmd::cmdNotifyUpdatePowerResult send_cmd;
		
		if(p_user->GetTempId() != p_room->GetMasterId())
		{
			send_cmd.set_result(Cmd::cmdNotifyUpdatePowerResult::UPDATE_RESULT_NOPOWER);
			p_user->SendCmd(Cmd::enNotifyUpdatePowerResult, &send_cmd, send_cmd.ByteSize());
			return true;
		}

		Cmd::cmdRequestUpdatePower recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);

		if(p_room->GetRoomType() == Cmd::ROOM_TYPE_ONLY)
		{

			Global::logger->debug("update power <%u>", recv_cmd.power());
			
			stOnlyRoom * p_only_room = (stOnlyRoom *)p_room;
			stInfoUser * p_dest_user = stInfoUserManager::GetInstance().GetUser(recv_cmd.tempid());
			if(!p_dest_user)
			{
				send_cmd.set_result(Cmd::cmdNotifyUpdatePowerResult::UPDATE_RESULT_NOUSER);
				p_user->SendCmd(Cmd::enNotifyUpdatePowerResult, &send_cmd, send_cmd.ByteSize());
				return true;
			}
			
			p_dest_user->SetPower(recv_cmd.power());
			
			Cmd::cmdNotifyUpdatePower s_result;
			s_result.set_tempid(recv_cmd.tempid());
			s_result.set_power(recv_cmd.power());
			p_only_room->BroadCast(Cmd::enNotifyUpdatePower, &s_result, s_result.ByteSize());
			
		}
		else
		{
			send_cmd.set_result(Cmd::cmdNotifyUpdatePowerResult::UPDATE_RESULT_TYPEERR);
			p_user->SendCmd(Cmd::enNotifyUpdatePowerResult, &send_cmd, send_cmd.ByteSize());
			return true;
		}
		
		return true;
	}

	/// 分配麦
	else if(cmd_id == Cmd::enRequestAssignMic)
	{
		if(! p_user->IsInRoom())
		{
			return true;
		}

		stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(p_user->GetRoomId());
		if(!p_room)
		{
			return true;
		}

		Cmd::cmdNotifyAssignResult send_cmd;
		
		if(p_user->GetPower() < Cmd::POWER_TYPE_MASTER)
		{
			send_cmd.set_result(Cmd::cmdNotifyAssignResult::ASSIGN_RESULT_NOPOWER);
			p_user->SendCmd(Cmd::enNotifyAssignResult, &send_cmd, send_cmd.ByteSize());
			return true;
		}

		Cmd::cmdRequestUpdatePower recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		
		if(p_room->GetRoomType() == Cmd::ROOM_TYPE_ONLY)
		{
			stOnlyRoom * p_only_room = (stOnlyRoom *)p_room;
			if(recv_cmd.tempid() == 0)
			{
				p_only_room->ClearMic();
				return true;
			}
			
			stInfoUser * p_dest_user = stInfoUserManager::GetInstance().GetUser(recv_cmd.tempid());
			if(!p_dest_user)
			{
				send_cmd.set_result(Cmd::cmdNotifyAssignResult::ASSIGN_RESULT_NOUSER);
				p_user->SendCmd(Cmd::enNotifyAssignResult, &send_cmd, send_cmd.ByteSize());				
				return true;
			}
			
			p_only_room->AddQueueUser(p_dest_user);
		}
		else
		{
			send_cmd.set_result(Cmd::cmdNotifyAssignResult::ASSIGN_RESULT_TYPEERR);
			p_user->SendCmd(Cmd::enNotifyAssignResult, &send_cmd, send_cmd.ByteSize());
			return true;
		}
		
		return true;
	}
	
	return true;
}
