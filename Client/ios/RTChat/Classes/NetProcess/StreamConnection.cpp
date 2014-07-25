//
//  StreamConnection.cpp
//  RTChat
//
//  Created by raymon_wang on 14-7-11.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "StreamConnection.h"
#include "rtpipv4address.h"
#include "rtpudpv4transmitter.h"
#include "rtpsessionparams.h"
#include "ClientTask.h"

void checkerror(int rtperr)
{
	if (rtperr < 0)
	{
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}

static StreamConnection* s_streamConnection = NULL;

StreamConnection::StreamConnection() :
_clientTask(NULL)
{
    s_streamConnection = this;
}

StreamConnection::~StreamConnection()
{
    
}

bool StreamConnection::initConnection(const char *destipstr, uint16_t destport)
{
    int status;
    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;
    DWORD destip;
    
    destip = inet_addr(destipstr);
	if (destip == INADDR_NONE)
	{
		std::cerr << "Bad IP address specified" << std::endl;
		return -1;
	}
    
    destip = ntohl(destip);
    
    // IMPORTANT: The local timestamp unit MUST be set, otherwise
	//            RTCP Sender Report info will be calculated wrong
	// In this case, we'll be sending 10 samples each second, so we'll
	// put the timestamp unit to (1.0/10.0)
    sessparams.SetOwnTimestampUnit(1.0/8000.0);
    
    sessparams.SetAcceptOwnPackets(true);
    
    transparams.SetPortbase(10070);
    
    status = _session.Create(sessparams ,&transparams);
    
    checkerror(status);
    
    RTPIPv4Address addr(destip,destport);
    
    status = _session.AddDestination(addr);

    checkerror(status);
    
    _clientTask = new ClientTask( this );
    
	_clientTask->start();
    
    return true;
}

bool StreamConnection::reConnect()
{
    return true;
}

void StreamConnection::sendLoginMsg()
{
    char msg[64] = {0};
    snprintf(msg, 63, "lwangxin");
    int status = _session.SendPacket(msg, 8, 0, false, 10);
    checkerror(status);
}

bool StreamConnection::sendData(const char *ptr, int size)
{
    return _session.SendPacket(ptr, size, 0, false, 10);
}

void StreamConnection::OnReceiveMsg()
{
    printf("in OnReceiveMsg");
}

StreamConnection* getStreamConnection()
{
    return s_streamConnection;
}


