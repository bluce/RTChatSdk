#include "RTCPUser.h"
#include "RTPUser.h"
#include "BaseRoom.h"


stRTCPUser::stRTCPUser(stBaseRoom * p_room, const char * c_name,
			   defSSRCId ssrc_id, struct sockaddr_in & _addr, const QWORD temp_id, enRTCPUserType _type)

{
	tempid = temp_id;
	ssrcid = ssrc_id;
	addr = _addr;
	m_pRoom = p_room;
	m_blTerminate = false;
	m_blIsLeave = false;
	m_blPing = true;
	rtpuserid = 0;
	m_strCName = c_name;
	m_enUserType = _type;
}


stRTCPUser::~stRTCPUser()
{
	
}


void stRTCPUser::SetAddr(const sockaddr_in & _addr)
{
	addr = _addr;
}


const char * stRTCPUser::GetCName()
{
	return m_strCName.c_str();
}


bool stRTCPUser::Init()
{
	return true;
}


defSSRCId stRTCPUser::GetRTPUserId() const
{
	return rtpuserid;
}

	
enRTCPUserType stRTCPUser::GetUserType() const
{
	return m_enUserType;
}


void stRTCPUser::SetSSRCId(const defSSRCId ssrc_id)
{
	ssrcid = ssrc_id;
}


defSSRCId stRTCPUser::GetSSRCId() const
{
	return ssrcid;
}

void stRTCPUser::SetTempId(const defTempId temp_id)
{
	tempid = temp_id;
}


defTempId stRTCPUser::GetTempId() const
{
	return tempid;
}


bool stRTCPUser::IsInit()
{
	if(rtpuserid != 0)
	{
		return true;
	}
	
	return false;
}


void stRTCPUser::Leave()
{
	m_blIsLeave = true;
	
	stRTPUser * p_user = m_pRoom->GetRTPUser(rtpuserid);
	if(p_user)
	{
		Global::logger->debug("[rtp_user_leave_flag] rtcp <%s> leave so rtp also leave <ssrcid=%u>",
							  GetCName(),
							  rtpuserid);
		p_user->Leave();
	}
}


bool stRTCPUser::IsLeave()
{
	return m_blIsLeave;
}


void stRTCPUser::Terminate()
{
	m_blTerminate = true;
	stRTPUser * p_user = m_pRoom->GetRTPUser(rtpuserid);
	if(p_user)
	{
		Global::logger->debug("[rtp_user_terminate] rtcp <%s> terminate so rtp also terminate <ssrcid=%u>",
							  GetCName(),
							  rtpuserid);
		p_user->Terminate();
	}
}


bool stRTCPUser::IsTerminate()
{
	return m_blTerminate;
}


void stRTCPUser::SetRTPUserId(defSSRCId _id)
{
	Global::logger->debug("[init_rtcp_user] a rtcp <%s> set rtp <rtpid=%u>", GetCName(),_id);
	rtpuserid = _id;
}


void stRTCPUser::ClearPing()
{
	m_blPing = true;
}


bool stRTCPUser::Update(QWORD cur_time)
{
	if(m_blPing)
	{
		m_blPing = false;
		ping_time = cur_time + 30 * 1000;
		return true;
	}
	else
	{
		if(ping_time < cur_time)
		{
			return false;
		}
	}
	
	return true;
}


void stRTCPUser::StopSendActive()
{
	defSSRCId ssrc_id = GetSSRCId();
	char msg[100];
	memset(msg, 0, sizeof(msg));
	*(BYTE *)(&msg[0]) = 0x80;
	*(BYTE *)(&msg[1]) = 0xcc;
	WORD * p1 = (WORD *)(&msg[2]);
	*p1 = htons(0x06);
	DWORD * p2 = (DWORD *)(&msg[4]);
	*p2 = htonl(ssrc_id);
	msg[8] = 's';
	msg[9] = 't';
	msg[10] = 'o';
	msg[11] = 'p';
	DWORD * p3 = (DWORD *)(&msg[12]);
	*p3 = htonl(ssrc_id);
	DWORD * p4 = (DWORD *)(&msg[16]);
	*p4 = htonl(ssrc_id);
	DWORD * p5 = (DWORD *)(&msg[20]);
	*p5 = htonl(ssrc_id);
	DWORD * p6 = (DWORD *)(&msg[24]);
	*p6 = htonl(0);
	sendto(m_pRoom->GetSockCtrl(),  msg, 32, 0, (struct sockaddr *)&(addr), sizeof(addr));
}


void stRTCPUser::SendData(const char * msg, const unsigned int msg_len)
{
	if(IsInit() && !IsLeave() && !IsTerminate())
	{
		stRTPUser * p_user = m_pRoom->GetRTPUser(rtpuserid);
		if(p_user)
		{
			p_user->SendData(msg, msg_len);
		}
		else
		{
			stAssert(0);
		}
	}
}



void stRTCPUser::SendPingCmd()
{
	if(IsInit() && !IsLeave() && !IsTerminate())
	{
		stRTPUser * p_user = m_pRoom->GetRTPUser(rtpuserid);
		if(p_user)
		{
			char ss = 'p';
			p_user->SendData(&ss,1);
		}
		else
		{
			stAssert(0);
		}
	}
}

