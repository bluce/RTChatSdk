#pragma once
#include "public.h"
#include "zThread.h"
#include <vector>

//class RecvMessage
//{
//public:
//    std::vector<BYTE>content;
//    int preSize;
//    int messageSize;
//public:
//    RecvMessage()
//    {
//        ok = false;
//		preSize = 0;
//		messageSize = 0;
//		content.resize(256) ;
//    }
//    void setSize(DWORD size)
//    {
//		if (size > 10240 || size <= 0 ) 
//			return;
//        if (content.size() < size)
//        	content.resize(size);
//        messageSize = size;
//    }
//    bool ok;
//    void append(char **ptr, int& size)
//    { 
//        char *tmp = *ptr;
//        int copySize = (size >= messageSize - preSize)?messageSize - preSize : size;
//        bcopy(tmp,&content[preSize],copySize);
//        preSize += copySize;
//        if (preSize == messageSize)
//            ok = true;
//        (*ptr) += copySize;
//        size -= copySize;
//    }
//    void clear()
//    {
//        ok = false;
//	preSize = 0;
//	messageSize = 0;
////	content.resize(0);	    
//    }	
//};
//
//class MessageRecord{
//public:
//    RecvMessage header;
//    RecvMessage body;
//	bool valid;
//	MessageRecord()
//	{
//		valid = true;
//	}
//public:
//
//	bool Process(StreamConnection *connection);
//};

class StreamConnection;

class ClientTask : public zThread
{
public:

	ClientTask( StreamConnection *pConnection );
	~ClientTask();

	virtual void run();

	void Stop()
	{
//		_record.valid = false;

		join();
	}

private:
	StreamConnection*       _client;
//	MessageRecord           _record;
};