#include "game.pb.h"
#include "AssertEx.h"
#include "InfoClient.h"
#include "NullCmd.h"
#include "RoomManager.h"

stInfoClient * stInfoClient::m_pInstance = NULL;
 
bool stInfoClient::Init()
{
	if(! stRClientTask::Init())
	{
		return false;
	}

	return true;
}


void stInfoClient::ClientFinal()
{
	Terminate(true);
	TTerminate();
	Join();
}


bool stInfoClient::SendCmd(defTempId temp_id, Cmd::enCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size)
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
	p_info_cmd->tempid = temp_id;
	p_info_cmd->cmdlen = p_send_cmd->len();
	memcpy(p_info_cmd->data, buf, p_info_cmd->cmdlen);
	return stRClientTask::SendCmdInConnect(send_buf, p_info_cmd->len());
}



bool stInfoClient::SendCmd(const char * cmd, const defCmdLen cmd_len)
{
	return stRClientTask::SendCmdInConnect(cmd, cmd_len);
}



bool stInfoClient::CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size)
{
	const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);
	defTempId tempid = p_info_cmd->tempid;
	defCmdId cmd_id = p_recv_cmd->cmdid;

	Global::logger->debug("[infoclient_recv_cmd] info client recv a command from cmd queue <id=%u,size=%u,proto_size=%u>",
						  cmd_id,cmd_size, p_recv_cmd->cmdlen);

	/// 创建房间
	if(cmd_id == Cmd::enRequestCreateRoom)
	{
		Cmd::cmdRequestCreateRoom recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		Global::logger->debug("[request_create_room] request create room <tempid=%llu,type=%u>", tempid, recv_cmd.type());
		
		Cmd::stTaskId room_id = stRoomManager::GetInstance().CreateRoom(recv_cmd.type());

		Cmd::cmdNotifyCreateResult send_cmd;
		if((QWORD)room_id == 0)
		{
			send_cmd.set_isok(false);
			SendCmd(tempid, Cmd::enNotifyCreateResult, &send_cmd, send_cmd.ByteSize());
			stAssert(0);
			return true;
		}
		
		send_cmd.set_isok(true);
		send_cmd.set_type(recv_cmd.type());
		send_cmd.set_roomid((QWORD)room_id);
		send_cmd.set_reason(recv_cmd.reason());
		send_cmd.set_ip(Global::g_strServiceIP);
		send_cmd.set_port(room_id.order_id);

		SendCmd(tempid, Cmd::enNotifyCreateResult, &send_cmd, send_cmd.ByteSize());
	}

	/// 删除房间
	if(cmd_id == Cmd::enDelRoom)
	{
		//Cmd::cmdDelRoom recv_cmd;
		//recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
	}

	/// 删除一个用户
	if(cmd_id == Cmd::enRemoveRoomUser)
	{
		Cmd::cmdRemoveRoomUser recv_cmd;
		recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		stBaseRoom * p_room = stRoomManager::GetInstance().GetRoom(recv_cmd.roomid());
		if(!p_room)
		{
			Global::logger->warn("[remove_room_user] remove room user but not find room <roomid=%llu>", recv_cmd.roomid());
			stAssert(0);
			return true;
		}
		
		p_room->Put(pt_null_cmd, cmd_size);
		return true;
	}
	
    return true;
}


const bool stInfoClient::IsConnected()
{
	bool is_connect = false;
	is_connect = stRClientTask::IsConnected();
	return is_connect;
}


