#ifndef _STGLOBAL_H
#define _STGLOBAL_H

#include "Type.h"
#include "Define.h"
#include "Mutex.h"
#include "RWLock.h"
#include "Cond.h"
#include "Timer.h"
#include "Logger.h"
#include "StringMap.h"

namespace Global
{

	extern stStringMap Value;

	extern stStringMap g_stURLMap;

	extern unsigned int __thread g_dwRandNum;
	
	extern stLogger * logger;
	
	extern stLogger * net_logger;

	extern stRTime m_stRTime;

	extern std::string g_strServiceIP;

	extern WORD g_wdServicePort;

	extern std::string g_strLogPath;

	extern WORD g_wdChannelId;

	/// infoserver addr
	extern std::string g_strInfoTransIP;
	extern WORD g_wdInfoTransPort;
	extern std::string g_strInfoGateIP;
	extern WORD g_wdInfoGatePort;
	extern std::string g_strInfoLog;

	/// udp port
	extern unsigned int g_dwMinPort;
	extern unsigned int g_dwMaxPort;
};

extern "C" unsigned int GetChannelIDFromArgs();

#endif
