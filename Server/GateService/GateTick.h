#ifndef STGATETICK_H
#define STGATETICK_H

#include "Type.h"
#include "Timer.h"
#include "RWLock.h"
#include "Thread.h"


class stGateTick: public stThread
{
public:
    stGateTick();
	
    virtual ~stGateTick();

    void Final();

    virtual void Run();

	static stGateTick & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stGateTick;
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

	static stRTime m_stRTime;
	
private:
	
	static stGateTick * m_pInstance;
	
	stRWLock m_stCmdLock;
};

#endif
