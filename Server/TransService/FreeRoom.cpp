#include "FreeRoom.h"
#include "RoomManager.h"
#include "InfoClient.h"


stFreeRoom::~stFreeRoom()
{
   
}


stFreeRoom::stFreeRoom():stBaseRoom(Cmd::ROOM_TYPE_FREE)
{
 
}

	
void stFreeRoom::AddSendChannel(stFreeUser * p_free_user)
{
	Cmd::cmdNotifyTakeMic send_cmd;
	send_cmd.set_tempid(p_free_user->GetTempId());
	send_cmd.set_uniqueid("");
	send_cmd.set_mtime(0);
	stInfoClient::GetInstance().SendCmd(p_free_user->GetTempId(), Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());

	BroadCastAddListen(p_free_user);
	NotifyAllSendToMe(p_free_user);	
}

	
void stFreeRoom::AddRecvChannel(stFreeUser * p_src_user, const char * user_name, const char * dest_name)
{
	stFreeUser * p_user = (stFreeUser *)GetRTCPUser(user_name);
	if(!p_user)
	{
		Global::logger->warn("not find rtcp user addrecvchannel <%s,%s,%s>", p_src_user->GetCName(), user_name, dest_name);
		//stAssert(0);
		return;
	}

	p_user->AddBeListenChannel(p_src_user->GetCName());

	stFreeUser * p_dest_user = (stFreeUser *)GetRTCPUser(dest_name);
	if(p_dest_user)
	{
		p_src_user->SetDestName(dest_name);
		p_dest_user->AddListenChannel(p_src_user->GetCName());
		//Global::logger->debug("[new_listener] add a new lister channel <%s> listen <%s>",
		//					  p_src_user->GetCName(), p_dest_user->GetCName());
	}
	else
	{
		Global::logger->debug("not find dest user <%s,%s,%s>", p_src_user->GetCName(), user_name, dest_name);
		//stAssert(0);
	}
}



void stFreeRoom::RemoveRecvChannel(stFreeUser * p_src_user)
{
	if(m_stRTCPUse.size() == 0)
	{
		return;
	}
	
	defRTCPUserMapIter iter = m_stRTCPUse.begin();
	for(; iter != m_stRTCPUse.end(); iter++)
	{
		stFreeUser * p_free_user = (stFreeUser *)iter->second;
		if(p_free_user)
		{
			if(p_free_user->GetChannelType() == CHANNEL_TYPE_SEND && p_free_user->GetTempId() == p_src_user->GetTempId())
			{
				p_free_user->DelBeListenChannel(p_src_user->GetCName());
				break;
			}
		}
		else
		{
			stAssert(0);		
		}
	}	
}



void stFreeRoom::SendRTPData(defSSRCId ssrc_id, const char * recv_buf, const int recv_len)
{
	stFreeUser * p_free_user = NULL;
	defRTPUserMapIter iter = m_stRTPUse.find(ssrc_id);
	if(iter != m_stRTPUse.end())
	{
		stRTPUser * p_rtp_user = iter->second;
		if(p_rtp_user && p_rtp_user->IsInit() && !p_rtp_user->IsLeave() && !p_rtp_user->IsTerminate())
		{
			p_free_user = (stFreeUser *)GetRTCPUser(p_rtp_user->GetRTCPName());
		}
	}

	if(p_free_user)
	{
		if(p_free_user->IsInit() && ! p_free_user->IsLeave() && ! p_free_user->IsTerminate())
		{
			p_free_user->BroadCast(recv_buf, recv_len);
		}
	}
}


void stFreeRoom::BroadCastAddListen(stFreeUser * p_dest_user)
{
	if(m_stRTCPUse.size() == 0)
	{
		stAssert(0);
		return;
	}

	Global::logger->debug("[add_listen_begin] <<<<<<<<<<<<<<<notify <%s> add listeners begin ->>>>>>>>>", p_dest_user->GetCName());
	
	defRTCPUserMapIter iter = m_stRTCPUse.begin();
	for(; iter != m_stRTCPUse.end(); iter++)
	{
		stFreeUser * p_free_user = (stFreeUser *)iter->second;
		if(p_free_user)
		{
			if(p_free_user->GetChannelType() == CHANNEL_TYPE_SEND && p_free_user->IsInit() &&
			   ! p_free_user->IsLeave() && ! p_free_user->IsTerminate())
			{
				if(strcmp((iter->first).c_str(),p_dest_user->GetCName()) != 0)
				{
					p_free_user->GetFreeRecvChannel(p_dest_user);
				}
			}
		}
		else
		{
			stAssert(0);		
		}
	}

	Global::logger->debug("[add_listen_begin] <<<<<<<<<<<<<<<--notify <%s> add listeners end ->>>>>>>", p_dest_user->GetCName());
}



void stFreeRoom::NotifyAllSendToMe(stFreeUser * p_src_user)
{
	if(m_stRTCPUse.size() == 0)
	{
		stAssert(0);
		return;
	}

	Global::logger->debug("[add_listen] <<<<<<<notify me <%s> add all sender to belister begin->>>>>>>>", p_src_user->GetCName());
	
	Cmd::cmdNotifyAddVoiceUser send_me;
			
	defRTCPUserMapIter iter = m_stRTCPUse.begin();
	for(; iter != m_stRTCPUse.end(); iter++)
	{
		stFreeUser * p_free_user = (stFreeUser *)iter->second;
		if(p_free_user)
		{
			if(p_free_user->GetChannelType() == CHANNEL_TYPE_SEND && p_free_user->IsInit() &&
			   ! p_free_user->IsLeave() && ! p_free_user->IsTerminate())
			{
				if(strcmp(p_free_user->GetCName(), p_src_user->GetCName()) != 0)
				{
					p_src_user->GetFreeRecvChannel(p_free_user);
				}
			}
		}
	}

	Global::logger->debug("[add_listen] <<<<<<<notify me <%s> add all sender to belister end->>>>>>>>", p_src_user->GetCName());
}
