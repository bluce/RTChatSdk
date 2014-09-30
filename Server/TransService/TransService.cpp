#include "TransService.h"
#include "ServerConfig.h"
#include "InfoClient.h"

stTransService * stTransService::m_pInstance = NULL;

bool stTransService::Init()
{
	const stTransConfig::stInfo * p_info = stServerConfig::GetInstance().GetTransInfo(GetChannelIDFromArgs());
	
    if(! p_info)
	{
		Global::logger->fatal("[load_config] transservice load config self failed");
		return false;
	}
	
	Global::g_strServiceIP = p_info->baseinfo.ip;

	Global::g_wdChannelId = GetChannelIDFromArgs();

	Global::logger->info("[sys_init] transservice start successful <%s>",p_info->baseinfo.ip);
	
	return true;
}


bool stTransService::ServiceCallBack()
{
	::usleep(50000);
	return true;
}


void stTransService::Final()
{

}
