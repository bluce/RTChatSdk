#ifndef _STLOGICSVRINFOMGR_H
#define _STLOGICSVRINFOMGR_H

#include "Command.h"
#include "Global.h"
#include "RWLock.h"


class stLogicSvrInfoMgr
{
 public:
	typedef std::map<DWORD, Cmd::stLogicSvrInfo > defLogicSvrInfoMap;
	typedef std::map<DWORD, Cmd::stLogicSvrInfo >::iterator defLogicSvrInfoMapIter;
	typedef std::map<DWORD, Cmd::stLogicSvrInfo >::value_type defLogicSvrInfoMapValue;
	
	static stLogicSvrInfoMgr & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stLogicSvrInfoMgr();
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

	void AddLogicSvrInfo(const WORD & svr_id, Cmd::stLogicSvrInfo & info);

	void RemoveLogicSvrInfo(const WORD & svr_id);

	stLogicSvrInfoMgr();
	
 private:
	
	static stLogicSvrInfoMgr * m_pInstance;

	defLogicSvrInfoMap  m_stLogicSvrInfo;

	stRWLock m_stRWLock;
};

#endif
