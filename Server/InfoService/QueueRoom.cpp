#include "QueueRoom.h"
#include "QueueManager.h"
#include "InfoTick.h"
#include "NullCmd.h"
#include "InfoUser.h"

#define MIC_TIME 20

stQueueRoom::stQueueRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port):
	stBaseRoom(task_id, room_id, _type, r_ip, r_port)
{
	m_stCurMicTempId = 0;
	m_CurMicTime = 0;
	memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
}


bool stQueueRoom::Init()
{
	stBaseRoom::Init();
	m_stMicQueue.Init(this);
	return true;
}


void stQueueRoom::SendReConnectInfo(stInfoUser * p_user)
{
	/// 告知麦序
	if(m_stCurMicTempId != 0)
	{
		Cmd::cmdNotifyTakeMic send_cmd;
		send_cmd.set_tempid(m_stCurMicTempId);
		send_cmd.set_uniqueid(m_strCurMicUniqueId);
		if(stInfoTick::m_stRTime.GetMSec() > m_CurMicTime)
		{
			DWORD tt = stInfoTick::m_stRTime.GetMSec() - m_CurMicTime / 1000;
			send_cmd.set_mtime(tt);
		}
		else
		{
			send_cmd.set_mtime(0);
		}
		
		p_user->SendCmd(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
	}
	
	m_stMicQueue.NotifyMicQueueToUser(p_user);
}


void stQueueRoom::Update()
{
	stBaseRoom::Update();
	
	if(m_stCurMicTempId == 0)
	{
		stQueueInfo * p_info = m_stMicQueue.GetFirstMic();
		if(p_info)
		{
			m_stCurMicTempId = p_info->m_stTempId;
			memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
			strncpy(m_strCurMicUniqueId, p_info->uniqueid, sizeof(m_strCurMicUniqueId) - 1);
			m_CurMicTime = stInfoTick::m_stRTime.GetMSec();
			
			Cmd::cmdNotifyTakeMic send_cmd;
			send_cmd.set_tempid(m_stCurMicTempId);
			send_cmd.set_uniqueid(m_strCurMicUniqueId);
			send_cmd.set_mtime(MIC_TIME);
			BroadCast(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
			Global::logger->debug("[get_mic] notify cur who get mic change <tempid=%llu,uniqueid=%s>",
								  m_stCurMicTempId, m_strCurMicUniqueId);
			delete p_info;
		}
	}
	else
	{
		if(m_CurMicTime + MIC_TIME * 1000 < stInfoTick::m_stRTime.GetMSec())
		{
			Global::logger->debug("[leave_mic] mic time out will leave <tempid=%llu,uniqueid=%s>",
								  m_stCurMicTempId, m_strCurMicUniqueId);
			m_stCurMicTempId = 0;
			memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
			
			Cmd::cmdNotifyTakeMic send_cmd;
			send_cmd.set_tempid(m_stCurMicTempId);

			send_cmd.set_uniqueid(m_strCurMicUniqueId);
			send_cmd.set_mtime(MIC_TIME);
			BroadCast(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
			Global::logger->debug("[get_mic] notify cur not any  get mic, stop play <tempid=%llu>", m_stCurMicTempId);
		}
	}
}


bool stQueueRoom::EnterRoom(stInfoUser * p_user)
{
	if(! stBaseRoom::EnterRoom(p_user))
	{
		return false;
	}
	
	/// 告知麦序
	if(m_stCurMicTempId != 0)
	{
		Cmd::cmdNotifyTakeMic send_cmd;
		send_cmd.set_tempid(m_stCurMicTempId);
		send_cmd.set_uniqueid(m_strCurMicUniqueId);
		if(stInfoTick::m_stRTime.GetMSec() > m_CurMicTime)
		{
			DWORD tt = stInfoTick::m_stRTime.GetMSec() - m_CurMicTime / 1000;
			send_cmd.set_mtime(tt);
		}
		else
		{
			send_cmd.set_mtime(0);
		}
		
		p_user->SendCmd(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
	}
	
	m_stMicQueue.NotifyMicQueueToUser(p_user);
					
	return true;
}


void stQueueRoom::AddQueueUser(stInfoUser * p_user)
{
	Global::logger->debug("[request_join_mic] add to mic queue <roomid=%llu, tempid=%llu>", roomid, p_user->GetTempId());
	
	m_stMicQueue.AddQueueUser(p_user);
	BroadCastMicQueue();
}


void stQueueRoom::DelQueueUser(stInfoUser * p_user)
{
	if(m_stCurMicTempId == p_user->GetTempId())
	{
		m_stCurMicTempId = 0;
		memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
		m_CurMicTime = 0;
		Cmd::cmdNotifyTakeMic send_cmd;
		send_cmd.set_tempid(m_stCurMicTempId);
		send_cmd.set_uniqueid(m_strCurMicUniqueId);
		send_cmd.set_mtime(MIC_TIME);
		BroadCast(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
	}

	Global::logger->debug("[leave_mic] del mic from queue <roomid=%llu, tempid=%llu>", roomid, p_user->GetTempId());
	m_stMicQueue.DelQueueUser(p_user);
	BroadCastMicQueue();
}


void stQueueRoom::BroadCastMicQueue()
{
	m_stMicQueue.BroadCastMicQueue();
}


void stQueueRoom::LeaveRoom(stInfoUser * p_user)
{
	stBaseRoom::LeaveRoom(p_user);
	DelQueueUser(p_user);
}


