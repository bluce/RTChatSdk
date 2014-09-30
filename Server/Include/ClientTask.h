#ifndef _STCLIENTTASK_H
#define _STCLIENTTASK_H

#include "Type.h"
#include "Thread.h"
#include "Socket.h"

class stClientTask: public stDisableCopy, public stThread
{
 public:

	stClientTask(const char * client_name, const char * server_ip, const WORD & port);

	~stClientTask();

	const char * GetServiceIP() const
	{
		return m_strServerIP.c_str();
	}

	const WORD & GetServicePort()
	{
		return m_wdPort;
	}

	void Run();

	virtual bool Init();

	bool SendCmd(const char * pt_null_cmd, const int cmd_size, bool is_buffer = false);
	
	bool SendQueueCmd();
	
	virtual bool CmdParse(const Cmd::stNullCmd * p_null_cmd, const int cmd_len) = 0;
	
	bool SendDataNoPoll();
	
	virtual bool RecvDataNoPoll();

	const char * GetIP()
	{
		return m_strClientIP.c_str();
	}

	WORD & GetPort()
	{
		return m_wdClientPort;
	}
	
 protected:
	
	stSocket * m_pSocket;
	
	virtual void Final()
	{
		if(m_pSocket)
		{
			delete m_pSocket;
		}
		m_pSocket = NULL;
	}

	stRTime m_stRTime;
	
 private:	
	
	const std::string m_strServerIP;

	const WORD m_wdPort;

	std::string m_strClientIP;
	
	WORD m_wdClientPort;

	int m_iEpollfd;

	std::vector<struct epoll_event > m_stEventVec;
	
	int m_iReadEpollfd;
	
	std::vector<struct epoll_event > m_stReadEventVec;
};

#endif
