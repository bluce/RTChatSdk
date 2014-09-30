#ifndef STGATETASK_H
#define STGATETASK_H

#include "ServiceTask.h"
#include "CommandQueue.h"

class stGateTask: public stServiceTask, public stCmdParse
{
 public:
 	stGateTask(const int sock, const struct sockaddr_in * addr);

	bool SendCmd(const char * p_send_cmd, const int cmd_size);

	bool SendCmd(defTempId temp_id, defCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size);
		
	bool CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);

	bool CmdParseQueue(const Cmd::stNullCmd * pt_null_cmd, const unsigned int cmd_size);

	int RecycleConn();

	void AddTaskToManager();

	void RemoveFromManager();

	void SetTaskId(const WORD & task_id);

	const WORD GetTaskId();

	bool IsInit();

 private:
	
	WORD m_wdTaskId;
	
	bool m_blInit;
};

#endif
