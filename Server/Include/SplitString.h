#ifndef _ST_SPLITSTRING_H
#define _ST_SPLITSTRING_H

#include <vector>
#include "Share.h"

class stSplitString
{
 public:
	typedef __gnu_cxx::hash_map<std::string, std::string, str_hash> defSplitMap;
	typedef defSplitMap::iterator defSplitMapIter;
	typedef defSplitMap::value_type defSplitMapValue;

	stSplitString()
	{
		
	}

	const char * operator[](const char * key)
	{
		return m_SplitMap[key].c_str();
	}
	
	bool Init(const char * split_string, const char * split = "\t\n");

	bool InitXML(const char * split_string, const char * split = " ");
 private:
  	void StringSplit(std::vector<std::string> & string_container, const char * in_string, const char * split="\t\n");
	
	defSplitMap m_SplitMap;
};

#endif
