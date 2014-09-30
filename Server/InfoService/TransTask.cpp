#include "BaseRoomManager.h"
#include "InfoUserManager.h"
#include "TransTask.h"
#include "GateTask.h"
#include "TransTaskManager.h"
#include "NullCmd.h"


stTransTask::stTransTask(const int sock, const struct sockaddr_in * addr): stServiceTask(sock, addr)
{
	Resize(SVR_CMDSIZE);
	m_wdTransId = 0;
	m_dwRoomCount = 0;
	m_blInit = false;
}


void stTransTask::SetTransId(const WORD & zone_id)
{
	m_wdTransId = zone_id;
	stTransTaskManager::GetInstance().AddTask(this);
	m_blInit = true;
}


const WORD stTransTask::GetTransId()
{
	return m_wdTransId;
}



void stTransTask::AddRoomCount()
{
	m_dwRoomCount++;
}



void stTransTask::SubRoomCount()
{
	if(m_dwRoomCount > 0)
	{
		m_dwRoomCount--;
	}
}



DWORD stTransTask::GetRoomCount()
{
	return m_dwRoomCount;
}



bool stTransTask::IsInit()
{
	return m_blInit;
}



bool stTransTask::SendCmd(defCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size)
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
	p_info_cmd->tempid = 0;
	p_info_cmd->cmdlen = p_send_cmd->len();
	memcpy(p_info_cmd->data, buf, p_info_cmd->cmdlen);

	return SendCmd(send_buf, p_info_cmd->len());
}


bool stTransTask::SendCmd(const char * p_send_cmd, const int cmd_size)
{
	return stServiceTask::SendCmd(p_send_cmd, cmd_size);
}


bool stTransTask::CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size)
{
	const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);

	defCmdId cmd_id = p_recv_cmd->cmdid;

	if(cmd_id == Cmd::enRegisterSvr)
	{
		Cmd::cmdRegisterSvr recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		SetTransId(recv_cmd.svrid());
	}
	else
	{
		if(IsInit())
		{
			Put(pt_null_cmd, cmd_size);
		}
		else
		{
			stAssert(0);
			Terminate();
		}
	}
	
	return true;
}


bool stTransTask::CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size)
{
	const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);
	
	defTempId tempid = p_info_cmd->tempid;
	defCmdId cmd_id = p_recv_cmd->cmdid;

	Global::logger->debug("[infoserver_recv_cmd] zone task recv a command <id=%u>",cmd_id);


	if(cmd_id == Cmd::enNotifyCreateResult)
	{
		Cmd::cmdNotifyCreateResult recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);

		stInfoUser * p_user = stInfoUserManager::GetInstance().GetUser(tempid);
		
		if(recv_cmd.roomid() == 0)
		{
			stAssert(0);
			return false;
		}
		
		
		if(p_user)
		{
			p_user->SendCmd((const char *)pt_null_cmd, cmd_size);
			
#ifdef _DEBUG			
			const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
			const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);
			Cmd::cmdNotifyCreateResult recv_cmd;
			recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
			Global::logger->debug("[create_room_return] <tempid=%llu,roomid=%llu,ip=%s,port=%u>",
								  tempid,
								  recv_cmd.roomid(),
								  recv_cmd.ip().c_str(),
								  recv_cmd.port());
#endif
		}
		else
		{
			stAssert(0);
		}

		stBaseRoom * p_room = stBaseRoomManager::GetInstance().CreateRoom(GetTransId(), recv_cmd.roomid(),
																		  recv_cmd.type(),
																		  recv_cmd.ip().c_str(), recv_cmd.port());
		if(p_room)
		{
			p_room->Init();
			stBaseRoomManager::GetInstance().AddRoom(p_room);
		}

		p_room->SetMaster(p_user);

		p_user->SetPower(Cmd::POWER_TYPE_MASTER);
		
		p_room->EnterRoom(p_user);

		p_user->EnterRoom(p_room);

		Global::logger->debug("[notify_rand_chat] notify rand chat begin  <reason=%u>", recv_cmd.reason());
		if(recv_cmd.reason() == Cmd::ROOM_REASON_RANDCHAT)
		{
			Cmd::cmdReturnRandChat send_cmd;
			send_cmd.set_isok(false);
			
			if(stInfoUserManager::GetInstance().Size() == 1)
			{
				p_user->SendCmd(Cmd::enReturnRandChat, &send_cmd, send_cmd.ByteSize());
				return true;
			}

			stInfoUser * p_dest_user = stInfoUserManager::GetInstance().GetNotInRoomUser(tempid);
			if(! p_dest_user)
			{
				Global::logger->debug("[notify_rand_chat] notify rand chat not find <roomid=%llu,uniqueid=%s>", recv_cmd.roomid(),
								  p_user->GetUniqueId());
				p_user->SendCmd(Cmd::enReturnRandChat, &send_cmd, send_cmd.ByteSize());
				return true;
			}

			Cmd::cmdNotifyRandChat rand_chat;
			rand_chat.set_tempid(tempid);
			rand_chat.set_uniqueid(p_user->GetUniqueId());
			rand_chat.set_roomid(recv_cmd.roomid());
			p_dest_user->SendCmd(Cmd::enNotifyRandChat, &rand_chat, rand_chat.ByteSize());
			Global::logger->debug("[notify_rand_chat] notify rand chat <roomid=%llu,uniqueid=%s>", recv_cmd.roomid(),
								  p_user->GetUniqueId());
		}
		
	}
	else if(cmd_id == Cmd::enDelRoom)
	{
		Cmd::cmdDelRoom recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		stBaseRoom * p_room = stBaseRoomManager::GetInstance().GetRoom(recv_cmd.roomid());
		if(!p_room)
		{
			Global::logger->warn("[del_room] del room but not find room <roomid=%llu>", recv_cmd.roomid());
			stAssert(0);
			return true;
		}

		Global::logger->warn("[del_room] del room <roomid=%llu>", recv_cmd.roomid());
		p_room->Terminate();
	}
	else
	{
		stInfoUser * p_user = stInfoUserManager::GetInstance().GetUser(tempid);
		if(p_user)
		{
			Global::logger->debug("transservice send a command to <tempid=%llu, cmd_id=%u>", tempid, cmd_id);
			p_user->SendCmd((const char *)pt_null_cmd, cmd_size);
		}
		else
		{
			Global::logger->debug("transservice send a command to <tempid=%llu, cmd_id=%u>", tempid, cmd_id);
			//stAssert(0);
		}
	}
	
	
	return true;
}


int stTransTask::RecycleConn()
{
	return 1;
}


void stTransTask::AddTaskToManager()
{
	
}


void stTransTask::RemoveFromManager()
{
	stTransTaskManager::GetInstance().RemoveTask(this);
	/// 删除所有的房间
}



