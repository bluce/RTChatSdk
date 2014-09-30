#ifndef _ST_SOCKET_H
#define _ST_SOCKET_H

#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <zlib.h>

#include "Global.h"
#include "Buffer.h"
#include "AssertEx.h"
#include "Encrypt.h"

namespace Skt
{
   	const unsigned int ZIP_FLAG = 0x01000000;
	const unsigned int RC5_FLAG = 0x02000000;
	const int WAIT_WRITE_TIME = 5000;
	const int WAIT_READ_TIME = 2000;
};


class stSocket: public stDisableCopy
{
 public:
	stSocket(const int s_sock, const struct sockaddr_in * s_addr = NULL);

	~stSocket();

	inline const char * GetIP() const
	{
		return inet_ntoa(m_stAddr.sin_addr);
	}

	inline const DWORD GetIPNum() const
	{
		return ntohl(m_stAddr.sin_addr.s_addr);
	}

	inline const WORD GetPort() const 
	{
		return ntohs(m_stAddr.sin_port);
	}

	inline const int GetSock()
	{
		return m_stSock;
	}
	
	inline const WORD GetServicePort() const
	{
		return ntohs(m_stServiceAddr.sin_port);
	}

	void SetSendTempBufferSize(const unsigned int size);
	
	void SetSendBufferSize(const unsigned int size);

	void SetRecvBufferSize(const unsigned int size);

	void SetEncryptType(stEncrypt::enEncryptType type);
	
	void Init(const int s_sock, const struct sockaddr_in * src_addr);

	void Reset();

	bool SendCmd(const Cmd::stNullCmd * pt_cmd, const int cmd_len, bool is_buffer = false, bool is_zip = false);
	
	bool SendCmdNoPacket(const void * pt_cmd, const int cmd_len, bool is_buffer = false);

	bool SendQueueCmdNoPoll();

	bool SendQueueCmd();

	const int RecvData();

	const int RecvDataNoPoll();

	const DWORD GetCmdFromBuffer(void * pt_cmd);

	void AddEpoll(int epoll_fd, uint32_t evts, void *p_data);

	void DelEpoll(int epoll_fd, uint32_t evts);

	const WORD IsHaveCommand();

	const WORD IsHaveWebCommand();

	const int GetWebSocketCmd(void * pt_cmd);
	
	const int GetWebClientCmd(void * pt_cmd);

	const int IsShakeSvrCommand();

	const int IsShakeClientCommand();
	
 private:

	const WORD MakePacket(const void * pt_cmd, const int cmd_size, char * send_queue, bool iszip = false);

	bool SendQueueData(const void * pt_cmd, const int cmd_size);
	
	const int SendData(const void * pt_cmd, const int cmd_size);

	const int SendDataNoPoll(const void * pt_cmd, const int cmd_size);
	
	int WaitForWrite();

	int WaitForRead();

	void SetFlag(const DWORD set_flag);

	bool IsSet(const DWORD set_flag);

	void ClearFlag(const DWORD clear_flag);

	bool UpDataRecvBuffer(int & ret_value);

	bool SetNonBlock();

	bool IsEncrypt();
	
 private:
	int m_stSock;
	
	struct sockaddr_in m_stAddr;

	struct sockaddr_in m_stServiceAddr;
	
	stMutex m_stMutex;

	DWORD m_dwFlag;

	DWORD m_dwRecvSize;

	stEncrypt m_stEncrypt;

	stByteBuffer m_stRecvBuffer;

	stByteBuffer m_stSendTempBuffer;

	stByteBuffer m_stSendBuffer;

	stByteBuffer m_stZipBuffer;

	stByteBuffer m_stUnzipBuffer;
};

#endif
