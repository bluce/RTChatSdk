#ifndef STOBJQUEUE_H
#define STOBJQUEUE_H

#include <queue>
#include "Global.h"

template<typename obj_type, int queue_size = 512> 
class stObjQueue
{
 public:
 	typedef std::pair<volatile bool, obj_type *  > defObjQueue;
 	stObjQueue(): m_dwQueueSize(queue_size)
	{
		ObjQueue.resize(m_dwQueueSize);
		m_dwReadAddr = 0;
		m_dwWriteAddr = 0;
	}

	~stObjQueue()
	{
		Final();
	}

	void Init()
	{
		for(unsigned int i=0; i<m_dwQueueSize; i++)
		{
			obj_type * p_obj = new obj_type();
			p_obj->Reset();
			Put(p_obj);
		}
	}

	bool Put(obj_type * p_obj)
	{
		if(!ObjQueue[m_dwWriteAddr].first)
		{
			p_obj->Reset();
			ObjQueue[m_dwWriteAddr].second = p_obj;
			ObjQueue[m_dwWriteAddr].first = true;
			m_dwWriteAddr = (++m_dwWriteAddr) % m_dwQueueSize;
			return true;
		}
		return false;
	}

 
	obj_type * Get()
	{
		obj_type * p_obj = NULL;
		if(ObjQueue[m_dwReadAddr].first)
		{
			p_obj = ObjQueue[m_dwReadAddr].second;
			ObjQueue[m_dwReadAddr].first = false;
			m_dwReadAddr = (++m_dwReadAddr) % m_dwQueueSize;
			return p_obj;
		}
		Global::logger->fatal("[obj_queue] objqueue get a null command failed");
		return p_obj;
	}

 	void Resize(unsigned int _size)
 	{
		Final();
		m_dwQueueSize = _size;
		ObjQueue.resize(_size);
		Init();
 	}

 private:
	void Final()
	{
		for(unsigned int i=0; i<m_dwQueueSize; i++)
		{
			if(ObjQueue[i].second)
			{
				delete ObjQueue[i].second;
				ObjQueue[i].second = NULL;
				ObjQueue[i].first = false;
			}
		}
		ObjQueue.clear();
	}

 	std::vector<std::pair<volatile bool, obj_type *  > > ObjQueue;
 	unsigned int m_dwQueueSize;
	unsigned int m_dwReadAddr;
	unsigned int m_dwWriteAddr;
};

#endif
