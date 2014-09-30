/**
 * @file ThreadGroup.h
 * @date 2010-01-26 CST
 * @version $Id $
 * @author hurixin hurixing2008@gmail.com
 * @brief 线程组管理
 *
 */

#ifndef _MDTHREADGROUP_H
#define _MDTHREADGROUP_H

#include<vector>
#include "Thread.h"

/**
 * @brief 线程回调
 * 
 */
struct stThreadCallBack
{
	virtual ~stThreadCallBack(){}
	virtual bool Exec(stThread *) = 0;
};


/**
 * @brief 线程组管理
 *
 */
class stThreadGroup
{
 public:
	/** 
	 * @brief 构造
	 * 
	 * @param count 线程组的大小
	 */
	stThreadGroup(const WORD count = 1): m_wdCount(count)
	{
	}
	
	~stThreadGroup()
	{
		DelAllThread();
	}
	
   	///增加一个线程到组里面
	void AddThread(stThread * p_thread)
	{
		if(m_stThreadVec.size() < m_wdCount)
		{
			m_stThreadVec.push_back(p_thread);
		}
		else
		{
			//Global::logger->fatal(LOGINFO_THREADINFO5, SYS_INIT);
		}
	}

	/// 删除所以线程
	void DelAllThread()
	{
		while(! m_stThreadVec.empty())
		{
			stThread * p_thread = m_stThreadVec.back();
			m_stThreadVec.pop_back();
			if(p_thread)
			{
				p_thread->TTerminate();
				if(p_thread->Join() == 0)
				{
					delete p_thread;
					p_thread = NULL;
				}
			}
		}
	}

	/** 
	 * @brief 获取设定的个数
	 * 
	 * @return 
	 */
	const WORD & GetCount() const 
	{
		return m_wdCount;
	}

	/// 重载
	stThread * operator[] (const int index)
	{
		if(index >= (int)m_stThreadVec.size())
		{
			return NULL;
		}
		return m_stThreadVec[index];
	}

	/** 
	 * @brief 所以线程回调
	 * 
	 * @param call_back 回调函数
	 * 
	 * @return 所有成功true
	 */
	bool ExecAllThread(stThreadCallBack & call_back)
	{
		std::vector<stThread * >::iterator iter = m_stThreadVec.begin();
		for(; iter != m_stThreadVec.end(); iter++)
		{
			if(! call_back.Exec(*iter))
			{
				return false;
			}
		}
		return true;
	}
	
 private:
	/// 线程个数
	WORD m_wdCount;
	/// 线程组
	std::vector< stThread * > m_stThreadVec;
};

#endif
