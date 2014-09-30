#include "RTPUser.h"
#include "BaseRoom.h"

stRTPUser::stRTPUser(stBaseRoom * p_room, defSSRCId ssrc_id, struct sockaddr_in & _addr)
{
	ssrcid = ssrc_id;
	m_blInit = false;
	addr = _addr;
	m_pRoom = p_room;
	m_blTerminate = false;
	m_blIsLeave = false;
}


stRTPUser::~stRTPUser()
{
	
}


defSSRCId stRTPUser::GetSSRCId() const
{
	return ssrcid;
}


bool stRTPUser::IsInit()
{
	return m_blInit;
}


void stRTPUser::Terminate()
{
	m_blTerminate = true;
}


bool stRTPUser::IsTerminate()
{
	return m_blTerminate;
}


void stRTPUser::Leave()
{
	m_blIsLeave = true;
}


bool stRTPUser::IsLeave()
{
	return m_blIsLeave;
}


void stRTPUser::SetRTCPName(const char * c_name)
{
	Global::logger->debug("set rtp <ssrcid=%u> to rtcp <%s>",GetSSRCId(), c_name);
	m_strRTCPName = c_name;
	m_blInit = true;
}


const char * stRTPUser::GetRTCPName()
{
	return m_strRTCPName.c_str();
}


void stRTPUser::SetAddr(const sockaddr_in & _addr)
{
	addr = _addr;
}


void stRTPUser::SendData(const char * msg, const unsigned int msg_len)
{
	if(IsInit())
	{
		if(!IsTerminate())
		{
			if(!IsLeave())
			{
				sendto(m_pRoom->GetSockData(), msg, msg_len, 0, (struct sockaddr *)&(addr), sizeof(addr));
			}
		}
		else
		{
			Global::logger->debug("[send_rtp_data] but connect terminate <ssrcid=%u>", GetSSRCId());
			stAssert(0);
		}
	}
	else
	{
		Global::logger->debug("[send_rtp_data] but connect  not init <ssrcid=%u>", GetSSRCId());
		stAssert(0);
	}
}
