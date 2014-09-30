#include "Global.h"
#include "GateTick.h"
#include "GateUserManager.h"

stGateTick * stGateTick::m_pInstance = NULL;


stRTime stGateTick::m_stRTime;


stGateTick::stGateTick() : stThread("infotick")
{
	
}


stGateTick::~stGateTick()
{
	
}


void stGateTick::Run()
{

	QWORD get_time_delay = 0;
	
	while(! IsTTerminate())
	{
		::usleep(10000);
		m_stRTime.GetNow();

		stGateUserManager::GetInstance().Update(m_stRTime.GetMSec());
		
		if(m_stRTime.GetMSec() > get_time_delay)
		{
		   	get_time_delay = m_stRTime.GetMSec() + 1000 * 60;
		}
	}
	
    Final();
}


void stGateTick::Final()
{
	
}
