#ifndef _ST_COND_H
#define _ST_COND_H

#include <pthread.h>
#include "Type.h"

class stMutex;

class stCond: public stDisableCopy
{
 public:
	stCond();

	~stCond();

	int broadcast();

   	int signal();

	int wait(stMutex & md_mutex);
	
	int timedwait(stMutex & md_mutex, int timedwait);
	
 private:

	::pthread_cond_t m_cond;
};

#endif
