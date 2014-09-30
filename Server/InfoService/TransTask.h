#ifndef _ST_TRANSTASK_H
#define _ST_TRANSTASK_H

#include "ServiceTask.h"
#include "CommandQueue.h"
#include "NullCmd.h"

class stTransTask: public stServiceTask, public stCmdParse
{
 public:
 	stTransTask(const int sock, const struct sockaddr_in * addr);

	bool SendCmd(defCmdId cmd_id, const google::protobuf::Message * send_cmd, const defCmdLen cmd_size);

	bool SendCmd(const char * p_send_cmd, const int cmd_size);

	bool CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);

	bool CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size);

	void SetTransId(const WORD & zone_id);

	const WORD GetTransId();

	void AddRoomCount();

	void SubRoomCount();

	DWORD GetRoomCount();

	bool IsInit();

	int RecycleConn();

	void AddTaskToManager();

	void RemoveFromManager();

 private:
	WORD m_wdTransId;

	DWORD m_dwRoomCount;

	bool m_blInit;
};

#endif
