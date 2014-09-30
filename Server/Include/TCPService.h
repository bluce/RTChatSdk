#ifndef _ST_TCPSERVICE_H
#define _ST_TCPSERVICE_H

class stTCPService
{
 public:
	stTCPService(const WORD & port, const char * service_ip);

	~stTCPService();

	bool Init();

	int Accept(struct sockaddr_in * client_addr);

	const WORD & GetPort() const
	{
		return m_wdPort;
	}
	
 private:

	static const int MAX_WAITQUEUE = 2000;

	static const int MAX_POLLTIME = 2000;
	
	int m_stSock;

	WORD m_wdPort;

	std::string m_strServiceIP;
	int m_stEpollfd;
};

#endif
