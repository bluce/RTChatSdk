#ifndef _ST_STRINGMAP_H
#define _ST_STRINGMAP_H

#include "Type.h"

class stStringMap
{
 public:
	~ stStringMap()
	{
		m_StringVec.clear();
	}
	
	std::string & operator[] (const std::string & key)
	{
		return m_StringVec[key];
	}
 private:
	__gnu_cxx::hash_map<std::string, std::string, str_hash > m_StringVec;
};


#endif
