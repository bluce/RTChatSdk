#ifndef _ST_NETSERVICE_H
#define _ST_NETSERVICE_H

#include "Service.h"
#include "TCPService.h"

class stNetService: public stService
{
 public:
	virtual ~stNetService()
	{
		m_pNetInstance = NULL;
	}

	static stNetService * GetNetService()
	{
		return m_pNetInstance;
	}

 protected:
	stNetService(const char * net_name): stService(net_name)
	{
		m_pNetInstance = this;
		m_strNetName = net_name;
		m_pTCPService = NULL;
	}
		
	virtual bool CreateTask(const int sock, const struct sockaddr_in * addr) = 0;
	
	bool Init(const WORD & port, const char * service_ip);

	bool ServiceCallBack();

	virtual	void Final();
 private:

	std::string m_strNetName;
	
	stTCPService * m_pTCPService;

	static stNetService * m_pNetInstance;
};

#endif
