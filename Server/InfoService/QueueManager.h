#ifndef _QUEUE_MANAGER_H
#define _QUEUE_MANAGER_H

#include "Global.h"
#include "Define.h"

class stInfoUser;
class stQueueRoom;

struct stQueueInfo
{
	stQueueInfo(defTempId temp_id, const char * unique_id)
	{
		m_stTempId = temp_id;
		memset(uniqueid, 0, sizeof(uniqueid));
		strncpy(uniqueid, unique_id, sizeof(uniqueid) - 1 );
	}

	stQueueInfo(const stQueueInfo & info)
	{
		m_stTempId = info.m_stTempId;
		memset(uniqueid, 0, sizeof(uniqueid));
		strncpy(uniqueid, info.uniqueid, sizeof(uniqueid) - 1 );
	}
	
	defTempId m_stTempId;
	char uniqueid[UNIQUE_LEN];
};


class stQueueManager
{
 public:
	typedef std::map<DWORD, stQueueInfo * > defMicQueueMap;
	typedef std::map<DWORD, stQueueInfo * >::iterator  defMicQueueMapIter;
	typedef std::map<DWORD, stQueueInfo * >::value_type defMicQueueMapValue;
	
	stQueueManager();
	
	~stQueueManager();

	void Init(stQueueRoom * p_room);

	bool AddQueueUser(stInfoUser * p_user);

	bool DelQueueUser(stInfoUser * p_user);

	void BroadCastMicQueue();

	stQueueInfo * GetFirstMic();

	bool IsEmpty();

	void NotifyMicQueueToUser(stInfoUser * p_user);
	
 private:
	bool IsHaveMic(stInfoUser * p_user);

	std::map<DWORD, stQueueInfo * > m_stMicQueue;

	stQueueRoom * m_pRoom;

	DWORD micorder;
};

#endif
