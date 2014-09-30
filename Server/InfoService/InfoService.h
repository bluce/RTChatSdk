#ifndef STINFOSERVICE_H
#define STINFOSERVICE_H

#include "ServiceTaskSched.h"
#include "MNetService.h"

const int MULTI_PORT_NUM = 2;

enum enPortIndex
{
	enGatePort,
	enTransPort,
	enMaxPort
};

class stInfoService: public stMNetService
{
 public:

	stInfoService(std::vector<WORD> & vec, const std::vector<std::string> & vecip , 
				  const char * name = "infoservice", const int count = MULTI_PORT_NUM);
	
	bool CreateTask(const int & sock, const struct sockaddr_in * addr, const WORD & port);

	bool Init();

	void Final();

	static stInfoService * m_pInstance;
	
	static stInfoService * GetInstancePtr()
	{
		return m_pInstance;
	}

	bool CheckTransTaskIP(const char * ip, WORD & zone_id);
	
	bool CheckGateTaskIP(const char * ip, WORD & zone_id);

 private:

	WORD m_wdGatePort;

	WORD m_wdTransPort;

	stServiceTaskSched m_stGateTaskSched;

	stServiceTaskSched m_stTransTaskSched;
};

#endif

