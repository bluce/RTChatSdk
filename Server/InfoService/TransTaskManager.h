#ifndef _STTRANSTASKMANAGER_H
#define _STTRANSTASKMANAGER_H

#include "Global.h"
#include "RWLock.h"
#include "TransTask.h"


class stTransTaskManager
{
 public:
	typedef std::map<WORD, stTransTask * > defTransTaskMap;
	typedef std::map<WORD, stTransTask * >::iterator defTransTaskMapIter;
	typedef std::map<WORD, stTransTask * >::value_type defTransTaskMapValue;
	
	static stTransTaskManager & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stTransTaskManager();
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

	void AddTask(stTransTask * p_task);

	void RemoveTask(stTransTask * p_task);

	stTransTask * GetTransTask(const WORD & zone_id);

	const DWORD Size() ;

	void DealQueueCmd();

	void Broadcast(const char * p_send, const unsigned int send_size);

	stTransTask * GetMinTask();
	
 private:
	
	static stTransTaskManager * m_pInstance;

	defTransTaskMap  m_stTaskMap;

	stRWLock m_stRWLock;
};

#endif
