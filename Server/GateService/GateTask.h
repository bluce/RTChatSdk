#ifndef ST_GATE_TASK_H
#define ST_GATE_TASK_H

#include "ServiceTask.h"

class stGateTask: public stServiceTask
{
 public:
	stGateTask(const int sock, const struct sockaddr_in * addr, const QWORD _id);

	bool SendCmd(defCmdId cmd_id, const ::google::protobuf::Message * send_cmd, const defCmdLen cmd_size);

	bool SendCmd(const char * send_cmd, const unsigned int cmd_len);

	bool SendCmdToInfo(defCmdId cmd_id, const ::google::protobuf::Message * send_cmd, const defCmdLen cmd_size);

	bool SendCmdToInfo(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);
	
	bool CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);

	int RecycleConn();

	void AddTaskToManager();

	void RemoveFromManager();

	QWORD GetTaskId();

	QWORD GetUserId();

	void SetTaskId(QWORD task_id);

	void SetInit(defTempId _id);

	bool IsInit();

 private:
	
	QWORD taskid;

	QWORD userid;

	bool m_blInit;
};

#endif
