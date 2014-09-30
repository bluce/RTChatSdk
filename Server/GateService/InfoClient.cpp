#include "InfoClient.h"
#include "GateService.h"
#include "game.pb.h"
#include "GateUserManager.h"
#include "GateTask.h"

stInfoClient * stInfoClient::m_pInstance = NULL;

stInfoClient::stInfoClient(const char * name, const char * server_ip,const WORD & port)
	:stClientTask(name, server_ip, port)
{

}

stInfoClient::~stInfoClient()
{

}


bool stInfoClient::Init()
{
    if (!stClientTask::Init())
    {
        return false;
    }
	Start();
    return true;
}


void stInfoClient::Final()
{
	stClientTask::Final();

	Global::logger->info("[gate_server] Disconnection with Info Server. GateServer will Terminate");

	stGateService::GetInstance().Terminate();
}



bool stInfoClient::SendCmd(defTempId temp_id, Cmd::enInternalId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size)
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
	if (m_pSocket)
	{
		bool ret_code = true;
		ret_code = m_pSocket->SendCmd((const Cmd::stNullCmd *)send_buf, p_info_cmd->len());
		return ret_code;
	}

	return false;
}


bool stInfoClient::SendCmd(const char * pt_cmd, const unsigned int cmd_size)
{
	if (m_pSocket)
	{
		bool ret_code = true;
		ret_code = m_pSocket->SendCmd((const Cmd::stNullCmd *)pt_cmd, cmd_size);
		return ret_code;
	}
	
	return false;
}


bool stInfoClient::CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size)
{
	const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);
	defTempId tempid = p_info_cmd->tempid;
	defCmdId cmd_id = p_recv_cmd->cmdid;
	
	Global::logger->debug("[info_cmd] info client recv a command <id=%u>",cmd_id);

	stGateUser * p_user = stGateUserManager::GetInstance().GetUser(tempid);
	if(! p_user)
	{
		Global::logger->warn("[send_cmd_to_user] info client send cmd but not find user <taskid=%llu>", tempid);
		return true;
	}
		

	if(cmd_id == Cmd::enUnRegisterUser)
	{
		p_user->Terminate();
	}
	else
	{
		if(p_user->IsConnect())
		{
			stGateTask * p_task = p_user->GetTask();
			if(p_task && p_task->IsInit())
			{
				Global::logger->debug("[info_cmd] send a command to  <taskid=%llu,id=%u>",tempid, cmd_id);
				p_task->SendCmd((const char *)p_recv_cmd, p_info_cmd->cmdlen);
			}
			else
			{
				//stAssert(0);
			}
		}
	}

	return true;
}

