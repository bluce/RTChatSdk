#ifndef _STBASEROOMMANAGER_H
#define _STBASEROOMKMANAGER_H

#include "Global.h"
#include "BaseRoom.h"
#include "NullCmd.h"

class stBaseRoomManager
{
 public:
	typedef std::map<QWORD, stBaseRoom * > defBaseRoomMap;
	typedef std::map<QWORD, stBaseRoom * >::iterator defBaseRoomMapIter;
	typedef std::map<QWORD, stBaseRoom * >::value_type defBaseRoomMapValue;
	
	static stBaseRoomManager & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stBaseRoomManager();
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

	bool GetRoomInfos(Cmd::cmdNotifyRoomList & send_cmd);
		
	bool AddRoom(stBaseRoom * p_room);

	void RemoveRoom(const QWORD temp_id);

	void RemoveTransService(const WORD task_id);

	stBaseRoom * GetRoom(const QWORD temp_id);

	stBaseRoom * CreateRoom(WORD task_id, QWORD room_id, Cmd::enRoomType room_type, const char * r_ip , const unsigned int r_port);

	const DWORD Size();

	void Update();
	
 private:
	
	static stBaseRoomManager * m_pInstance;

	defBaseRoomMap  m_stRoomMap;

};

#endif
