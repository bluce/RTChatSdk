#include "LoginService.h"
#include "LoginTask.h"
#include "ServerConfig.h"

stLoginService * stLoginService::m_pInstance = NULL;


bool stLoginService::Init()
{
	const stSvrLoginConfig & pLoginConfig = stServerConfig::GetInstance().GetLoginSvrConfig();
	
	if(pLoginConfig.port == 0)
	{
		Global::logger->fatal("[init_loadconfig] load config.xml failed");
		return false;
	}

	if (!stNetService::Init(pLoginConfig.port, pLoginConfig.ip.c_str()))
	{
		return false;
	}

    m_stLoginTaskSched.Init();

	Global::logger->info("[sys_init] loginservice initialization successful <%s,%d>",pLoginConfig.ip.c_str(), pLoginConfig.port);
	return true;
}


bool stLoginService::CreateTask(const int sock, const struct sockaddr_in * addr)
{
	if(sock == -1 || addr == NULL)
	{
		return false;
	}

	static QWORD l_order = 0;
	l_order++;
	stLoginTask * p_task = new stLoginTask(sock, addr, l_order);
	if(p_task == NULL)
	{
		TEMP_FAILURE_RETRY(close(sock));
		return false;
	}

	if(m_stLoginTaskSched.AddNormalSched(p_task))
	{
		return true;
	}
	
	stAssert(0);
	delete p_task;
	TEMP_FAILURE_RETRY(close(sock));
	return false;
}


void stLoginService::Final()
{
	stNetService::Final();
}



