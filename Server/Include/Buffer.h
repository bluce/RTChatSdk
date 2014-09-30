#ifndef _STBYTEBUFFER_H
#define _STBYTEBUFFER_H

#include "BaseOS.h"
#include "Global.h"
#include <unistd.h>

class stByteBuffer
{
 public:
	stByteBuffer(const unsigned int buffer_length = (Skt::MAX_SOCKETSIZE * 2));
		
	~stByteBuffer();
	
	const unsigned int ReadAddr();
	
	const unsigned int WriteAddr();

	const unsigned int Length();

	unsigned char * WriteBuf();

	unsigned int WriteSize();
	
	bool ResizeBuffer(const unsigned int size);

	bool Write(const unsigned char * write_data, const unsigned int write_len);

	bool WriteSuccess(const unsigned int size);
	
	bool IsCanRead();

	unsigned int ReadSize();
		
	unsigned char * ReadBuf();
	
	void ReadSuccess(const unsigned int size);

	void Reset();
 private:
	unsigned int m_dwAddLength;
	const unsigned int m_dwMaxLength;
	unsigned int m_dwReadAddr;
	unsigned int m_dwWriteAddr;
	unsigned int m_dwBufLength;
	std::vector<unsigned char > m_DataVec;
};

#endif
