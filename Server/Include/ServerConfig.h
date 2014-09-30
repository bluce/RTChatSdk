#ifndef ST_SERVERCONFIG_H_
#define ST_SERVERCONFIG_H_

#include "Type.h"
#include "XMLParser.h"
#include <vector>
#include <string>
#include <map>
#include <set>


class stGatewayConfig
{
 public:
	struct stBaseInfo
	{
		stBaseInfo()
		{
			id = 0;
			memset(ip, 0, sizeof(ip));
			port = 0;
		}
		unsigned int id;
		char ip[16];
		unsigned short port;
	};

	struct stInfo
	{
		stBaseInfo baseinfo;
		std::string logpath;
	};

	const stInfo * GetInfoByID(unsigned int svr_id)  const;
		
	std::map<WORD, const stInfo *>  stSvrInfo;
};


class stTransConfig
{
 public:
	struct stBaseInfo
	{
		stBaseInfo()
		{
			id = 0;
			memset(ip, 0, sizeof(ip));
		}
		
		unsigned int id;
		char ip[16];
	};

	struct stInfo
	{
		stInfo()
		{
			minport = 0;
			maxport = 0;
		}
		stBaseInfo baseinfo;
		std::string logpath;
		unsigned int minport;
		unsigned int maxport;
	};

	const stInfo * GetInfoByID(unsigned int svr_id)  const;
		
	std::map<WORD, const stInfo *>  stSvrInfo;
};


class stSvrInfoConfig
{
 public:
	std::string gateip;
	WORD gateport;
	std::string transip;
	WORD transport;
	std::string logpath;
};


class stSvrLoginConfig
{
 public:
	std::string ip;
	WORD port;
	std::string logpath;
};


class stServerConfig
{
 public:

	stServerConfig();
	~stServerConfig();

	bool Load(const char * szConfigFile);

	static stServerConfig &	GetInstance();

	const stGatewayConfig::stInfo * GetGateWayInfo(WORD id);

	const stTransConfig::stInfo  * GetTransInfo(WORD id);

	const stSvrInfoConfig & GetInfoSvrConfig();

	const stSvrLoginConfig & GetLoginSvrConfig();
	
 private:

	bool LoadGatewaySvr( stXMLParser & xmlFile , xmlNodePtr & pRoot);

	bool LoadTransSvr( stXMLParser & xmlFile , xmlNodePtr & pRoot);

	bool LoadInfoSvr( stXMLParser & xmlFile , xmlNodePtr & pRoot);

	bool LoadLoginSvr( stXMLParser & xmlFile , xmlNodePtr & pRoot);
	
	stGatewayConfig m_stGatewayInfo;

	stTransConfig m_stTransInfo;

	stSvrInfoConfig m_strInfoSvr;

	stSvrLoginConfig m_strLoginSvr;
};

#endif
