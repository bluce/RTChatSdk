#ifndef _STCOMMMANDQUEUE_H
#define _STCOMMMANDQUEUE_H

#include <queue>
#include <ext/pool_allocator.h>
#include <ext/mt_allocator.h>
#include "AssertEx.h"
#include "Global.h"
#include "ObjQueue.h"

const unsigned int CMD_BUF_LENGTH = 256;
struct stCmdBuf
{
	void Reset()
	{
		memset(m_byCmdData, 0, sizeof(m_byCmdData));
	}
	
	BYTE m_byCmdData[CMD_BUF_LENGTH];
};

typedef stObjQueue<stCmdBuf, 128> defCmdBuf;

typedef std::pair<int, unsigned char * > defCmdPair;
	
typedef std::pair<volatile bool, defCmdPair > defCmdQueue;

template <unsigned int dwQueueSize=2048 >
class stCmdQueue
{
 public:
	stCmdQueue()
	{
		m_dwReadAddr = 0;
		m_dwWriteAddr = 0;
		m_stCmdBuf.Init();
	}

	~stCmdQueue()
	{
		Final();
	}

 	int Put(const void * cmd_data, const unsigned int real_size);
 
 	defCmdPair * Get();

 	void Erase();

	void Resize(unsigned int _size)
 	{
	 	m_stCmdBuf.Resize(_size);
 	}
 
 private:

 	bool MoveCmdToQueue();

 	void Final();

 private:

	__gnu_cxx::__mt_alloc<unsigned char > __mt_alloc;

	defCmdQueue CmdQueue[dwQueueSize];

	std::queue<defCmdPair, std::deque<defCmdPair > > CmdDeque;

	unsigned int m_dwReadAddr;

	unsigned int m_dwWriteAddr;

 	defCmdBuf m_stCmdBuf;
};


class stCmdParse
{
 protected:
	stCmdParse()
	{
		m_dwDelay = 0;
		m_dwCurrent = 0;
	}
		
	virtual ~stCmdParse(){}
 public:

	int Put(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);
	
	virtual bool CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size) = 0;

	void Get(const WORD default_get_count );

	void Resize(unsigned int _size)
	{
		m_stCmdQueue.Resize(_size);
	}
	
 private:

	stCmdQueue<> m_stCmdQueue;
	
	stRTime m_stTime;
	
	DWORD m_dwDelay;
	
	DWORD m_dwCurrent;
};

#endif
