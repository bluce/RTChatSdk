#include "QueueRoom.h"


stQueueRoom::stQueueRoom(): stBaseRoom(Cmd::ROOM_TYPE_QUEUE)
{
	
}


stQueueRoom::~stQueueRoom()
{
	
}


void stQueueRoom::SendRTPData(defSSRCId ssrc_id, const char * recv_buf, const int recv_len)
{
	defRTPUserMapIter rtp_iter = m_stRTPUse.find(ssrc_id);
	if(rtp_iter == m_stRTPUse.end())
	{
		stAssert(0);
		return;
	}

	stRTPUser * p_rtp_user = rtp_iter->second;
	if(!p_rtp_user)
	{
		stAssert(0);
		return;
	}

	if(! p_rtp_user->IsInit() || p_rtp_user->IsLeave() || p_rtp_user->IsTerminate())
	{
		stAssert(0);
		return;
	}

	defRTCPUserMapIter iter = m_stRTCPUse.begin();
	
	for(; iter != m_stRTCPUse.end(); iter++)
	{
		if(iter->first != p_rtp_user->GetRTCPName())
		{
			stRTCPUser * p_user = iter->second;
			if(p_user)
			{
				if(! p_user->IsLeave())
				{
					if(!p_user->IsTerminate())
					{
						p_user->SendData(recv_buf, recv_len);
					}
				}
			}
			else
			{
				Global::logger->fatal("[rtcp_user] find rtcp user is null <cname=%s>", iter->first.c_str());
				stAssert(0);
			}
		}
	}
}

