#include "GateService.h"
#include "GateTask.h"
#include "ServerConfig.h"
#include "Command.h"

stGateService * stGateService::m_pInstance = NULL;


bool stGateService::Init()
{
	const stGatewayConfig::stInfo * p_info = stServerConfig::GetInstance().GetGateWayInfo(GetChannelIDFromArgs());
	if(!p_info)
	{
		stAssert(0);
		return false;
	}
	
	if (!stNetService::Init(p_info->baseinfo.port, p_info->baseinfo.ip))
	{
		return false;
	}

	Global::g_strServiceIP = p_info->baseinfo.ip;

    m_stGateTaskSched.Init();

	Global::logger->info("[sys_init] gateservice initialization successful <%s,%d>",p_info->baseinfo.ip, p_info->baseinfo.port);
	
	return true;
}


bool stGateService::CreateTask(const int sock, const struct sockaddr_in * addr)
{
	if(sock == -1 || addr == NULL)
	{
		return false;
	}
	
	static DWORD l_order = 0;
	l_order++;
	Cmd::stTaskId tempid;
	tempid.server_id = GetChannelIDFromArgs();
	tempid.order_id = l_order;
		
	stGateTask * p_task = new stGateTask(sock, addr, tempid.taskid);
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


void stGateService::Final()
{
	stNetService::Final();
}



