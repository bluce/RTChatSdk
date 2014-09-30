#ifndef _ST_QUEUEROOM_H
#define _ST_QUEUEROOM_H

#include "BaseRoom.h"
#include "QueueManager.h"

class stInfoUser;

class stQueueRoom: public stBaseRoom
{
 public:
	stQueueRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port);

	bool Init();

	void Update();

	bool EnterRoom(stInfoUser * p_user);
	
	void LeaveRoom(stInfoUser * p_user);

	void AddQueueUser(stInfoUser * p_user);

	void DelQueueUser(stInfoUser * p_user);

	void BroadCastMicQueue();

	virtual void SendReConnectInfo(stInfoUser * p_user);

 private:

	stQueueManager m_stMicQueue;

	defTempId m_stCurMicTempId;

	char m_strCurMicUniqueId[UNIQUE_LEN];

	QWORD m_CurMicTime;
	
};

#endif
