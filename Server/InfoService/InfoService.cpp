#include "InfoService.h"
#include "GateTask.h"
#include "TransTask.h"
#include "FunctionTime.h"
#include "ServerConfig.h"

stInfoService * stInfoService::m_pInstance = NULL;

stInfoService::stInfoService(std::vector<WORD> & vec, const std::vector<std::string> & vecip , 
			const char * name, const int count):stMNetService(vec, vecip, name, count),
		m_stGateTaskSched(6,1), m_stTransTaskSched(6,1)
{
	m_pInstance = this;
	m_wdGatePort = 0;
	m_wdTransPort = 0;
		
	if(((int)vec.size() == count) && (count == MULTI_PORT_NUM))
	{
		m_wdGatePort = vec[enGatePort];
		m_wdTransPort = vec[enTransPort];
	}
}

bool stInfoService::CreateTask(const int & sock, const struct sockaddr_in * addr, const WORD & port)
{
	if(port == m_wdGatePort)
	{
		if(sock == -1 || addr == NULL)
		{
			return false;
		}

		stGateTask * p_task = new stGateTask(sock, addr);
		if(p_task == NULL)
		{
			TEMP_FAILURE_RETRY(close(sock));
			return false;
		}
		
		if(m_stGateTaskSched.AddNormalSched(p_task))
		{
			return true;
		}
		
		stAssert(0);
		delete p_task;
		TEMP_FAILURE_RETRY(close(sock));
		return false;
	}
	
	else if(port == m_wdTransPort)
	{
		if(sock == -1 || addr == NULL)
		{
			return false;
		}
		
		stTransTask * p_task = new stTransTask(sock, addr);
		if(p_task == NULL)
		{
			TEMP_FAILURE_RETRY(close(sock));
			return false;
		}
		
		if(m_stTransTaskSched.AddNormalSched(p_task))
		{
			return true;
		}
		
		stAssert(0);
		delete p_task;
		TEMP_FAILURE_RETRY(close(sock));
		return false;
	}
	else
	{
		if(sock != -1)
		{
			TEMP_FAILURE_RETRY(close(sock));
		}
	}
	
	return false;
}


bool stInfoService::Init()
{
	if(! stMNetService::Init())
	{
		return false;
	}
	
	m_stGateTaskSched.Init();
	m_stTransTaskSched.Init();
	Global::logger->info("[sys_init] infoservice initialization successful");
	return true;
}


void stInfoService::Final()
{
	stMNetService::Final();
}

bool stInfoService::CheckTransTaskIP(const char * task_ip, WORD & zone_id)
{
	return true;
}


bool stInfoService::CheckGateTaskIP(const char * task_ip, WORD & zone_id)
{
	return true;
}

