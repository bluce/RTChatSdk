#ifndef STGATESERVICE_H
#define STGATESERVICE_H

#include "ServiceTaskSched.h"
#include "NetService.h"


class stGateService: public stNetService
{
 public:
	bool Init();

	void Final();

    bool CreateTask(const int sock, const struct sockaddr_in * addr);

    static stGateService & GetInstance()
    {
        if(m_pInstance == NULL)
		{
			m_pInstance = new stGateService();
		}
        return * m_pInstance;
    }

    static void DelInstance()
    {
        if(m_pInstance)
		{
			delete m_pInstance;
		}
        m_pInstance = NULL;
    }

 	stGateService(): stNetService("GateSevice"), m_stGateTaskSched(6, 1)
	{
		
	}

    ~stGateService(){}
   

 private:

	static stGateService * m_pInstance;

	stServiceTaskSched m_stGateTaskSched;
};

#endif

