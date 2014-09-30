#ifndef _ST_GATEUSER_H
#define _ST_GATEUSER_H

#include "Global.h"
#include "RWLock.h"

class stGateTask;

class stGateUser
{
 public:
	stGateUser(const defTempId temp_id, const char * uid);
	
	defTempId GetTempId() const;

	bool IsConnect();

	void Connect();

	void DisConnect(defTempId _id, QWORD _time);

	stGateTask * GetTask();

	void SetTask(stGateTask * p_task);

	bool IsTerminate();

	void Terminate();

	bool CheckTerminte(QWORD _cur);

	const char * GetUniqueId();
	
 private:
	defTempId taskid;
	
	defTempId tempid;

	bool m_blIsConnect;

	bool m_blIsTerminate;

	QWORD m_qdDisTime;

	char uniqueid[UNIQUE_LEN];

	stRWLock m_stLock;

	stRWLock m_TaskLock;
};

#endif
