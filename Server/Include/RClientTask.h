#ifndef _STRCLIENTTASK_H
#define _STRCLIENTTASK_H

#include "Type.h"
#include "Thread.h"
#include "Socket.h"
#include "Timer.h"


class stRClientTask: public stDisableCopy, public stThread
{
 public:
	stRClientTask(const char * client_name, const char * server_ip, const WORD & port, bool is_websocket = false);

	virtual ~stRClientTask();

	const char * GetServiceIP() const;

	void SetServiceIP(const char * ip);

	void SetServicePort(const WORD & port);

	const WORD & GetServicePort();

	void Run();

	virtual bool Init();

   	bool SendCmdInConnect(const char * pt_null_cmd, const int cmd_size, bool is_buffer = true);

	bool SendWebData(const char * p_send_cmd, const int cmd_size, bool is_zip = false);

	virtual bool CmdParse(const Cmd::stNullCmd * p_null_cmd, const int cmd_len) = 0;
	
	virtual bool RecvDataNoPoll();
	
	bool SendDataNoPoll();

	const char * GetIP();

	WORD & GetPort();

	bool IsTerminate();

	void Terminate(bool bl_terminate);

	virtual const bool IsConnected();
	
	void SetConnect(bool set_vlaue);
	
	bool Connect();

	virtual void ConnectSuccess();

	virtual void ShakeSuccess();

	void SetWebSocket();

	bool IsWebSocket();

	void SetShakeHand();

	bool IsShakeHand();

 protected:

	stSocket m_stSocket;

	virtual void Final();

	stRTime m_stRTime;

	stRClientTask * m_pMySelf;
	
	void ReadyReConnect();
	
 private:	
	volatile bool m_blConnected;
	
	stMutex m_stLock;
	
	bool m_blTermiate;
	
	bool m_blCanRelease;
	
	std::string m_strServerIP;

	WORD m_wdPort;

	std::string m_strClientIP;
	
	WORD m_wdClientPort;

	int m_iEpollfd;

	std::vector<struct epoll_event > m_stEventVec;
	
	int m_iReadEpollfd;
	
	std::vector<struct epoll_event > m_stReadEventVec;

	bool m_blIsWebSocket;

	bool m_blIsShakeHand;
};

#endif
