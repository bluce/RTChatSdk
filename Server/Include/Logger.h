#ifndef _ST_LOGGER_H_
#define _ST_LOGGER_H_

#include <log4cxx/logger.h>
#include <log4cxx/rollingfileappender.h>
#include <log4cxx/helpers/simpledateformat.h>
#include <apr-1/apr_time.h>

#include "Type.h"
#include "Mutex.h"

class stLocalFileAppender: public log4cxx::rolling::RollingFileAppenderSkeleton
{
 public:
	stLocalFileAppender()
	{
		m_pDataFormat = NULL;
	}

	~stLocalFileAppender()
	{
		if(m_pDataFormat)
			delete m_pDataFormat;
		m_pDataFormat = NULL;
	}

   	void subAppend(const log4cxx::spi::LoggingEventPtr& event, log4cxx::helpers::Pool & p);

	void RollOver();

	void SetFile(const log4cxx::LogString & file)
	{
		m_strFileName = file;
	}
	
	void activateOptions();
	
 private:	
	
	apr_time_t m_stNowTime;

	apr_time_t m_stCheckTime;

	log4cxx::LogString m_strScheduledFileName;
	
	log4cxx::LogString m_strFileName;

	log4cxx::LogString m_strDatePattern;

	log4cxx::helpers::SimpleDateFormat * m_pDataFormat;
};


class stLogger
{
 public:
	stLogger(const log4cxx::LogString & log_name = "Cube");
	
	~stLogger()
	{
		
	}

	void AddConsoleLog();

	void RemoveConsoleLog();

	void AddLocalFileLog(const log4cxx::LogString & file);

	void SetLevel(const std::string & set_level);

	void fatal(const char * pattern, ...);
	
	void error(const char * pattern, ...);
	
	void warn(const char * pattern, ...);
	
	void info(const char * pattern, ...);
	
	void debug(const char * pattern, ...);
	
 private:

	log4cxx::Logger * m_ptLogger;

	char m_cstrMessage[MAX_LOGGERSIZE];

	stMutex m_stMsgMutex;
};

#endif
