#include "AssertEx.h"
#include "Global.h"
#include "RTPSession.h"
#include "BaseRoom.h"
#include "Command.h"
#include <fcntl.h>

stRTPSession::stRTPSession()
{
	m_stSockData = -1;
	m_stSockCtrl = -1;
	m_pRoom = NULL;
	memset(m_stRecvBuf, 0, sizeof(m_stRecvBuf));
	m_iRecvCount = 0;
}

stRTPSession::~stRTPSession()
{
	
}


int stRTPSession::GetSockData()
{
	return m_stSockData;
}


int stRTPSession::GetSockCtrl()
{
	return m_stSockCtrl;
}


Cmd::stTaskId stRTPSession::GenTaskId(struct sockaddr_in & addr)
{
	Cmd::stTaskId task_id;
	task_id.server_id = ntohl(addr.sin_addr.s_addr);
	task_id.order_id = ntohs(addr.sin_port);
	return (QWORD)task_id;
}


bool stRTPSession::Init(stBaseRoom * p_room, WORD port)
{
	m_pRoom = p_room;
	
	if(m_stSockData != -1 || m_stSockCtrl != -1)
	{
		stAssert(0);
		return false;
	}

	
	WORD data_port = port;
	WORD ctrl_port = data_port + 1;
	
	m_stSockData = -1;
	struct sockaddr_in myaddr;
	
	if((m_stSockData = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		stAssert(0);
		return false;
	}

	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(data_port);
	myaddr.sin_addr.s_addr = inet_addr(Global::g_strServiceIP.c_str());

	if(bind(m_stSockData, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1)
	{
		stAssert(0);
		return false;
	}

	m_stSockCtrl = -1;
	struct sockaddr_in ctrl_addr;
	
	if((m_stSockCtrl = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		stAssert(0);
		return false;
	}

	memset(&ctrl_addr, 0, sizeof(ctrl_addr));
	ctrl_addr.sin_family = AF_INET;
	ctrl_addr.sin_port = htons(ctrl_port);
	ctrl_addr.sin_addr.s_addr = inet_addr(Global::g_strServiceIP.c_str());

	if(bind(m_stSockCtrl, (struct sockaddr *)&ctrl_addr, sizeof(ctrl_addr)) == -1)
	{
		stAssert(0);
		return false;
	}

	int fsflags = fcntl(m_stSockData,F_GETFL,0);
    fsflags |= O_NONBLOCK;
    fcntl(m_stSockData,F_SETFL,fsflags);

    fsflags = fcntl(m_stSockCtrl,F_GETFL,0);
    fsflags |= O_NONBLOCK;
    fcntl(m_stSockCtrl,F_SETFL,fsflags);
	

	Global::logger->debug("[room_rtpsession] init rtp session successful <roomid=%llu,dateport=%u,ctrlport=%u>",
						  m_pRoom->GetRoomId(), data_port, ctrl_port);
	return true;
	
}



void stRTPSession::Final()
{
	if(m_stSockData != -1)
	{
		::shutdown(m_stSockData, SHUT_RDWR);
		TEMP_FAILURE_RETRY(::close(m_stSockData));
		m_stSockData = -1;			
	}


	if(m_stSockCtrl != -1)
	{
		::shutdown(m_stSockCtrl, SHUT_RDWR);
		TEMP_FAILURE_RETRY(::close(m_stSockCtrl));
		m_stSockCtrl = -1;			
	}
}



void stRTPSession::OnRTPData()
{
	struct sockaddr_in peeraddr;
	socklen_t len;
	memset(m_stRecvBuf, 0, sizeof(m_stRecvBuf));
	memset(&peeraddr, 0, sizeof(peeraddr));
	len = sizeof(peeraddr);
	m_iRecvCount = recvfrom(m_stSockData,&m_stRecvBuf,sizeof(m_stRecvBuf), 0,(struct sockaddr *)&peeraddr, &len);
	if(m_iRecvCount > 12)
	{
		defSSRCId * p_temp = (defSSRCId *)((const char *)(&m_stRecvBuf[8]));
		defSSRCId ssrc_id = ntohl(*p_temp);
		Cmd::stTaskId task_id = GenTaskId(peeraddr);
		//Global::logger->debug("->>>>>>>>>>>>>>>>>>recv a rtp data <taskid=%llu,ssrcid=%u>", (QWORD)task_id, ssrc_id);
		if(task_id.IsVaild())
		{
			if(m_pRoom->IsRegisterRTP(ssrc_id, peeraddr))
			{
				m_pRoom->SendRTPData(ssrc_id, m_stRecvBuf, m_iRecvCount);	
			}
			else
			{
				m_pRoom->AddRTPUser(ssrc_id, peeraddr);
			}
		}
	}
	else
	{
		//stAssert(0);
	}
}


void stRTPSession::OnRTCPData()
{
	struct sockaddr_in peeraddr;
	socklen_t len;
	memset(m_stRecvBuf, 0, sizeof(m_stRecvBuf));
	memset(&peeraddr, 0, sizeof(peeraddr));
	len = sizeof(peeraddr);
	m_iRecvCount = recvfrom(m_stSockCtrl,&m_stRecvBuf,sizeof(m_stRecvBuf), 0,(struct sockaddr *)&peeraddr, &len);
	if(m_iRecvCount > 8)
	{
		defSSRCId ssrc_id = 0;
		WORD pack_len = 0;
		do
		{
			BYTE pack_type = *(BYTE *)(&m_stRecvBuf[pack_len + 1]);
			defSSRCId * p_temp = (defSSRCId *)((const char *)(&m_stRecvBuf[pack_len + 4]));
			ssrc_id = ntohl(*p_temp);
			WORD * pp_temp = (WORD *)((const char *)(&m_stRecvBuf[pack_len + 2]));
			WORD one_pack_len = ntohs(*pp_temp);
			if(pack_len + (one_pack_len + 1 ) * 4 > m_iRecvCount)
			{
				stAssert(0);
				return;
			}
			
			if(pack_type == 200) // SR
			{
				
			}
			else if(pack_type == 201) // RR
			{
				
			}
			else if(pack_type == 202) //SDES
			{
				BYTE sub_type = *(BYTE *)(&m_stRecvBuf[pack_len + 8]);
				BYTE sub_type_len = *(BYTE *)(&m_stRecvBuf[pack_len + 9]);
				if(sub_type == 0x01) // CNAME
				{
					if(sub_type_len > 59)
					{
						stAssert(0);
						return;
					}
					
					char cname[sub_type_len + 1];
					memset(cname, 0, sizeof(cname));
					memcpy(cname, (const void *)(&m_stRecvBuf[pack_len + 10]), sub_type_len);
					Cmd::stTaskId task_id = GenTaskId(peeraddr);
					if(task_id.IsVaild())
					{
						//Global::logger->debug("[rtcp_data] recv a rtcp data <ssrcid=%u,ip=%s,port=%u>",
						//					  ssrc_id, inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
						if(m_pRoom->IsRegisterRTCP(cname, ssrc_id, peeraddr))
						{
							m_pRoom->ClearPing(cname);
						}
						else
						{
							m_pRoom->AddRTCPUser(cname, sub_type_len, ssrc_id, peeraddr);
						}
					}
				}
			}
			else if(pack_type == 203) // BYE
			{
				
			}

			pack_len = pack_len +  (one_pack_len + 1) * 4;
		}while(pack_len < m_iRecvCount);
	}
}
