#ifndef _ST_MUTE_H_
#define _ST_MUTE_H_

#include <pthread.h>
#include <iostream>

class stCond;

class stMutex : private stDisableCopy
{
	friend class stCond;
 public:

	stMutex(int kind = PTHREAD_MUTEX_FAST_NP)
	{
		pthread_mutexattr_t attr;
		::pthread_mutexattr_init(&attr);
		::pthread_mutexattr_settype(&attr, kind);
		::pthread_mutex_init(&m_mutex, &attr);
	}

	~stMutex()
	{
		::pthread_mutex_destroy(&m_mutex);
	}

	inline int lock()
	{
		int ret_code = 0;
		ret_code = ::pthread_mutex_lock(&m_mutex);
		return ret_code;
	}

	inline int unlock()
	{
		int ret_code = 0;
		ret_code = ::pthread_mutex_unlock(&m_mutex);
		return ret_code;
	}
	
 private:

	pthread_mutex_t m_mutex;
};

#endif

