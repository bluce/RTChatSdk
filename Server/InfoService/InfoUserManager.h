#ifndef _STINFOUSERMANAGER_H
#define _STINFOUSERKMANAGER_H

#include "Global.h"
#include "InfoUser.h"


class stInfoUserManager
{
 public:
	typedef std::map<defTempId, stInfoUser * > defInfoUserMap;
	typedef std::map<defTempId, stInfoUser * >::iterator defInfoUserMapIter;
	typedef std::map<defTempId, stInfoUser * >::value_type defInfoUserMapValue;
	
	static stInfoUserManager & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stInfoUserManager();
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

	bool AddUser(stInfoUser * p_user);

	void RemoveUser(const defTempId temp_id);

	stInfoUser * GetUser(const defTempId temp_id);

	void RemoveGateService(WORD task_id);

	stInfoUser * GetNotInRoomUser(const defTempId temp_id);

	stInfoUser * GetRandUser(const defTempId temp_id);

	const DWORD Size();
 private:
	
	static stInfoUserManager * m_pInstance;

	defInfoUserMap  m_stUserMap;

};

#endif
