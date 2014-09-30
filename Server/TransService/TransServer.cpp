#include "Global.h"
#include "Share.h"
#include "ServerConfig.h"
#include "TransService.h"
#include "InfoClient.h"
#include "AssignPort.h"
#include "NullCmd.h"

bool Init(int argc, char * argv[])
{
	PublicFun::ArgParse(argc, argv);

	Global::logger = new stLogger("game");
	
	Global::net_logger = Global::logger;

	if(stServerConfig::GetInstance().Load(Global::Value["config_path"].c_str()) == false)
	{
		Global::logger->fatal("[game_server] load info server config failed <%s>",Global::Value["config_path"].c_str());
		return false;
	}

	const stTransConfig::stInfo * p_info = stServerConfig::GetInstance().GetTransInfo(GetChannelIDFromArgs());

    if(!p_info)
	{
		Global::logger->fatal("[load_config] gameservice load config self failed");
		return false;
	}

	Global::g_dwMinPort = p_info->minport;
	
	Global::g_dwMaxPort = p_info->maxport;

	Global::logger->AddLocalFileLog(p_info->logpath.c_str());

	//Global::logger->SetLevel("info");

	const stSvrInfoConfig & pInfoConfig = stServerConfig::GetInstance().GetInfoSvrConfig();
	if(pInfoConfig.transport == 0)
	{
		Global::logger->fatal("[init_loadconfig] load config.xml failed");
		return false;
	}

	if(! stAssignPort::GetInstance().Init())
	{
		Global::logger->fatal("[init] load udp port failed <min=%u,max=%u>", Global::g_dwMinPort, Global::g_dwMaxPort);
		stAssert(0);
		return false;
	}
	

	if(!stInfoClient::GetInstance().Init())
	{
		Global::logger->fatal("[init_infoclient] unable connect info server");
		return false;
	}
	else
	{
		Cmd::cmdRegisterSvr send_cmd;
		send_cmd.set_svrid(GetChannelIDFromArgs());
		stInfoClient::GetInstance().SendCmd(0, (Cmd::enCmdId)Cmd::enRegisterSvr, &send_cmd, send_cmd.ByteSize());
	}

		
	return true;
}


void Final()
{
	stInfoClient::GetInstance().DelInstance();

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
		Global::logger->fatal("[system_init] transservice initializtion faield");
		return 0;
	}

	std::ostringstream os;
	os<<"[gameservice] <<<<<<<<<<<<<<<<<<<<<<<<<<< trans service "<< GetChannelIDFromArgs()
	  <<" start successful >>>>>>>>>>>>>>>>>>>>>>>>>>>\n";

	Global::logger->info(os.str().c_str());

	Global::logger->RemoveConsoleLog();
	
	stTransService::GetInstance().Main();
	
	Final();
}

