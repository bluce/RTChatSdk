#include "OnlyRoom.h"
#include "InfoTick.h"
#include "NullCmd.h"
#include "InfoUser.h"

#define ONLY_MIC_TIME 60

stOnlyRoom::stOnlyRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port):
	stBaseRoom(task_id, room_id, _type, r_ip, r_port)
{
	m_stCurMicTempId = 0;
	m_CurMicTime = 0;
	memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
}


bool stOnlyRoom::Init()
{
	stBaseRoom::Init();
	return true;
}


void stOnlyRoom::ClearMic()
{
	m_stCurMicTempId = 0;
	memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
	
	Cmd::cmdNotifyTakeMic send_cmd;
	send_cmd.set_tempid(m_stCurMicTempId);
	send_cmd.set_uniqueid(m_strCurMicUniqueId);
	send_cmd.set_mtime(ONLY_MIC_TIME);
	BroadCast(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
	Global::logger->debug("[get_mic] notify cur not any  get mic, stop play <tempid=%llu>", m_stCurMicTempId);
}


void stOnlyRoom::Update()
{
	stBaseRoom::Update();
	
	if(m_stCurMicTempId != 0)
	{
		if(m_CurMicTime + ONLY_MIC_TIME * 1000 < stInfoTick::m_stRTime.GetMSec())
		{
			Global::logger->debug("[leave_mic] only mic time out will leave <tempid=%llu,uniqueid=%s>",
								  m_stCurMicTempId, m_strCurMicUniqueId);
			m_stCurMicTempId = 0;
			memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
			
			Cmd::cmdNotifyTakeMic send_cmd;
			send_cmd.set_tempid(m_stCurMicTempId);
			send_cmd.set_uniqueid(m_strCurMicUniqueId);
			send_cmd.set_mtime(ONLY_MIC_TIME);
			BroadCast(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
			Global::logger->debug("[get_mic] notify cur not any  get mic, stop play <tempid=%llu>", m_stCurMicTempId);
		}
	}
}


bool stOnlyRoom::EnterRoom(stInfoUser * p_user)
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
	
	return true;
}


void stOnlyRoom::AddQueueUser(stInfoUser * p_user)
{
	Global::logger->debug("[request_only_mic] request only mic <roomid=%llu, tempid=%llu>", roomid, p_user->GetTempId());

	//if(m_stCurMicTempId == 0)
	
	{
		m_stCurMicTempId = p_user->GetTempId();
		memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
		strncpy(m_strCurMicUniqueId, p_user->GetUniqueId(), sizeof(m_strCurMicUniqueId)-1);
		Cmd::cmdNotifyTakeMic send_cmd;
		send_cmd.set_tempid(m_stCurMicTempId);
		send_cmd.set_uniqueid(m_strCurMicUniqueId);
		m_CurMicTime = stInfoTick::m_stRTime.GetMSec();
		send_cmd.set_mtime(ONLY_MIC_TIME);
		BroadCast(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
	}
}


void stOnlyRoom::DelQueueUser(stInfoUser * p_user)
{
	if(m_stCurMicTempId == p_user->GetTempId())
	{
		m_stCurMicTempId = 0;
		memset(m_strCurMicUniqueId, 0, sizeof(m_strCurMicUniqueId));
		m_CurMicTime = 0;
		Cmd::cmdNotifyTakeMic send_cmd;
		send_cmd.set_tempid(m_stCurMicTempId);
		send_cmd.set_uniqueid(m_strCurMicUniqueId);
		send_cmd.set_mtime(ONLY_MIC_TIME);
		BroadCast(Cmd::enNotifyTakeMic, &send_cmd, send_cmd.ByteSize());
		Global::logger->debug("[leave_mic] del mic from only room <roomid=%llu, tempid=%llu>", roomid, p_user->GetTempId());
	}
}


void stOnlyRoom::LeaveRoom(stInfoUser * p_user)
{
	stBaseRoom::LeaveRoom(p_user);
	DelQueueUser(p_user);
}


