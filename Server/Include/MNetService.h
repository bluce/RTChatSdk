#ifndef _ST_MNETSERVICE_H
#define _ST_MNETSERVICE_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Type.h"
#include "Service.h"
#include "MTCPService.h"

class stMNetService: public stService
{
 public:
	virtual ~stMNetService()
	{
		Final();
	}

	static stMNetService * GetMNetService()
	{
		return m_pMNetInstance;
	}

 protected:

	stMNetService(std::vector<WORD> vec, std::vector<std::string> ip_vec, const char * name, const int count);

   	bool Init();

	void Final();

	virtual bool CreateTask(const int & sock, const struct sockaddr_in * addr, const WORD & port) = 0;


	bool ServiceCallBack()
	{
		m_stAcceptAddrVec.clear();
		m_stAcceptSockVec.clear();
		m_stAcceptPortVec.clear();
		int ret_code = m_stMTCPService.Accept(m_stAcceptAddrVec, m_stAcceptSockVec, m_stAcceptPortVec);
		if(ret_code)
		{
			for(int i = 0; i<ret_code; i++)
			{
				CreateTask(m_stAcceptSockVec[i], &(m_stAcceptAddrVec[i]), m_stAcceptPortVec[i]);
			}
		}
		return true;
	}

 protected:

	static stMNetService * m_pMNetInstance;
	
	std::string m_strMNetName;

	stMTCPService m_stMTCPService;

	std::vector<WORD > m_PortVec;

	std::vector<struct sockaddr_in > m_stAcceptAddrVec;
	
	std::vector<int> m_stAcceptSockVec;
	
	std::vector<WORD> m_stAcceptPortVec;

	const int m_iCount;
};

#endif
