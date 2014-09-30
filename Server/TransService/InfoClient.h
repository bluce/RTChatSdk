#ifndef ST_INFOTASK_H
#define ST_INFOTASK_H

#include "Global.h"
#include "RClientTask.h"
#include "CommandQueue.h"
#include "internal.pb.h"
#include "game.pb.h"


class stInfoClient: public stRClientTask
{
public:
	stInfoClient(const char * name, const char * server_ip,const WORD & port): stRClientTask(name, server_ip, port), m_strClientName(name)
	{
	}

	static stInfoClient & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stInfoClient("infoclient", Global::g_strInfoTransIP.c_str(), Global::g_wdInfoTransPort);
		}
		return *  m_pInstance;
	}

	static void DelInstance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
		}
		m_pInstance = NULL;
	}
	
	bool Init();

	const bool IsConnected();

	bool SendCmd(defTempId temp_id, Cmd::enCmdId cmd_id, const ::google::protobuf::Message * pt_null_cmd,const defCmdLen cmd_size);

	bool SendCmd(const char * cmd, const defCmdLen cmd_len);

	bool CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);

 private:
	
	static stInfoClient * m_pInstance;
	
	virtual ~stInfoClient()
	{
		ClientFinal();
	}
	
	void ClientFinal();

	std::string m_strClientName;

};


#endif
