#ifndef _ST_TYPE_H
#define _ST_TYPE_H

#include "BaseOS.h"
#include "Define.h"
#include "NullCmd.h"

class stDisableCopy
{
 protected:
	stDisableCopy(){}
	~stDisableCopy(){}
 private:
	stDisableCopy(const stDisableCopy & class_data);
	const stDisableCopy & operator= (const stDisableCopy & class_data);
};


#ifdef _LINUX

#include <ext/hash_map>

struct str_hash: public std::unary_function<const std::string, size_t>
{
	const size_t operator()(const std::string & str) const 
	{
		return __gnu_cxx::__stl_hash_string(str.c_str());
	}
};

#endif

#endif

