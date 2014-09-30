#ifndef _ST_ASSIGN_PORT_H
#define _ST_ASSIGN_PORT_H

#include "Global.h"
#include "RWLock.h"

class stAssignPort
{
 public:
	static stAssignPort & GetInstance();

	static void DelInstance();

	~stAssignPort();

	bool Init();

	DWORD GetPort();

	void PutPort(DWORD port);
	
 private:
	stAssignPort();
	
	static stAssignPort * m_pInstance;

	std::set<DWORD > m_stPortSet;

	stRWLock m_stLock;
};

#endif
