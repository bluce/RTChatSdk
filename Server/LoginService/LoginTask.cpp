#include "LoginTask.h"
#include "game.pb.h"
#include "NullCmd.h"
#include "AssertEx.h"


stLoginTask::stLoginTask(const int sock, const struct sockaddr_in * addr, const QWORD _id): stServiceTask(sock, addr, true)
{
	///SetEnableCheck();
}


bool stLoginTask::SendCmd(defCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size)
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


bool stLoginTask::CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size)
{
	if((unsigned int)cmd_size < sizeof(Cmd::stProtoBufCmd))
	{
		Global::logger->fatal("[login_cmd_error] this user send cmd len less sizeof(stprotobufcmd) <ip=%s,port=%u>",
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
		Global::logger->fatal("[login_cmd_error] this user send cmd length error <ip=%s,port=%u,cmdid=%u,cmdsize=%u>",
							  GetIP(),GetPort(), cmd_id, cmd_size);
		Terminate();
		stAssert(0);
		return false;
	}
	
	Global::logger->debug("[login_cmd] recv a command <ip=%s,port=%u,id=%u,len=%u>",GetIP(), GetPort(), cmd_id,cmd_size);

	/// 登录
	if(cmd_id == Cmd::enRequestLogicInfo)
	{
		Cmd::cmdRequestLogicInfo recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		if(recv_cmd.appid() == "jianghu123")
		{
			static DWORD pp = 16001;
			Cmd::cmdNotifyLogicInfo send_cmd;
#ifdef _DEBUG			
			send_cmd.set_gateip("180.168.126.249");
#else
			//send_cmd.set_gateip("180.168.126.249");
			send_cmd.set_gateip("122.11.47.93");
#endif			
			send_cmd.set_gateport(pp);
			send_cmd.set_token(recv_cmd.appid());
			send_cmd.set_result(Cmd::cmdNotifyLogicInfo::LOGIC_RESULT_OK);
			SendCmd(Cmd::enNotifyLogicInfo, &send_cmd, send_cmd.ByteSize());
			pp++;
			if(pp == 16004)
			{
				pp = 16001;
			}
		}
		else if(recv_cmd.appid() == "ball456")
		{
			static DWORD pp = 16001;
			Cmd::cmdNotifyLogicInfo send_cmd;
#ifdef _DEBUG			
			send_cmd.set_gateip("180.168.126.253");
#else
			send_cmd.set_gateip("122.11.47.94");
			//send_cmd.set_gateip("180.168.126.249");
#endif			
			send_cmd.set_gateport(pp);
			send_cmd.set_token(recv_cmd.appid());
			send_cmd.set_result(Cmd::cmdNotifyLogicInfo::LOGIC_RESULT_OK);
			SendCmd(Cmd::enNotifyLogicInfo, &send_cmd, send_cmd.ByteSize());
			pp++;
			if(pp == 16004)
			{
				pp = 16001;
			}
		}
		else
		{
			Global::logger->debug("appid error <%s>", recv_cmd.appid().c_str());
			stAssert(0);
			Terminate();
		}
	}
	else
	{
		stAssert(0);
	}

	//Terminate();
	
	return true;
}


int stLoginTask::RecycleConn()
{
	return 1;
}

