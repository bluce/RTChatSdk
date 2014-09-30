#ifndef _ST_RWLOCK_H
#define _ST_RWLOCK_H

#include <pthread.h>
#include "Type.h"


class stRWLock : private stDisableCopy
{
 public:
	stRWLock()
	{
		::pthread_rwlock_init(&m_rwlock, NULL);
	}

	~stRWLock()
	{
		::pthread_rwlock_destroy(&m_rwlock);
	}
	
	inline int rdlock()
	{
		int ret_code = 0;
		ret_code = ::pthread_rwlock_rdlock(&m_rwlock);
		return ret_code;
	}
	

	inline int wrlock()
	{
		int ret_code = 0;
		ret_code = ::pthread_rwlock_wrlock(&m_rwlock);
		return ret_code;
	}

	
	inline int unlock()
	{
		int ret_code = 0;
		ret_code = ::pthread_rwlock_unlock(&m_rwlock);
		return ret_code;
	}

 private:

	pthread_rwlock_t m_rwlock;
	
};


class stAutoRDLock: public stDisableCopy
{
 public:

	stAutoRDLock(stRWLock & lock): m_stRWLock(lock)
	{
		m_stRWLock.rdlock();
	}
		
	~stAutoRDLock()
	{
		m_stRWLock.unlock();
	}
	
 private:

	stRWLock & m_stRWLock;
};


class stAutoWRLock: public stDisableCopy
{
 public:

	stAutoWRLock(stRWLock & lock): m_stRWLock(lock)
	{
		m_stRWLock.wrlock();
	}
	
	~stAutoWRLock()
	{
		m_stRWLock.unlock();
	}
	
 private:

	stRWLock & m_stRWLock;
};

#endif

