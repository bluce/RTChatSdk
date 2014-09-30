#ifndef _ST_MTCPSERVICE_H
#define _ST_MTCPSERVICE_H

class stMTCPService
{
 public:
	stMTCPService(std::vector<WORD> & port_vec, std::vector<std::string > & ip_vec);

	~stMTCPService();

	bool Init();

	int Accept(std::vector<struct sockaddr_in> & client_addr, std::vector<int> & sock_vec, std::vector<WORD> & port_vec);

 private:
	static const int MAX_WAITQUEUE = 2000;

	static const int MAX_POLLTIME = 2000;
	
	std::vector<int> m_stSockVec;

	std::vector<WORD> m_stPortVec;
	
	std::vector<std::string> m_stIPVec;
	
	std::map<int, WORD > m_stMap;

	int m_stEpollfd;

	std::vector<struct epoll_event> m_stEpollEvent;
};

#endif
