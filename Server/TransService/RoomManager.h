#ifndef _STROOMANAGER_H
#define _STROOMANAGER_H

#include "Global.h"
#include "RWLock.h"
#include "BaseRoom.h"
#include "FreeRoom.h"


/**
 *
 * @brief 房间管理类
 *
 */
class stRoomManager
{
 public:
	typedef std::map<defRoomId, stBaseRoom * > defRoomMap;
	typedef std::map<defRoomId, stBaseRoom * >::iterator defRoomMapIter;
	typedef std::map<defRoomId, stBaseRoom * >::value_type defRoomMapValue;
	
	static stRoomManager & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stRoomManager();
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


	QWORD CreateRoom(WORD type);

	bool RemoveRoom(stBaseRoom * p_room);

	stBaseRoom * GetRoom(const defRoomId & room_id);

	void Update();

	virtual ~stRoomManager();
		
 private:

	bool AddRoom(stBaseRoom * p_room);

	stRoomManager();
	
	static stRoomManager * m_pInstance;

	defRoomMap  m_stRoomMap;

	stRWLock m_stRWLock;
};

#endif
