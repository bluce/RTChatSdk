#ifndef STLOGINSERVICE_H
#define STLOGINSERVICE_H

#include "ServiceTaskSched.h"
#include "NetService.h"


class stLoginService: public stNetService
{
 public:
	bool Init();

	void Final();

    bool CreateTask(const int sock, const struct sockaddr_in * addr);

    static stLoginService & GetInstance()
    {
        if(m_pInstance == NULL)
		{
			m_pInstance = new stLoginService();
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

 	stLoginService(): stNetService("LoginSevice"), m_stLoginTaskSched(6, 1){}

    ~stLoginService(){}
   

 private:

	static stLoginService * m_pInstance;

	stServiceTaskSched m_stLoginTaskSched;
};

#endif

