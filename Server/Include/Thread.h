#ifndef _ST_THREAD_H
#define _ST_THREAD_H

#include <pthread.h>
#include "Type.h"
#include "Mutex.h"
#include "Cond.h"


class stThread
{
 public:
	stThread(const char * name = "Thread");
	
	virtual ~stThread();

	const bool IsAlive() const;
	
	static void * InitThread(void * new_thread);

	bool Start();

	void TTerminate();

	const bool IsTTerminate() const ;

	virtual void Run() = 0;
	
	const char * GetThreadName() const;

	int Join();
	
 private:
	std::string m_strThreadName;
	
	stMutex m_stLock;

	stCond m_stCond;

	volatile bool m_blALive;

	volatile bool m_blTerminate;

	pthread_t m_stThread;			
};

#endif
