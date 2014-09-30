/**
 * @file VarType.h
 * @date 2010-01-04 CST
 * @version $Id $
 * @author hurixin hurixing2008@gmail.com
 * @brief 变量,二进制流变量操作
 *
 */

#ifndef _MDVARTYPE_H
#define _MDVARTYPE_H

#include <vector>
#include <sstream>

/**
 * @brief 变量,二进制流变量操作
 *
 */
class stVarType
{
public:
	/// 够造
	stVarType(): m_DataVec(1, '\0')
	{
		
	}

	/// 拷贝够造	
	stVarType(const stVarType & data)
	{
		m_DataVec = data.m_DataVec;
	}

	/// 赋值够造
	template< typename data_type >
	explicit stVarType(const data_type & value)
	{
		Put(value);
	}

	/// 重载拷贝=
	inline stVarType & operator= (const stVarType & data)
	{
		m_DataVec = data.m_DataVec;
		return (*this);
	}

	/// 重载赋值＝
	template< typename data_type >
	inline stVarType & operator= (const data_type & value)
	{
		Put(value);
		return (*this);
	}

	/**
	 * @brief 赋值
	 *
	 * 二进制流变量必须使用此方法赋值
	 *
	 * @param p_data 数据
	 * @param data_len 数据长度
	 *
	 */
	inline void Put(const void * p_data, const unsigned int data_len)
	{
		m_DataVec.resize(data_len+1);
		if(data_len)
		{
			memcpy(&m_DataVec[0], p_data, data_len);
		}
		m_DataVec[data_len] = '\0';
	}

	/**
	 * @brief 赋值
	 *
	 */
	template <typename data_type >
	inline void Put(const data_type & value)
	{
		std::ostringstream in_value;
		in_value<< value;
		m_DataVec.resize(in_value.str().length() + 1);
		strcpy((char *)(&m_DataVec[0]), in_value.str().c_str());
	}

	/// 不实现
	template <const void *>
	inline void Put(const void * value);

	/// 重载 char *
	inline operator const char * () const
	{
		return ((const char *)(&m_DataVec[0]));
	}

	inline const char * ToStr() const
	{
		return ((const char *)(&m_DataVec[0]));
	}

	/// 重载 *
	inline operator const void * () const 
	{
		return ((const void *)(&m_DataVec[0]));
	}

	/// 重载各变量
	inline operator unsigned char() const
	{
		return atoi((const char *)&m_DataVec[0]);
	}

	inline unsigned char ToUChar() const 
	{
		return atoi((const char *)&m_DataVec[0]);
	}

	inline operator char () const
	{
		return m_DataVec[0];
	}

	inline char ToChar() const
	{
		return m_DataVec[0];
	}

	inline operator unsigned short() const
	{
		return atoi((const char *)(*this));
	}

	inline unsigned short ToUShort() const
	{
		return atoi((const char *)(*this));
	}

	inline short ToShort() const
	{
		return atoi((const char *)(*this));
	}

	inline operator short() const
	{
		return atoi((const char *)(*this));
	}

	inline operator unsigned int() const
	{
		return atoi((const char *)(*this));
	}

	inline unsigned int ToUInt() const
	{
		return atoi((const char *)(*this));
	}

	inline int ToInt() const
	{
		return atoi((const char *)(*this));
	}

	inline operator int() const
	{
		return atoi((const char *)(*this));
	}

	inline float ToFloat() const
	{
		return atof((const char *)(*this));
	}

	inline operator float() const
	{
		return atof((const char *)(*this));
	}

	inline double ToDouble() const
	{
		return atof((const char *)(*this));
	}
	
	inline operator double() const
	{
		return atof((const char *)(*this));
	}

	inline unsigned long ToULong() const
	{
		return strtoul((const char *)(*this), NULL, 10);
	}

	inline operator unsigned long() const
	{
		return strtoul((const char *)(*this), NULL, 10);
	}

	inline long ToLong() const
	{
		return strtol((const char *)(*this), NULL, 10);
	}
	
	inline operator long() const
	{
		return strtol((const char *)(*this), NULL, 10);
	}

	inline unsigned long long ToULLong() const
	{
		return strtoull((const char *)(*this), NULL, 10);
	}
	
	inline operator unsigned long long() const
	{
		return strtoull((const char *)(*this), NULL, 10);
	}


	inline long long ToLLong() const
	{
		return strtoll((const char *)(*this), NULL, 10);
	}
	
	inline operator long long() const
	{
		return strtoll((const char *)(*this), NULL, 10);
	}

	inline const unsigned int  Size() const
	{
		if(m_DataVec.empty())
			return 0;
		else
			return m_DataVec.size() - 1;
	}

	inline bool Empty() const
	{
		return ((m_DataVec.empty()) || (m_DataVec.size() == 1));
	}

	inline void Clear()
	{
		m_DataVec.clear();
		m_DataVec[0] = '\0';
	}
	
 private:
	/// 变量内存
	std::vector<unsigned char > m_DataVec;
};

#endif
