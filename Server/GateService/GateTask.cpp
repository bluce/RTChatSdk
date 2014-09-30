#include "internal.pb.h"
#include "game.pb.h"
#include "GateTask.h"
#include "InfoClient.h"
#include "NullCmd.h"
#include "AssertEx.h"
#include "GateUserManager.h"
#include "GateTaskManager.h"
#include "GateTick.h"


stGateTask::stGateTask(const int sock, const struct sockaddr_in * addr, const QWORD _id): stServiceTask(sock, addr, true)
{
	SetEnableCheck();

	taskid = _id;
	m_blInit = false;
}


QWORD stGateTask::GetTaskId()
{
	return taskid;
}


QWORD stGateTask::GetUserId()
{
	return userid;
}


void stGateTask::SetTaskId(QWORD task_id)
{
	taskid = task_id;
}


bool stGateTask::IsInit()
{
	return m_blInit;
}


void stGateTask::SetInit(defTempId _id)
{
	stGateTaskManager::GetInstance().AddTask(this);
	m_blInit = true;
	userid = _id;
}



bool stGateTask::SendCmd(const char * send_cmd, const unsigned int cmd_len)
{
	return SendWebData(send_cmd, cmd_len);
}



bool stGateTask::SendCmd(defCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size)
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
	
	return SendWebData(buf, p_send_cmd->len());
}



bool stGateTask::SendCmdToInfo(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size)
{
	char buf[cmd_size + sizeof(Cmd::stInternalCmd)];
	memset(buf, 0, sizeof(buf));
	Cmd::stInternalCmd * p_send_cmd = (Cmd::stInternalCmd *)buf;
	p_send_cmd->tempid = userid;
	p_send_cmd->cmdlen = cmd_size;
	memcpy(p_send_cmd->data, pt_null_cmd, cmd_size);
	stInfoClient::GetInstance().SendCmd(buf, p_send_cmd->len());
	return true;
}



bool stGateTask::SendCmdToInfo(defCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size)
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

	char send_buf[p_send_cmd->len() + sizeof(Cmd::stInternalCmd)];
	memset(send_buf, 0, sizeof(send_buf));
	Cmd::stInternalCmd * p_info_cmd = (Cmd::stInternalCmd *)send_buf;
	p_info_cmd->tempid = userid;
	p_info_cmd->cmdlen = p_send_cmd->len();
	memcpy(p_info_cmd->data, buf, p_info_cmd->cmdlen);
	stInfoClient::GetInstance().SendCmd(send_buf, p_info_cmd->len());
	return true;
}


int stGateTask::RecycleConn()
{
	return 1;
}


void stGateTask::AddTaskToManager()
{		
	stGateTaskManager::GetInstance().AddTask(this);
}


void stGateTask::RemoveFromManager()
{
	Global::logger->debug("task terminate <userid=%llu,taskid=%llu>", GetUserId(), GetTaskId());
	
	stGateUser * p_user = stGateUserManager::GetInstance().GetUser(GetUserId());
	if(p_user)
	{
		p_user->DisConnect(GetTaskId(), stGateTick::m_stRTime.GetMSec());
	}
	else
	{
		Global::logger->debug("why not find user <%llu>", GetUserId());
		stAssert(0);
	}

	stGateTaskManager::GetInstance().RemoveTask(this);
}


bool stGateTask::CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size)
{
	if((unsigned int)cmd_size < sizeof(Cmd::stProtoBufCmd))
	{
		Global::logger->fatal("[gate_cmd_error] this user send cmd len less sizeof(stprotobufcmd) <ip=%s,port=%u>",
							  GetIP(),GetPort());
		Terminate();
		stAssert(0);
		return false;
	}
	
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)pt_null_cmd;

	DWORD cmd_id = p_recv_cmd->cmdid;
	
	defCheckCmdLen(cmd_id, p_recv_cmd->cmdlen, (defCmdId)cmd_size);
	
	if(!is_check)
	{
		Global::logger->fatal("[gate_cmd_error] this user send cmd length error <ip=%s,port=%u,cmdid=%u,cmdsize=%u>",
							  GetIP(),GetPort(), cmd_id, cmd_size);
		Terminate();
		stAssert(0);
		return false;
	}
	
	Global::logger->debug("[gate_cmd] recv a command <ip=%s,port=%u,id=%u,len=%u>",GetIP(), GetPort(), cmd_id,cmd_size);

	if(IsInit())
	{
		stGateUser * p_user = stGateUserManager::GetInstance().GetUser(userid);
		if(!p_user)
		{
			Terminate();
			return true;
		}
		
		SendCmdToInfo(pt_null_cmd, cmd_size);
	}
	else
	{
		/// 登录
		if(cmd_id == Cmd::enRequestLogin)
		{
			Cmd::cmdRequestLogin p_recv;
			p_recv.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		
			Global::logger->debug("[login_cmd] recv login para <token=%s,uniqueid=%s>",
								  p_recv.token().c_str(), p_recv.uniqueid().c_str());

			Cmd::cmdRegisterUser info_cmd;
			info_cmd.set_isrec(false);
			
			if(p_recv.has_tempid())
			{				
				stGateUser * p_old_user = stGateUserManager::GetInstance().GetUser(p_recv.tempid());
				if(p_old_user)
				{
					SetInit(p_recv.tempid());
					p_old_user->Connect();
					p_old_user->SetTask(this);
					
					info_cmd.set_isrec(true);
					info_cmd.set_tempid(p_old_user->GetTempId());
					info_cmd.set_uniqueid(p_old_user->GetUniqueId());

					SendCmdToInfo(Cmd::enRegisterUser, &info_cmd, info_cmd.ByteSize());

					Global::logger->debug("reconnect <tempid=%llu>", p_recv.tempid());

					Cmd::cmdNotifyLoginResult send_cmd;
					send_cmd.set_result(Cmd::cmdNotifyLoginResult::LOGIN_RESULT_OK);
					send_cmd.set_tempid(p_recv.tempid());
					SendCmd(Cmd::enNotifyLoginResult, &send_cmd, send_cmd.ByteSize());
					
					return true;
				}
			}

			stGateUser * p_new_user = new stGateUser(GetTaskId(), p_recv.uniqueid().c_str());
			p_new_user->SetTask(this);
			stGateUserManager::GetInstance().AddUser(p_new_user);
				
			Cmd::cmdNotifyLoginResult send_cmd;
			send_cmd.set_result(Cmd::cmdNotifyLoginResult::LOGIN_RESULT_OK);
			send_cmd.set_tempid(GetTaskId());
			SendCmd(Cmd::enNotifyLoginResult, &send_cmd, send_cmd.ByteSize());
			
			SetInit(GetTaskId());
			
			info_cmd.set_tempid(GetTaskId());
			info_cmd.set_uniqueid(p_recv.uniqueid());
			SendCmdToInfo(Cmd::enRegisterUser, &info_cmd, info_cmd.ByteSize());
		}
		else
		{
			Global::logger->debug("recv error cmd <%u>", cmd_id);
			stAssert(0);
			Terminate();
		}
	}

	return true;
}
