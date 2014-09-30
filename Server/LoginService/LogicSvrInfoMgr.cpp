#include "LogicSvrInfoMgr.h"
#include "Command.h"
#include "AssertEx.h"

stLogicSvrInfoMgr * stLogicSvrInfoMgr::m_pInstance = NULL;


stLogicSvrInfoMgr::stLogicSvrInfoMgr()
{
	
}

void stLogicSvrInfoMgr::AddLogicSvrInfo(const WORD & svr_id, Cmd::stLogicSvrInfo & info)
{
	m_stRWLock.wrlock();
	
	std::pair<defLogicSvrInfoMapIter, bool> ret_code;
	ret_code = m_stLogicSvrInfo.insert(defLogicSvrInfoMapValue(svr_id, info));
	
	if(! ret_code.second)
	{
		Global::logger->info("[loginserver_register_logicsvr] register a logic server info <id=%u,ip=%s,port=%u>",
							 info.id, info.ip, info.port);
		stAssert(0);
	}
	
	m_stRWLock.unlock();
}


void stLogicSvrInfoMgr::RemoveLogicSvrInfo(const WORD & svr_id)
{
	m_stRWLock.wrlock();
	defLogicSvrInfoMapIter iter = m_stLogicSvrInfo.find(svr_id);
	if(iter != m_stLogicSvrInfo.end())
	{
		m_stLogicSvrInfo.erase(iter);
	}
	
	m_stRWLock.unlock();
}

