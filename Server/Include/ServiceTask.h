#ifndef _ST_SERVICETASK_H
#define _ST_SERVICETASK_H

#include "Type.h"
#include "RWLock.h"
#include "Timer.h"
#include "Socket.h"

class stTaskQueue;

class stServiceTask: public stDisableCopy
{
 public:
	enum enTaskState
	{
		enNoUseState,
		enNormalState,
		enRecycleState
	};

	void ClearCheckCount();
	
	BYTE GetCheckCount();

	void SetState(enTaskState  state);

	const enTaskState & GetState() const;
	
	void GetNextState();

	void SetInBuf();

	bool IsInBuf();

	void SetWebSocket();

	bool IsWebSocket();

	void SetShakeHand();

	bool IsShakeHand();

	void SetTimeOut();

	void ResetLiftTime();
	
	void Terminate();

	bool IsTerminate();

	void TerminateFinal();

	bool IsTerminateFinal();

	stServiceTask(stTaskQueue * p_queue, bool is_websocket = false);

	stServiceTask(const int sock, const struct sockaddr_in * addr, bool is_websocket = false);

	virtual void Init(const int s_sock, const struct sockaddr_in * s_addr, stTaskQueue * task_queue, bool is_websocket = false);

	virtual void Reset();

	virtual ~stServiceTask();

	const char * GetIP() const;
	
	const WORD GetPort() const;

	const DWORD GetIPNum() const;

	const WORD GetServicePort() const;
	
   	int CheckVerify();

	virtual int WaitSync();
		
	virtual int RecycleConn() = 0;

	virtual void DisConnect();
	
	virtual void AddTaskToManager();

	virtual void RemoveFromManager();
	
	bool CheckTimeOut(const stRTime & real_time, const DWORD delay_mses = 8000);

	void EnableCheckFlux();

	bool IsCheckFlux();

	bool CheckConnTime(const stRTime & current_time);

	virtual void CheckConn();
	
	bool IsEnableCheck();
	
	void SetEnableCheck();

	bool IsAddRead();

	void SetAddRead();

	void AddRecvCount();

	const DWORD GetRecvCount();

	void ClearRecvCount();
	
	bool SendCmd(const char * p_send_cmd, const int cmd_size, bool is_zip = false);
	
	bool SendWebData(const char * p_send_cmd, const int cmd_size, bool is_zip = false);

	bool SendCmdNoBuffer(const Cmd::stNullCmd * p_send_cmd, const int cmd_size, bool is_zip = false);

	virtual bool RecvDataNoPoll();

	bool SendDataNoPoll();

	void AddEpoll(int epoll_fd, uint32_t evts, void * p_data);

	void DelEpoll(int epoll_fd, uint32_t evts);

	void AddRecycleQueue();
	
 protected:

	stSocket * m_pSocket;

	virtual bool CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size) = 0;

	virtual int CheckVerifyCmd(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);

	stRTime m_stLiftTime;

	stRTime m_stCheckConnTime;

	BYTE m_byCheckCount;

	bool m_blEnableCheck;

	bool m_blCheckTimeOut;
	
	bool m_blCheckCmdFlux;

	enTaskState m_enTaskState;
	
 private:
	bool m_blTerminate;

	stRWLock m_stTLock;
	
	bool m_blTerminateFinal;

	bool m_blBuffer;

	bool m_blAddRead;

	DWORD m_dwRecvCmdCount;

	bool m_blIsInBuf;

	stTaskQueue * m_pTaskQueue;

	bool m_blIsWebSocket;

	bool m_blIsShakeHand;
};

#endif
