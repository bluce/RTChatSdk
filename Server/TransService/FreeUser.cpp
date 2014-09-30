#include "FreeUser.h"
#include "InfoClient.h"
#include "FreeRoom.h"

stFreeUser::stFreeUser(stBaseRoom * p_room, const char * c_name,
					   defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id):
	stRTCPUser(p_room, c_name,ssrc_id, _addr, temp_id, RTCPUSER_TYPE_FREE)
{
	m_enChannelType = CHANNEL_TYPE_MIN;
}


stFreeUser::~stFreeUser()
{
	
}


enChannelType stFreeUser::GetChannelType() const
{
	return m_enChannelType;
}


const char * stFreeUser::GetDestName()
{
	return m_strDestName.c_str();
}


void stFreeUser::SetDestName(const char * cname)
{
	m_strDestName = cname;
}



bool stFreeUser::Init()
{
	char c_type = GetCName()[0];

	stFreeRoom * p_free_room = (stFreeRoom *)m_pRoom;
	
	if(c_type == 's')
	{
		m_enChannelType = CHANNEL_TYPE_SEND;
		AddBeListenChannel(GetCName());
		p_free_room->AddSendChannel(this);
		
	}
	else if(c_type == 'r')
	{
		std::string ll = GetCName();
		std::string::size_type pos = ll.find_first_of('@');
		if(pos == std::string::npos)
		{
			stAssert(0);
		}

		std::string str_recv = ll.substr(1,pos-1);
		QWORD recv_id = strtoull(str_recv.c_str(), NULL, 10);
		std::stringstream send;
		std::stringstream recv;
		send<< "s" << GetTempId();
		recv<< "s" << recv_id;
		m_enChannelType = CHANNEL_TYPE_RECV;
		
		p_free_room->AddRecvChannel(this, send.str().c_str(), recv.str().c_str());
	}
	
	return true;
}


/**
 * @brief 增加一个收听的
 *
 */
void stFreeUser::AddListenChannel(const char * _channel)
{
	std::set<std::string >::iterator iter = m_stListeners.find(_channel);
	if(iter != m_stListeners.end())
	{
		Global::logger->debug("[add_lister] rtcp <%s> add lister <%s> have already exits",
							  GetCName(), _channel);
		stAssert(0);
		return;
	}

	if(m_stListeners.size() >= MAX_LISTEN_NUM)
	{
		Global::logger->debug("[add_lister] rtcp <%s> add lister <%s> but listeners full",
							  GetCName(), _channel);
		stAssert(0);
		return;
	}
	
	m_stListeners.insert(_channel);

	Global::logger->debug("[add_lister] rtcp <%s> add lister <%s>",
							  GetCName(), _channel);
}


/// 增加一个被收听通道 
void stFreeUser::AddBeListenChannel(const char * _channel)
{
	std::set<std::string >::iterator iter = m_stBeListen.find(_channel);
	if(iter != m_stBeListen.end())
	{
		Global::logger->debug("[add_belister] rtcp <%s> add belister <%s> have already exits",
							  GetCName(), _channel);
		stAssert(0);
		return;
	}

	if(m_stBeListen.size() >= MAX_LISTEN_NUM)
	{
		Global::logger->debug("[add_belister] rtcp <%s> add belister <%s> buf full",
							  GetCName(), _channel);
		stAssert(0);
		return;
	}
	
	m_stBeListen.insert(_channel);
	Global::logger->debug("[add_belister] rtcp <%s> add belister <%s>",
							  GetCName(), _channel);
}



/// 删除一个收听的人
void stFreeUser::DelListenChannel(const char * cname)
{
	if(m_stListeners.size() == 0)
	{
		Global::logger->debug("[del_lister] rtcp <%s> del lister channel <%s> but zero",
							  GetCName(), cname);
		stAssert(0);
		return;
	}

	std::set<std::string >::iterator iter = m_stListeners.find(cname);
	if(iter == m_stListeners.end())
	{
		Global::logger->debug("[del_lister] rtcp <%s> del lister <%s> but not find",
							  GetCName(), cname);
		stAssert(0);
		return;
	}
	
	m_stListeners.erase(iter);
}



/// 删除一个收听通道
void stFreeUser::DelBeListenChannel(const char * cname)
{
	if(m_stBeListen.size() == 0)
	{
		Global::logger->debug("[del_belister] rtcp <%s> del lister channel <%s> but zero",
							  GetCName(), cname);
		stAssert(0);
		return;
	}

	std::set<std::string >::iterator iter = m_stBeListen.find(cname);
	if(iter == m_stBeListen.end())
	{
		Global::logger->debug("[del_belister] rtcp <%s> del lister <%s> but not find",
							  GetCName(), cname);
		//stAssert(0);
		return;
	}
	
	m_stBeListen.erase(iter);
}



/**
 * @brief 从一个用户获取空闲的收听列表，没有就发送命令增加
 *
 */
void stFreeUser::GetFreeRecvChannel(stFreeUser * p_dest_user)
{
	if(m_stBeListen.size() == 0)
	{
		Global::logger->debug("[get_free] rtcp <%s> belisteners zero will lister <%s>",
							  GetCName(), p_dest_user->GetCName());
		stAssert(0);
		return;
	}

	bool ret_code = true;
	
	std::set<std::string >::iterator iter = m_stBeListen.begin();
	for(; iter != m_stBeListen.end(); iter++)
	{
		stFreeUser * p_free_user = (stFreeUser *)m_pRoom->GetRTCPUser((*iter).c_str());
		if(p_free_user)
		{
			if(strcmp(p_free_user->GetDestName(), "") == 0)
			{
				p_free_user->SetDestName(p_dest_user->GetCName());
				p_dest_user->AddListenChannel(p_free_user->GetCName());
				Global::logger->debug("[add_listener] assign <%s>  listen <%s>", p_free_user->GetCName(), p_dest_user->GetCName());
				ret_code = false;
				break;
			}
		}
		else
		{
			Global::logger->debug("not find rtcp user <%s>", (*iter).c_str());
			stAssert(0);
		}
	}

	if(ret_code)
	{
		if(m_stBeListen.size() >= MAX_LISTEN_NUM)
		{
			stAssert(0);
			return;
		}

		Global::logger->debug("[add_listener] send cmd notify <tempid=%llu> add recv channel lister <%llu>",
							  GetTempId(),
							  p_dest_user->GetTempId());
		
		Cmd::cmdNotifyAddVoiceUser send_cmd;
		Cmd::stVoiceUserInfo * p_info = send_cmd.add_info();
		p_info->set_id(p_dest_user->GetTempId());
		stInfoClient::GetInstance().SendCmd(GetTempId(), Cmd::enNotifyAddVoiceUser,
											&send_cmd, send_cmd.ByteSize());
	}
}


/// 删除自己
void stFreeUser::RemoveMe()
{
	if(IsLeave())
	{
		return;
	}
	
	stFreeUser * p_free_user = (stFreeUser *)(m_pRoom->GetRTCPUser(m_strDestName.c_str()));
	if(p_free_user)
	{
		p_free_user->DelListenChannel(GetCName());
		Global::logger->debug("[del_listen] delete listen <%s> from dest user <%s>",
							  GetCName(), p_free_user->GetCName());
	}
		
	if(GetChannelType() == CHANNEL_TYPE_SEND)
	{
		if(m_stListeners.size() == 0)
		{
			return;
		}

		
	
		std::set<std::string >::iterator iter =  m_stListeners.begin();
		for(; iter != m_stListeners.end(); iter++)
		{
			stFreeUser * p_free_user = (stFreeUser *)(m_pRoom->GetRTCPUser((*iter).c_str()));
			if(p_free_user && !p_free_user->IsTerminate() && !p_free_user->IsLeave())
			{
				p_free_user->SetDestName("");
				Global::logger->debug("[del_lister] notify <%s> clear dest name because dest<%s> remove",
									  p_free_user->GetCName(), GetCName());
			}
			else
			{
				Global::logger->debug("[del_lister] notify <%s> clear dest name because dest<%s> remove but state error",
									  p_free_user->GetCName(), GetCName());
			}
		}
	
		m_stListeners.clear();
	}

	if(GetChannelType() == CHANNEL_TYPE_RECV)
	{
		((stFreeRoom * )m_pRoom)->RemoveRecvChannel(this);
	}

	
}


void stFreeUser::Leave()
{
	RemoveMe();
	stRTCPUser::Leave();
}


void stFreeUser::Terminate()
{
	Leave();
	stRTCPUser::Terminate();
}



void stFreeUser::BroadCast(const char * send_buf, const unsigned int buf_len)
{
	if(m_stListeners.size() == 0)
	{
		return;
	}

	//Global::logger->debug("[begin]->>>>>>>>>>>>>>>>>>>>trans data begin <tempid=%llu,size=%u,ip=%s,port=%u>", taskid, buf_len,
	//				  inet_ntoa(addr.sin_addr),
	//					  ntohs(addr.sin_port));
	
	std::set<std::string>::iterator iter,next;
	next = m_stListeners.begin();
	for(iter = next; iter != m_stListeners.end(); iter = next)
	{
		if(next != m_stListeners.end())
		{
			next++;
		}
		
		stFreeUser * p_user = (stFreeUser *)(m_pRoom->GetRTCPUser((*iter).c_str()));
		if(p_user)
		{
			p_user->SendData(send_buf, buf_len);
		}
		else
		{
			Global::logger->debug("[assert] send rtp data but not find user <%s>", (*iter).c_str());
			stAssert(0);
			m_stListeners.erase(iter);
		}
	}

	//Global::logger->debug("[end]->>>>>>>>>>>>>>>>>>>>trans data end <tempid=%llu,size=%u,ip=%s,port=%u>", taskid, buf_len,
	//					  inet_ntoa(addr.sin_addr),
	//					  ntohs(addr.sin_port));
}
