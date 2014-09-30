#ifndef STINFOTICK_H
#define STINFOTICK_H

#include "Type.h"
#include "Timer.h"
#include "RWLock.h"
#include "Thread.h"
#include "CommandQueue.h"

class stInfoTick: public stThread, public stCmdParse
{
public:
    stInfoTick();
	
    virtual ~stInfoTick();

    void Final();

    virtual void Run();

	static stInfoTick & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stInfoTick;
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


	void PutCmd(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size);
	
	bool CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size);

	static stRTime m_stRTime;
private:
	
	static stInfoTick * m_pInstance;
	
	stRWLock m_stCmdLock;
};

#endif
