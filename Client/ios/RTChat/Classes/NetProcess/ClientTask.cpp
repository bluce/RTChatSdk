#include "ClientTask.h"
#include <sys/socket.h>
#include <sys/types.h>
#include "StreamConnection.h"
#include "rtpsession.h"

using namespace jrtplib;

void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}

//bool MessageRecord::Process(NetConnection *connection)
//{
//	char buf[256] = {0};
//	while (valid)
//	{
//        
//		int leftSize = ::recv(connection->GetSocket(),buf,256, 0 );
//		if (leftSize == -1) 
//			continue;
//		char *nowPtr = buf;  
//		while (leftSize)
//		{
//			if (!header.ok)
//			{
//				header.setSize(sizeof(stHeader));
//				header.append(&nowPtr,leftSize);
//			}
//			if (header.ok)
//			{
//				stHeader head = *(stHeader*) (&header.content[0]);
//				body.setSize(head.bodySize);
//				body.append(&nowPtr,leftSize);
//				if (body.ok)
//				{
//					connection->OnReceiveMsg(&body.content[0], body.messageSize);
//					header.clear();
//					body.clear();
//				}
//				else
//				{
//					break;
//				}
//			}
//			else
//			{
//				break;
//			}
//		}
//
//		::sleep( 1 );
//	}
//	return true;
//}

ClientTask::ClientTask( StreamConnection *pConnection )
:_client(pConnection)
{

}
ClientTask::~ClientTask()
{
	Stop();
}

void ClientTask::run()
{
    int status;
    while (1) {
        RTPSession& session = _client->get_rtpSession();
        if (session.GotoFirstSourceWithData()) {
            do
			{
				RTPPacket *pack;
				
				while ((pack = session.GetNextPacket()) != NULL)
				{
					// You can examine the data here
					printf("Got packet !\n");
                    
                    _client->OnReceiveMsg();
					
					// we don't longer need the packet, so
					// we'll delete it
					session.DeletePacket(pack);
				}
			} while (session.GotoNextSourceWithData());
        }
        
        session.EndDataAccess();
        
        status = session.Poll();
        
        checkerror(status);
        
        RTPTime::Wait(RTPTime(1,0));
    }
//	_record.Process(_client);
}
