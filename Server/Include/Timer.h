#ifndef _ST_TIMER_H
#define _ST_TIMER_H

#include <string>
#include <map>
#include <time.h>
#include <sys/time.h>

#include "Type.h"
#include "Mutex.h"


class stZTime
{
 public:
	static void GetSysZone(std::string & zone_name);

	static void SaveSysZone(std::string & zone_name);

	static void ReloadSysZone(const std::string & zone_name);

	static void GetLocalTime(struct tm & tm_time, time_t & time_value);

	static long m_SysZoneTime;
};


class stRTime
{
 public:
	stRTime(const unsigned int msecond = 0);
	
	~stRTime();
	
	void GetNow();
	
	void SetDelay(unsigned int msecond);

	void Delay(unsigned int msecond);

	stRTime & operator= (const stRTime & timer);

	stRTime & operator= (const unsigned long long msec);

	bool operator > (const stRTime & timer) const;
	
	bool operator >= (const stRTime & timer) const;

	bool operator < (const stRTime & timer) const;

	bool operator <= (const stRTime & timer) const;

	bool operator == (const stRTime & timer) const;

	const unsigned long GetSec() const;

	const unsigned long long GetMSec() const;

 private:
	unsigned long long now();
	
	unsigned long long m_msec;
};

class stTTime
{
 public:
	stTTime();

	stTTime(const stTTime & mdtime);

	stTTime & operator= (const unsigned int & rt);
	
	stTTime & operator= (const stRTime &rt);

	bool operator > (const stTTime &rt) const;

	bool operator >= (const stTTime &rt) const;

	bool operator < (const stTTime &rt) const;

	bool operator <= (const stTTime &rt) const;

	bool operator == (const stTTime &rt) const;
	
	void GetNow();

   	const unsigned int Elapse() ;
	
	time_t GetSec() const;

	const int GetMin() const ;

	const int GetHour() const ;

	const int GetMDay() const ;

	const int GetWDay() const ;

	const int GetMon() const ;

	const int GetYear() const ;

	const time_t  GetTimet() const;

	const char * GetStrTime();

 private:
	time_t m_secs;

	struct tm m_tmtime;
};

class stTimer
{
 public:
	virtual ~stTimer(){}

	stTimer(){}

	stTimer(const unsigned int delay_time, const stRTime & cur_rtime);

	stTimer(const unsigned int delay_time, const unsigned int cur_sec = 0);
		
	void Reload(const unsigned int delay_time, const stRTime & cur_rtime);

	bool operator() (const stRTime & cur_rtime);

	bool operator() (const QWORD & cur_rtime);
	
	bool IsExpirs(const stRTime & cur_rtime);

	virtual void Trigger(stRTime & cur_time){}
	
	virtual bool IsOver(){return false;}

 private:
	unsigned int m_dwDelay;

	stRTime m_stRTime;
};



template<class T>
class stGameTimer: public stTimer
{
 public:
	stGameTimer(T* pThis, int (T::*fCallBack)(std::string&), std::string &para, unsigned int uiInterval, stRTime & cur_time,
				int iMaxTimes);

	~stGameTimer(){}

	bool IsOver();

	void Trigger(stRTime & cur_time);

 private:
	int (T::*m_fCallBack)(std::string &);
	T * m_pOwn;
	std::string m_strPara;
	int m_CurrentTimes;
	int m_MaxTimes;
};


template<class T>
class stGameTimerMgr
{
 public:
	stGameTimerMgr()
	{
		m_dwTimerId = 0;
	}
	
	~stGameTimerMgr(){}

	DWORD AddTimer(T* pThis, int (T::*fCallBack)(std::string&), std::string &para,
				  unsigned int uiInterval, stRTime & cur_time, int iMaxTimes = 1);

	void RemoveTimer(DWORD timer_id);
	
	void Update(stRTime & cur_time);

 private:
	std::map<DWORD,  stTimer * >  m_Timers;
	DWORD m_dwTimerId;
};


template<class T>
bool stGameTimer<T>::IsOver()
{
	return m_CurrentTimes >= m_MaxTimes;
}

template<class T >
void stGameTimer<T>::Trigger(stRTime & cur_time)
{
	if(IsExpirs(cur_time))
	{
		m_CurrentTimes++;
		if(m_pOwn)
		{
			(m_pOwn->*m_fCallBack)(m_strPara);
		}
	}
}

template<class T>
stGameTimer<T>::stGameTimer(T* pThis, int (T::*fCallBack)(std::string&), std::string &para,
							unsigned int uiInterval, stRTime & cur_time, int iMaxTimes)
{
	m_pOwn = pThis;
	m_fCallBack = fCallBack;
	m_strPara = para;
	Reload(uiInterval, cur_time);
	m_MaxTimes = iMaxTimes;
	m_CurrentTimes = 0;
}


template<class T>
DWORD stGameTimerMgr<T>::AddTimer(T* pThis, int (T::*fCallBack)(std::string&), std::string &para,
				  unsigned int uiInterval, stRTime & cur_time, int iMaxTimes)
{
	stGameTimer<T> *pTimer = new stGameTimer<T>(pThis, fCallBack, para, uiInterval, cur_time, iMaxTimes);
	m_dwTimerId++;
	m_Timers[m_dwTimerId] = pTimer;
	return m_dwTimerId;
}


template<class T>
void stGameTimerMgr<T>::RemoveTimer(DWORD timer_id)
{
	std::map<unsigned int, stTimer * >::iterator iter = m_Timers.find(timer_id);
	if(iter != m_Timers.end())
	{
		delete iter->second;
		iter->second = NULL;
		m_Timers.erase(iter);
	}
}


template<class T>
void stGameTimerMgr<T>::Update(stRTime & cur_time)
{
	if (!m_Timers.empty())
	{
		std::map<unsigned int, stTimer * >::iterator iter,next;
		next = m_Timers.begin();
		for(iter = next; iter != m_Timers.end(); iter = next)
		{
			if(next != m_Timers.end())
			{
				next++;
			}
			if(iter->second != NULL)
			{
				iter->second->Trigger(cur_time);

				if(iter->second->IsOver())
				{
					delete iter->second;
					iter->second = NULL;
					m_Timers.erase(iter);
				}
			}
		}
	}
}




class stTimerEqu
{
 public:
	stTimerEqu(const int min = 0,  const int hour = 0);
	
	bool operator() (const stTTime & cur_ttime);
	
 private:
	bool m_blIsEqu;
	
	int m_wdMin;
	
	int m_wdHour;
};

#endif
