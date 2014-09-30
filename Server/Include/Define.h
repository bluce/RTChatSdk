#ifndef _ST_DEFINE_H
#define _ST_DEFINE_H

#include "BaseOS.h"

typedef	QWORD defRoomId;

typedef	QWORD defTempId;

typedef QWORD defTokenId;

typedef WORD defCmdId;

typedef WORD defInternalId;

typedef DWORD defCmdLen;

typedef QWORD defSSRCId;

const BYTE UNIQUE_LEN = 33;

const WORD MAX_LOGGERSIZE = 4096;

const BYTE MAX_NAMESIZE = 32;

const BYTE MAX_ACCNAMESIZE = 48;

const BYTE MAX_IPSIZE = 16;

const BYTE MIN_ZIPSIZE = 64;

const DWORD CMD_FLUX_PER_SECOND = 100;

const DWORD SVR_CMDSIZE = 40960;

namespace Skt
{
	const DWORD MAX_SOCKETSIZE = 64 * 1024;

	const BYTE PACKET_HEAD_LEN = sizeof(DWORD);
	
	const DWORD MAX_PACKETSIZE = MAX_SOCKETSIZE - PACKET_HEAD_LEN;

	const DWORD MAX_USERDATASIZE = MAX_PACKETSIZE - 128;
};

#endif
