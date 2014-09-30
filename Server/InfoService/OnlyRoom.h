#ifndef _ST_ONLYROOM_H
#define _ST_ONLYROOM_H

#include "BaseRoom.h"

class stInfoUser;

class stOnlyRoom: public stBaseRoom
{
 public:
	stOnlyRoom(WORD task_id, QWORD room_id, Cmd::enRoomType _type, const char * r_ip , const unsigned int r_port);

	bool Init();

	void Update();

	bool EnterRoom(stInfoUser * p_user);
	
	void LeaveRoom(stInfoUser * p_user);

	void AddQueueUser(stInfoUser * p_user);

	void DelQueueUser(stInfoUser * p_user);

	void ClearMic();

	void BroadCastMicQueue();

 private:

	defTempId m_stCurMicTempId;

	char m_strCurMicUniqueId[UNIQUE_LEN];

	QWORD m_CurMicTime;
	
};

#endif
