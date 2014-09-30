#ifndef STINFO_CLIENT_H_
#define STINFO_CLIENT_H_

#include "internal.pb.h"
#include "ClientTask.h"
#include "CommandQueue.h"


class stInfoClient : public stClientTask
{
public:

	static stInfoClient & GetInstance()
	{
		if(m_pInstance == NULL)
		{
			m_pInstance = new stInfoClient("infoclient", Global::g_strInfoGateIP.c_str(), Global::g_wdInfoGatePort);
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

	stInfoClient(const char * name, const char * server_ip,const WORD & port);

	~stInfoClient();

	bool SendCmd(defTempId temp_id, Cmd::enInternalId cmd_id, const ::google::protobuf::Message * pt_null_cmd, const defCmdLen cmd_size);

	bool SendCmd(const char * pt_cmd, const unsigned int cmd_size);

	virtual bool CmdParse(const Cmd::stNullCmd * pt_null_cmd, const int cmd_size);

	virtual	bool Init();

	virtual void Final();

 private:
	static stInfoClient * m_pInstance;
};


#endif
