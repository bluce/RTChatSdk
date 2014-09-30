#include "InfoTick.h"
#include "BaseRoom.h"
#include "InfoUserManager.h"
#include "GateTaskManager.h"
#include "TransTaskManager.h"
#include "QueueManager.h"

#define MIC_TIME 20

stBaseRoom::stBaseRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port)
{
	roomid = room_id;
	ip = r_ip;
	port = r_port;
	taskid = task_id;
	roomtype = _type;
	m_blIsTerminate = false;
}


bool stBaseRoom::IsFull()
{
	return false;
}


bool stBaseRoom::Init()
{
	return true;
}


void stBaseRoom::SetMaster(stInfoUser * p_user)
{
	masterid = p_user->GetTempId();
}


defTempId stBaseRoom::GetMasterId() const
{
	return masterid;
}

bool stBaseRoom::IsTerminate()
{
	return m_blIsTerminate;
}


void stBaseRoom::Terminate()
{
	m_blIsTerminate = true;
}


const char * stBaseRoom::GetRoomIp()
{
	return ip.c_str();
}

WORD stBaseRoom::GetRoomPort() const
{
	return port;
}


WORD stBaseRoom::GetTaskId() const
{
	return taskid;
}


const Cmd::enRoomType stBaseRoom::GetRoomType()
{
	return roomtype;
}


WORD stBaseRoom::Size() const
{
	return m_stUserMap.size();
}



void stBaseRoom::SendReConnectInfo(stInfoUser * p_user)
{
	Cmd::cmdNotifyEnterResult send_cmd;
	send_cmd.set_result(Cmd::cmdNotifyEnterResult::ENTER_RESULT_OK);
	send_cmd.set_roomid(GetRoomId());
	send_cmd.set_type(GetRoomType());
	send_cmd.set_ip(GetRoomIp());
	send_cmd.set_port(GetRoomPort());
	p_user->SendCmd(Cmd::enNotifyEnterResult, &send_cmd, send_cmd.ByteSize());
			
	NotifyAllToMe(p_user, Cmd::cmdNotifySomeEnterRoom::ENTER_REASON_RECON);
}



void stBaseRoom::KickAllUser()
{
	if(m_stUserMap.size() == 0)
	{
		return;
	}

	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_user = iter->second;
		if(p_user)
		{
			p_user->LeaveRoomNoNotify();
			Global::logger->debug("[del_room] why have this user, kick user <roomid=%llu,tempid=%llu>",
								  roomid, p_user->GetTempId());
		}
		else
		{
			stAssert(0);
		}
	}
}


void stBaseRoom::Update()
{

}


const QWORD stBaseRoom::GetRoomId()
{
	return roomid;
}


bool stBaseRoom::EnterRoom(stInfoUser * p_user)
{
	bool ret_code = false;
	if(p_user)
	{
		std::pair<defInfoUserMapIter, bool> insert_code;
		insert_code = m_stUserMap.insert(defInfoUserMapValue(p_user->GetTempId(), p_user));
		
		if(insert_code.second)
		{
			Global::logger->info("[info_user_add] add user to room list successful <roomid=%llu,tempid=%llu>",
								 GetRoomId(), p_user->GetTempId());
		}
		else
		{
			Global::logger->fatal("[info_user_add] add user to room list failed <roomid=%llu,tempid=%llu>",
								  GetRoomId(), p_user->GetTempId());
			stAssert(0);
		}
		
		ret_code = insert_code.second;
	}
	else
	{
		stAssert(0);
	}

	if(ret_code)
	{
		NotifyAllToMe(p_user);
		NotifyMeToAll(p_user);
	}

	return ret_code;
}


void stBaseRoom::NotifyAllToMe(stInfoUser * p_des_user,Cmd::cmdNotifySomeEnterRoom::enEnterReason _reason)
{
	if(m_stUserMap.size() == 0)
	{
		stAssert(0);
		return;
	}
	
	Cmd::cmdNotifySomeEnterRoom send_cmd;
	send_cmd.set_reason(_reason);

	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_user = iter->second;
		if(p_user)
		{
			Cmd::stRoomUserInfo * p_info = send_cmd.add_info();
			p_info->set_tempid(p_user->GetTempId());
			p_info->set_uniqueid(p_user->GetUniqueId());
			p_info->set_power(p_user->GetPower());
		}
		else
		{
			stAssert(0);
		}
	}
	
	p_des_user->SendCmd(Cmd::enNotifySomeEnterRoom, &send_cmd, send_cmd.ByteSize());
}


void stBaseRoom::NotifyMeToAll(stInfoUser * p_des_user)
{
	if(m_stUserMap.size() == 0)
	{
		stAssert(0);
		return;
	}
	
	Cmd::cmdNotifySomeEnterRoom send_cmd;
	send_cmd.set_reason(Cmd::cmdNotifySomeEnterRoom::ENTER_REASON_NEW);
	Cmd::stRoomUserInfo * p_info = send_cmd.add_info();
	p_info->set_tempid(p_des_user->GetTempId());
	p_info->set_uniqueid(p_des_user->GetUniqueId());
	p_info->set_power(p_des_user->GetPower());

	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_user = iter->second;
		if(p_user)
		{
			if(p_user->GetTempId() != p_des_user->GetTempId())
			{
				p_user->SendCmd(Cmd::enNotifySomeEnterRoom, &send_cmd, send_cmd.ByteSize());
			}
		}
		else
		{
			stAssert(0);
		}
	}
}



void stBaseRoom::LeaveRoom(stInfoUser * p_user)
{
	defTempId user_id = p_user->GetTempId();
	defInfoUserMapIter iter = m_stUserMap.find(user_id);
	if(iter != m_stUserMap.end())
	{
		Global::logger->debug("[room_user_remove] remove a user from room <roomid=%llu, tempid=%llu>",roomid, user_id);
		/// delete iter->second;
		m_stUserMap.erase(iter);
		Cmd::cmdNotifySomeLeaveRoom send_cmd;
		send_cmd.set_tempid(user_id);
		BroadCast(Cmd::enNotifySomeLeaveRoom, &send_cmd, send_cmd.ByteSize());
	}
	else
	{
		stAssert(0);
	}
}



bool stBaseRoom::BroadCast(Cmd::enCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd,const defCmdLen cmd_size)
{
	if(m_stUserMap.size() == 0)
	{
		return false;
	}

	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_user = iter->second;
		if(p_user)
		{
			p_user->SendCmd(cmd_id, pt_null_cmd, cmd_size);
		}
		else
		{
			stAssert(0);
		}
	}
	
	return true;
}



bool stBaseRoom::BroadCast(const char * p_cmd, const defCmdLen cmd_len)
{
	if(m_stUserMap.size() == 0)
	{
		return false;
	}

	defInfoUserMapIter iter = m_stUserMap.begin();
	for(; iter != m_stUserMap.end(); iter++)
	{
		stInfoUser * p_user = iter->second;
		if(p_user)
		{
			p_user->SendCmd(p_cmd, cmd_len);
		}
		else
		{
			stAssert(0);
		}
	}
	
	return true;
}



bool stBaseRoom::SendCmdToTrans(defCmdId cmd_id, const ::google::protobuf::Message * send_cmd, const defCmdLen cmd_size)
{
	stTransTask * p_task = stTransTaskManager::GetInstance().GetTransTask(taskid);
	if(!p_task)
	{
		stAssert(0);
		return false;
	}
	
	return p_task->SendCmd(cmd_id, send_cmd, cmd_size);
}
