#include "Global.h"
#include "Share.h"
#include "ServerConfig.h"
#include "InfoService.h"
#include "InfoTick.h"

bool Init(int argc, char * argv[])
{
	PublicFun::ArgParse(argc, argv);

	Global::logger = new stLogger("info");
	
	Global::net_logger = Global::logger;

	if(stServerConfig::GetInstance().Load(Global::Value["config_path"].c_str()) == false)
	{
		Global::logger->fatal("[info_server] load info server config failed <%s>",Global::Value["config_path"].c_str());
		return false;
	}

	const stSvrInfoConfig & pInfoConfig = stServerConfig::GetInstance().GetInfoSvrConfig();
	
	if(pInfoConfig.transport == 0)
	{
		Global::logger->fatal("[init_loadconfig] load config.xml failed");
		return false;
	}
	
	Global::logger->AddLocalFileLog(pInfoConfig.logpath.c_str());

	std::vector<WORD> word_vec;
    word_vec.push_back(pInfoConfig.gateport);
    word_vec.push_back(pInfoConfig.transport);

	std::vector<std::string> ip_vec;
    ip_vec.push_back( pInfoConfig.gateip.c_str() );
    ip_vec.push_back( pInfoConfig.transip.c_str() );
	stInfoService::m_pInstance = new stInfoService(word_vec, ip_vec);

    if(! stInfoService::m_pInstance)
	{
		return false;
	}
	
	return true;
}


void Final()
{
	stInfoTick::GetInstance().DelInstance();
	
 	if(Global::logger)
	{
 		delete Global::logger;
	}
	
 	Global::logger = NULL;
}


int main(int argc, char * argv[])
{
	if(! Init(argc, argv))
	{
		Global::logger->fatal("[system_init] infoservice initializtion faield");
		return 0;
	}

	if(! stInfoTick::GetInstance().Start())
	{
		Global::logger->fatal("[info_init] info tick start failed");
		return 0;
	}

	Global::logger->info("[infoservice] <<<<<<<<<<<<<<<<<<<<<<<<<<<  infoservice start successful >>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

	Global::logger->RemoveConsoleLog();
	
	stInfoService::GetInstancePtr()->Main();
	
	Final();
}

