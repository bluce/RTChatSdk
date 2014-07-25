

#ifndef _zNoncopyable_h_
#define _zNoncopyable_h_


class zNoncopyable
{

	protected:

		/**
		 * \brief 缺省构造函数
		 *
		 */
		zNoncopyable() {};

		/**
		 * \brief 缺省析构函数
		 *
		 */
		virtual ~zNoncopyable() {};

	private:

		/**
		 * \brief 拷贝构造函数，没有实现，禁用掉了
		 *
		 */
		zNoncopyable(const zNoncopyable&);

		/**
		 * \brief 赋值操作符号，没有实现，禁用掉了
		 *
		 */
		zNoncopyable & operator= (const zNoncopyable &);

};

#endif

