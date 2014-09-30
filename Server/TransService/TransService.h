#ifndef STTRANSSERVICE_H
#define STTRANSSERVICE_H

#include "ServiceTaskSched.h"
#include "NetService.h"

class stTransService: public stService
{
 public:
	bool Init();
		
	void Final();

	bool ServiceCallBack();

    static stTransService & GetInstance()
    {
        if(m_pInstance == NULL)
		{
			m_pInstance = new stTransService("transservice");
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

 	
    virtual ~stTransService()
	{
		
	}


 	stTransService(const char * name): stService(name){}

 private:
	
	static stTransService * m_pInstance;
};

#endif

