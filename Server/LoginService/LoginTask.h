#ifndef ST_LOGIN_TASK_H
#define ST_LOGIN_TASK_H

#include "ServiceTask.h"
#include "CommandQueue.h"

class stLoginTask: public stServiceTask
{
 public:
	stLoginTask(const int sock, const struct sockaddr_in * addr, const QWORD _id);

	bool SendCmd(defCmdId cmd_id, const ::google::protobuf::Message * send_cmd, const defCmdLen cmd_size);

	bool CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);

	int RecycleConn();
};

#endif
