#include "AssignPort.h"

stAssignPort * stAssignPort::m_pInstance = NULL;


stAssignPort & stAssignPort::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new stAssignPort();
	}
	return *m_pInstance;
}

void stAssignPort::DelInstance()
{
	if(m_pInstance)
	{
		delete m_pInstance;
	}
	m_pInstance = NULL;
}

stAssignPort::~stAssignPort(){}


bool stAssignPort::Init()
{
	m_stLock.wrlock();
	unsigned int begin = Global::g_dwMinPort;
	unsigned int end = Global::g_dwMaxPort;
	for(; begin < end; begin = begin + 2)
	{
		m_stPortSet.insert(begin);
	}
	m_stLock.unlock();
	
	return true;
}

DWORD stAssignPort::GetPort()
{
	m_stLock.wrlock();
	if(m_stPortSet.empty())
	{
		m_stLock.unlock();
		return 0;
	}

	std::set<DWORD >::iterator iter = m_stPortSet.begin();
	DWORD ret_port = *iter;
	m_stPortSet.erase(iter);
	m_stLock.unlock();
	
	return ret_port;
}

void stAssignPort::PutPort(DWORD port)
{
	m_stLock.wrlock();
	m_stPortSet.insert(port);
	m_stLock.unlock();
}
	
stAssignPort::stAssignPort()
{
	
}

