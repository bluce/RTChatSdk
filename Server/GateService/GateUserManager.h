#ifndef _STGATEUSERMANAGER_H
#define _STGATEUSERMANAGER_H

#include "Global.h"
#include "RWLock.h"
#include "GateUser.h"

class stGateUserManager
{
 public:
	typedef std::map<defTempId, stGateUser * > defGateUserMap;
	typedef std::map<defTempId, stGateUser * >::iterator defGateUserMapIter;
	typedef std::map<defTempId, stGateUser * >::value_type defGateUserMapValue;
	
	static stGateUserManager & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stGateUserManager();
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

	void AddUser(stGateUser * p_task);

	stGateUser * GetUser(const defTempId & task_id);

	void Update(QWORD cur_time);
	
 private:
	
	static stGateUserManager * m_pInstance;

	defGateUserMap  m_stUserMap;

	stRWLock m_stRWLock;
};

#endif
