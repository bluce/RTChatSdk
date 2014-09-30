#ifndef _ST_SERVICETASKSCHED_H
#define _ST_SERVICETASKSCHED_H

#include "Global.h"
#include "NormalSched.h"
#include "RecycleSched.h"

class stServiceTaskSched: public stDisableCopy
{
 public:
	stServiceTaskSched(const WORD normal_thread_num = 1,
						  const WORD recycle_thread_num = 1):
	   	m_stNormalSched(this, normal_thread_num),
	   	m_stRecycleSched(this, recycle_thread_num)
	{
		
	}

	bool Init();

	void Final();
	
	int Size();

	int GetNormalSize();

	int GetRecycleSize();

	bool AddNormalSched(stServiceTask * p_task);

	bool AddRecycleSched(stServiceTask * p_task);

 private:
   	stNormalSched m_stNormalSched;

   	stRecycleSched m_stRecycleSched;
};

#endif


