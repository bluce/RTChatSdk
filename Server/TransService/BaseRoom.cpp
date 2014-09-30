#include <string.h>
#include "AssertEx.h"
#include "InfoClient.h"
#include "BaseRoom.h"
#include "RoomManager.h"
#include "TwoUser.h"
#include "OnlyUser.h"
#include "FreeUser.h"
#include "QueueUser.h"


stBaseRoom::stBaseRoom(Cmd::enRoomType _type)
{
	roomtype = _type;	
}


stBaseRoom::~stBaseRoom()
{
	
}


Cmd::enRoomType stBaseRoom::GetRoomType() const
{
	return roomtype;
}


const defRoomId stBaseRoom::GetRoomId()
{
	return m_dwRoomId;
}


int stBaseRoom::GetSockData()
{
	return m_stRTPSession.GetSockData();
}


int stBaseRoom::GetSockCtrl()
{
	return m_stRTPSession.GetSockCtrl();
}


stRTPUser * stBaseRoom::GetRTPUser(defSSRCId ssrc_id)
{
	stRTPUser * p_user = NULL;
	defRTPUserMapIter iter = m_stRTPUse.find(ssrc_id);
	if(iter != m_stRTPUse.end())
	{
		p_user = iter->second;
	}
	
	return p_user;
}


stRTCPUser * stBaseRoom::GetRTCPUser(const char * c_name)
{
	stRTCPUser * p_user = NULL;
	defRTCPUserMapIter iter = m_stRTCPUse.find(c_name);
	if(iter != m_stRTCPUse.end())
	{
		p_user = iter->second;
	}
	
	return p_user;
}


bool stBaseRoom::Init(Cmd::stTaskId & task_id)
{
	m_dwRoomId = (QWORD)task_id;

	if(! m_stRTPSession.Init(this, task_id.order_id))
	{
		return false;
	}

	m_iEpollDatafd = ::epoll_create(63);
	if(m_iEpollDatafd == -1)
  	{
		return false;
	}

	m_iEpollCtrlfd = ::epoll_create(63);
	if(m_iEpollCtrlfd == -1)
  	{
		return false;
	}

	m_EpollDataEventVec.resize(121);

	m_EpollCtrlEventVec.resize(121);

	__uint32_t data_events = EPOLLIN;
	
	__uint32_t ctrl_events = EPOLLIN | EPOLLERR | EPOLLPRI;
	
	AddDataEpoll(m_iEpollDatafd, data_events, (void *)this);
	
	AddCtrlEpoll(m_iEpollCtrlfd, ctrl_events, (void *)this);

	Start();

	Global::logger->debug("init room successful <roomid=%llu>", m_dwRoomId);
	
	return true;
}


void stBaseRoom::AddDataEpoll(int epoll_fd, uint32_t evts, void * p_data)
{
	struct epoll_event event;
	
	event.events = evts;
	event.data.ptr = p_data;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_stRTPSession.GetSockData(), &event) == -1)
	{
		Global::logger->fatal("[base_socket_addepoll] add epoll failed");
	}

	Global::logger->debug("add data epoll successful");
}


void stBaseRoom::AddCtrlEpoll(int epoll_fd, uint32_t evts, void * p_data)
{
	struct epoll_event event;
	
	event.events = evts;
	event.data.ptr = p_data;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_stRTPSession.GetSockCtrl(), &event) == -1)
	{
		Global::logger->fatal("[base_socket_addepoll] add epoll failed");
	}

	Global::logger->debug("add ctrl epoll successful");
}


void stBaseRoom::OnRecvRTCPData()
{
	m_stRTPSession.OnRTCPData();
}


void stBaseRoom::OnRecvRTPData()
{
	m_stRTPSession.OnRTPData();
}


bool stBaseRoom::IsRegisterRTP(defSSRCId ssrc_id, struct sockaddr_in & _addr)
{
	defRTPUserMapIter iter = m_stRTPUse.find(ssrc_id);
	if(iter != m_stRTPUse.end())
	{
		stRTPUser * p_user = iter->second;
		p_user->SetAddr(_addr);
		
		if(!p_user)
		{
			stAssert(0);
			return false;
		}
		
		if(!p_user->IsInit())
		{
			return false;
		}
		
		if(p_user->IsLeave() || p_user->IsTerminate())
		{
			return false;
		}

		return true;
	}
	
	return false;
}


void stBaseRoom::FindInitRTCP(stRTPUser * p_rtp_user)
{
	defSSRCId _id = p_rtp_user->GetSSRCId();
	
	defRTCPUserMapIter iter = m_stRTCPUse.begin();
	for(; iter != m_stRTCPUse.end(); iter++)
	{
		stRTCPUser * p_user = iter->second;
		if(p_user && p_user->GetSSRCId() == _id)
		{
			Global::logger->debug("FindInitRTCP set rtcp<%s> to rtp<ssrcid=%u>",
								  p_user->GetCName(), _id);
			p_user->SetRTPUserId(_id);
			p_rtp_user->SetRTCPName(p_user->GetCName());
		}
	}
}


bool stBaseRoom::IsRegisterRTCP(const char * c_name, defSSRCId ssrc_id, struct sockaddr_in & addr)
{
	defRTCPUserMapIter iter = m_stRTCPUse.find(c_name);
	if(iter != m_stRTCPUse.end())
	{
		stRTCPUser * p_user = iter->second;
		if(! p_user)
		{
			stAssert(0);
			return false;
		}

		if(p_user->IsLeave() || p_user->IsTerminate())
		{
			return false;
		}

		defSSRCId old_id = p_user->GetSSRCId();
		if(ssrc_id != old_id)
		{
			stRTPUser * p_c_user = GetRTPUser(ssrc_id);
			if(p_c_user)
			{
				RemoveRTPUser(old_id);
				p_user->SetSSRCId(ssrc_id);
				p_user->SetRTPUserId(ssrc_id);
				p_c_user->SetRTCPName(p_user->GetCName());
				Global::logger->debug("[init_rtcp_user] a rtcp user <%s> change rtp <old=%u,new=%u>",
									  p_user->GetCName(), old_id, ssrc_id);
			}
		}

		p_user->SetAddr(addr);
				
		return true;
	}
	
	return false;
}


void stBaseRoom::AddRTPUser(defSSRCId ssrc_id, struct sockaddr_in & addr)
{
	defRTPUserMapIter iter = m_stRTPUse.find(ssrc_id);
	if(iter != m_stRTPUse.end())
	{
		stRTPUser * p_user = iter->second;
		if(p_user)
		{
			if(p_user->IsInit() == false)
			{
				if(p_user->IsLeave() == false)
				{
					if(p_user->IsTerminate() == false)
					{
						Global::logger->debug("init rtp user <%u,%d,%d,%d,%x>", p_user->GetSSRCId(), (BYTE)(p_user->IsInit()),
											  (BYTE)(p_user->IsLeave()), (BYTE)(p_user->IsTerminate()), p_user);
						
						if(p_user->IsInit() == true)
						{
							Global::logger->debug("why->>>>>>>>>>>>>>>>>>>>>>>>>%u", p_user->GetSSRCId());
							stAssert(0);
						}

						if(p_user->IsLeave() == true)
						{
							Global::logger->debug("why->>>>>>>>>>>>>>>>>>>>>>>>>%u", p_user->GetSSRCId());
							stAssert(0);
						}
						
						FindInitRTCP(p_user);
					}
				}
			}
		}
		
		return;
	}

	
	stRTPUser * p_new_user = (new stRTPUser(this, ssrc_id, addr));
	m_stRTPUse.insert(defRTPUserMapValue(ssrc_id, p_new_user));
	FindInitRTCP(p_new_user);
	
	Global::logger->debug("[new_rtp] a new rtp connect <roomid=%llu,ssrc_id=%u,ip=%s,port=%u>",
						  GetRoomId(), ssrc_id, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}


void stBaseRoom::InitRTPUser(stRTCPUser * p_rtcp_user)
{
	defRTPUserMapIter iter = m_stRTPUse.find(p_rtcp_user->GetSSRCId());
	if(iter != m_stRTPUse.end())
	{
		stRTPUser * p_rtp_user = iter->second;
		if(!p_rtp_user)
		{
			stAssert(0);
			return;
		}
		p_rtcp_user->SetRTPUserId(p_rtp_user->GetSSRCId());
		p_rtp_user->SetRTCPName(p_rtcp_user->GetCName());
	}
	else
	{
		//Global::logger->debug("[init_rtp_user] a rtcp user <%s> init rtp user but not find <ssrcid=%u>",
		//					  p_rtcp_user->GetCName(),
		//					  p_rtcp_user->GetSSRCId());
	}
}



void stBaseRoom::AddRTCPUser(const char * cname, const WORD sub_type_len, const defSSRCId ssrc_id, struct sockaddr_in & _addr)
{
	defRTCPUserMapIter iter = m_stRTCPUse.find(cname);
	if(iter != m_stRTCPUse.end())
	{
		stRTCPUser * p_user = iter->second;
		if(p_user->IsLeave())
		{
			if(strcmp(cname, p_user->GetCName()) == 0)
			{
				Global::logger->debug("[add_rtcp_user] add will leave rtcp, no care <cname=%s>",iter->second->GetCName());
				return;
			}
		}
	}

	stRTCPUser * p_new_user = NewRTCPUser(this, cname, ssrc_id, _addr);
	m_stRTCPUse.insert(defRTCPUserMapValue(cname, p_new_user));
	if(p_new_user)
	{
		p_new_user->Init();
		InitRTPUser(p_new_user);
	}
	else
	{
		stAssert(0);
	}
}



void stBaseRoom::ClearPing(const char * c_name)
{
	defRTCPUserMapIter iter = m_stRTCPUse.find(c_name);
	if(iter == m_stRTCPUse.end())
	{
		stAssert(0);
		return;
	}
	stRTCPUser * p_user = iter->second;
	if(1)//p_user->IsInit())
	{
		p_user->ClearPing();
	}
	else
	{
		Global::logger->debug("[rtcp_user] not init but clear ping <cname=%s>",p_user->GetCName());
	}
}


void stBaseRoom::SetLeave(QWORD temp_id)
{
	defRTCPUserMapIter c_iter, c_next;
	c_next= m_stRTCPUse.begin();
	for(c_iter = c_next; c_iter != m_stRTCPUse.end(); c_iter = c_next)
	{
		if(c_next != m_stRTCPUse.end())
		{
			c_next++;
		}
		
		stRTCPUser * p_user = c_iter->second;
		if(p_user && p_user->GetTempId() == temp_id)
		{
			Global::logger->debug("[leave_flag] infoclient notify a rtcp leave from room <roomid=%llu,cname=%s,tempid=%llu>",
								  m_dwRoomId, (c_iter->first).c_str(), temp_id);
			p_user->Leave();
		}
	}
}


bool stBaseRoom::CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size)
{
	const Cmd::stInternalCmd * p_info_cmd = (const Cmd::stInternalCmd *)pt_null_cmd;
	const Cmd::stProtoBufCmd * p_recv_cmd = (const Cmd::stProtoBufCmd *)(p_info_cmd->data);
	defCmdId cmd_id = p_recv_cmd->cmdid;
	
	Global::logger->debug("[room_cmd] room_cmd recv a command <id=%u,size=%u,proto_size=%u>",
						  cmd_id,cmd_size, p_recv_cmd->cmdlen);

	Cmd::cmdRemoveRoomUser recv_cmd;
	recv_cmd.google::protobuf::MessageLite::ParseFromArray(p_recv_cmd->data, p_recv_cmd->cmdlen);
		
	if(cmd_id == Cmd::enRemoveRoomUser)
	{
		SetLeave(recv_cmd.tempid());
		return true;
	}
	
	return true;
}


void stBaseRoom::RemoveRTPUser(defSSRCId ssrc_id)
{
	defRTPUserMapIter iter = m_stRTPUse.find(ssrc_id);
	if(iter != m_stRTPUse.end())
	{
		iter->second->Terminate();
	}
}


void stBaseRoom::Run()
{
	QWORD nomem_timer = 0;
	DWORD nomem_count = 0;

	QWORD delay_one_sec = 0;
	QWORD cur_time = 0;
	stRTime m_stRTime;
	
	while(! IsTTerminate())
	{
		m_stRTime.GetNow();
		cur_time = m_stRTime.GetMSec();
		::usleep(200);

		Get(1);

		defRTPUserMapIter iter,next;
		next= m_stRTPUse.begin();
		for(iter = next; iter != m_stRTPUse.end(); iter = next)
		{
			if(next != m_stRTPUse.end())
			{
				next++;
			}
		
			stRTPUser * p_user = iter->second;
			if(p_user && p_user->IsTerminate())
			{
				Global::logger->debug("[room_delete_user] a rtp user delete from room <roomid=%llu,ssrcid=%llu>",
								  m_dwRoomId, iter->first);
				
				delete p_user;
				p_user = NULL;
				m_stRTPUse.erase(iter);
			}
		}


		defRTCPUserMapIter c_iter, c_next;
		c_next= m_stRTCPUse.begin();
		for(c_iter = c_next; c_iter != m_stRTCPUse.end(); c_iter = c_next)
		{
			if(c_next != m_stRTCPUse.end())
			{
				c_next++;
			}
		
			stRTCPUser * p_user = c_iter->second;
			if(p_user && p_user->IsTerminate())
			{
				Global::logger->debug("[room_delete_user] a rtcp user delete from room <roomid=%llu,cname=%s>",
									  m_dwRoomId, (c_iter->first).c_str());
				delete p_user;
				p_user = NULL;
				m_stRTCPUse.erase(c_iter);
			}
		}

		int ret_data_code = epoll_wait(m_iEpollDatafd, &m_EpollDataEventVec[0], 120, 0);
		if(ret_data_code)
		{
			for(int i = 0; i < ret_data_code; i++)
			{
				if(m_EpollDataEventVec[i].events & EPOLLIN)
				{
					OnRecvRTPData();
				}
				
				m_EpollDataEventVec[i].events = 0;
			}
		}


		int ret_ctrl_code = epoll_wait(m_iEpollCtrlfd, &m_EpollCtrlEventVec[0], 120, 0);
		if(ret_ctrl_code)
		{
			for(int i = 0; i < ret_ctrl_code; i++)
			{
				if(m_EpollCtrlEventVec[i].events & EPOLLIN)
				{
					OnRecvRTCPData();
				}
				
				m_EpollCtrlEventVec[i].events = 0;
			}
		}
			
		if(m_stRTCPUse.size() > 0)
		{
			defRTCPUserMapIter c_iter, c_next;
			c_next= m_stRTCPUse.begin();
			for(c_iter = c_next; c_iter != m_stRTCPUse.end(); c_iter = c_next)
			{
				if(c_next != m_stRTCPUse.end())
				{
					c_next++;
				}
		
				stRTCPUser * p_user = c_iter->second;
				if(p_user && !(p_user->Update(m_stRTime.GetMSec())))
				{
					Global::logger->debug("[check_ping] a rtcp user ping failed <cname=%s>", p_user->GetCName());
					p_user->Terminate();
				}
			}


			if(delay_one_sec < cur_time)
			{
				delay_one_sec = cur_time + 1000 * 10;
				if(m_stRTCPUse.size() > 0)
				{
					c_iter = m_stRTCPUse.begin();
					for(; c_iter != m_stRTCPUse.end(); c_iter++)
					{
						stRTCPUser * p_user = c_iter->second;
						if(p_user)
						{
							p_user->SendPingCmd();
						}
					}
				}
			}
		}
		else
		{
			if(nomem_timer < cur_time)
			{
				nomem_count++;
				nomem_timer = cur_time + 10 * 1000;
				if(nomem_count > 6)
				{
					TTerminate();
				}
			}
		}
	}
	
	Final();
}


void stBaseRoom::Final()
{
	Cmd::cmdDelRoom send_cmd;
	send_cmd.set_roomid(m_dwRoomId);
	stInfoClient::GetInstance().SendCmd(0, (Cmd::enCmdId)(Cmd::enDelRoom), &send_cmd, send_cmd.ByteSize());
	Global::logger->info("[remove_room] del room notify infoservice <roomid=%llu>", m_dwRoomId);
	stRoomManager::GetInstance().RemoveRoom(this);
}


stRTCPUser * stBaseRoom::NewRTCPUser(stBaseRoom * p_room, const char * c_name, defSSRCId ssrc_id, struct sockaddr_in & _addr)
{
	defTempId temp_id = 0;
	char c_type = c_name[0];
	
	if(c_type == 's')
	{
		temp_id = strtoull((const char *)(&c_name[1]), NULL, 10);
	}
	else if(c_type == 'r')
	{
		const char * temp_char = strstr(c_name, "@s");
		if(temp_char != NULL)
		{
			temp_id = strtoull((const char *)(temp_char+2), NULL, 10);
		}
	}
	
	if(temp_id == 0)
	{
		Global::logger->warn("[get_temp_id] error cname<%s>", c_name);
		stAssert(0);
		return NULL;
	}
	
	if(p_room->GetRoomType() == Cmd::ROOM_TYPE_TWO)
	{
		return (stRTCPUser *)(new stTwoUser(p_room, c_name, ssrc_id, _addr, temp_id));
	}
	else if(p_room->GetRoomType() == Cmd::ROOM_TYPE_QUEUE)
	{
		return (stRTCPUser *)(new stQueueUser(p_room, c_name, ssrc_id, _addr, temp_id));
	}
	else if(p_room->GetRoomType() == Cmd::ROOM_TYPE_FREE)
	{
		return (stRTCPUser *)(new stFreeUser(p_room, c_name, ssrc_id, _addr, temp_id));
	}
	else if(p_room->GetRoomType() == Cmd::ROOM_TYPE_ONLY)
	{
		return (stRTCPUser *)(new stOnlyUser(p_room, c_name, ssrc_id, _addr, temp_id));
	}
	else
	{
		return NULL;
	}
}
