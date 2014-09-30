#ifndef _ST_FUNCTIONTIME_H
#define _ST_FUNCTIONTIME_H

#include "Global.h"

class stFunctionTime
{
public:
	
	stFunctionTime(const unsigned long warning_time, const char * fun_name = "function_time"): m_qwLoggerTime(warning_time),
		m_strFunName(fun_name)
	{
		clock_gettime(CLOCK_REALTIME, &m_stBeginTime);
	}
		
	~stFunctionTime()
	{
		clock_gettime(CLOCK_REALTIME, &m_stEndTime);
		
		unsigned long end=m_stEndTime.tv_sec*1000000L + m_stEndTime.tv_nsec/1000L;
		unsigned long begin= m_stBeginTime.tv_sec*1000000L + m_stBeginTime.tv_nsec/1000L;
		if((end - begin) > m_qwLoggerTime)
		{
			Global::logger->warn("[warn_time] running time warning <thread=%s,time=%lu>", m_strFunName.c_str(), (end - begin));
		}
	}
	
private:
	struct timespec m_stBeginTime;
	
	struct timespec m_stEndTime;
	
	const unsigned long m_qwLoggerTime;
	
	std::string m_strFunName;
};

#endif
