#include "Global.h"
#include "Share.h"
#include "ServerConfig.h"
#include "GateService.h"
#include "InfoClient.h"
#include "NullCmd.h"
#include "GateTick.h"

bool Init(int argc, char * argv[])
{
	PublicFun::ArgParse(argc, argv);

	Global::logger = new stLogger("gateway");
	Global::net_logger = Global::logger;

	if(stServerConfig::GetInstance().Load(Global::Value["config_path"].c_str()) == false)
	{
		Global::logger->fatal("[gateway_server] load gateway server config failed <%s>",Global::Value["config_path"].c_str());
		return false;
	}

	const stGatewayConfig::stInfo * p_info = stServerConfig::GetInstance().GetGateWayInfo(GetChannelIDFromArgs());
	if(!p_info)
	{
		stAssert(0);
		return false;
	}

	Global::logger->AddLocalFileLog(p_info->logpath.c_str());
	
	if(!stInfoClient::GetInstance().Init())
	{
		Global::logger->fatal("[init_infoclient] unable connect info server");
		return false;
	}
	else
	{
		Cmd::cmdRegisterSvr send_cmd;
		send_cmd.set_svrid(GetChannelIDFromArgs());
		stInfoClient::GetInstance().SendCmd(0, Cmd::enRegisterSvr, &send_cmd, send_cmd.ByteSize());
	}

	return true;
}


void Final()
{
	stInfoClient::GetInstance().Final();

	stGateTick::GetInstance().DelInstance();
		
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
		Global::logger->fatal("[system_init] gateway initializtion faield");
		return 0;
	}

	if(! stGateTick::GetInstance().Start())
	{
		Global::logger->fatal("[gate_init] gate tick start failed");
		return 0;
	}

	std::ostringstream os;
	os<<"[gameservice] <<<<<<<<<<<<<<<<<<<<<<<<<<< gateway service "<< GetChannelIDFromArgs()
	  <<" start successful >>>>>>>>>>>>>>>>>>>>>>>>>>>\n";

	Global::logger->info(os.str().c_str());
	
	
	Global::logger->RemoveConsoleLog();
	
	stGateService::GetInstance().Main();
	
	Final();
}

