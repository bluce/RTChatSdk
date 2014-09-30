#include "Global.h"
#include "Share.h"
#include "ServerConfig.h"
#include "LoginService.h"

bool Init(int argc, char * argv[])
{
	PublicFun::ArgParse(argc, argv);

	Global::logger = new stLogger("login");
	Global::net_logger = Global::logger;

	if(stServerConfig::GetInstance().Load(Global::Value["config_path"].c_str()) == false)
	{
		Global::logger->fatal("[login_server] load login server config failed <%s>",Global::Value["config_path"].c_str());
		return false;
	}

	const stSvrLoginConfig & pLoginConfig = stServerConfig::GetInstance().GetLoginSvrConfig();
	
	if(pLoginConfig.port == 0)
	{
		Global::logger->fatal("[init_loadconfig] load config.xml failed");
		return false;
	}
	

	Global::logger->AddLocalFileLog(pLoginConfig.logpath.c_str());
	
	
	return true;
}


void Final()
{
	//stInfoClient::GetInstance().Final();
		
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
		Global::logger->fatal("[system_init] loginservice initializtion faield");
		return 0;
	}
	
	Global::logger->info("[loginservice] <<<<<<<<<<<<<<<<<<<<<<<<< loginservice start successful >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	
	Global::logger->RemoveConsoleLog();
	
	stLoginService::GetInstance().Main();
	
	Final();
}

