#ifndef _ST_PUBLICFUN_H
#define _ST_PUBLICFUN_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ext/hash_map>
#include <execinfo.h>

#include "Global.h"
#include "VarType.h"
#include "Buffer.h"
#include "SplitString.h"
#include "Encrypt.h"

class PublicFun
{
 public:
	static void ArgParse(const int argc, char ** argv);

	static int GetRandNum(int min, int max);

	static const DWORD DayToSec(const char * day);
};

#endif
