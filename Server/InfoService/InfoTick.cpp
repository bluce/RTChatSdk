#include "Global.h"
#include "InfoTick.h"
#include "TransTaskManager.h"
#include "GateTaskManager.h"
#include "BaseRoomManager.h"

stInfoTick * stInfoTick::m_pInstance = NULL;

stRTime stInfoTick::m_stRTime;


stInfoTick::stInfoTick() : stThread("infotick")
{
	
}

stInfoTick::~stInfoTick()
{
	
}


void stInfoTick::PutCmd(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size)
{
	m_stCmdLock.wrlock();
	Put(pt_null_cmd, cmd_size);
	m_stCmdLock.unlock();
}
	

bool stInfoTick::CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size)
{
/// 内部命令
	if(pt_null_cmd->byCmd == 1)
	{
		/// 增加一个新用户
		if(pt_null_cmd->byParam == 1)
		{
			
		}
	}
	return true;
}


void stInfoTick::Run()
{

	QWORD get_time_delay = 0;
	
	while(! IsTTerminate())
	{
		::usleep(10000);
		m_stRTime.GetNow();

		stTransTaskManager::GetInstance().DealQueueCmd();
		stGateTaskManager::GetInstance().DealQueueCmd();
		stBaseRoomManager::GetInstance().Update();
		
		if(m_stRTime.GetMSec() > get_time_delay)
		{
		   	get_time_delay = m_stRTime.GetMSec() + 1000 * 60;
		}
	}
	
    Final();
}

void stInfoTick::Final()
{
	
}
