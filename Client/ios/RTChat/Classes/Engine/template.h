//
//  template.h
//  lion
//
//  Created by 王欣 on 13-8-24.
//
//

#ifndef __lion__template__
#define __lion__template__

template <typename T>
class SingletonFactory
{
public:
    static T* create()
    {
        return new T();
    }
    
    static void destroy(T* This)
    {
        delete This;
    }
};

/*!
 \brief
 单件
 */
template < typename T, typename MANA = SingletonFactory<T> >
class Singleton
{
private:
    Singleton(const Singleton&);
    const Singleton& operator=(const Singleton&);
    
protected:
    static T* _instance;
    
    Singleton()
    {
        if (!_instance)
            _instance = static_cast<T*>(this);
    }
    
    virtual ~Singleton()
    {
        _instance = NULL;
    }
    
public:
    static bool create()
    {
        return MANA::create() ? true : false;
    }
    
    static void destroy()
    {
        if (_instance)
        {
            MANA::destroy(_instance);
            _instance = NULL;
        }
    }
    
    inline static bool created()
    {
        return _instance ? true : false;
    }
    
    inline static T& instance()
    {
        return (*_instance);
    }
    
    inline static T* instancePtr()
    {
        return _instance;
    }
    
};

template <typename T, typename MANA>
T* Singleton<T, MANA>::_instance = NULL;

template <typename R, typename P1>
class CallBackHelper {
public:
    CallBackHelper()
    :_func(NULL)
    {
        
    }
    
    virtual ~CallBackHelper()
    {
        _func = NULL;
    }
    
    virtual void registerCallBack(std::function<R (P1)> func)
    {
        _func = func;
    };
    
protected:
    std::function<R (P1)>           _func;
};

//手动调用构造函数，不分配内存
template<class _T1>
inline      void constructInPlace(_T1  *_Ptr)
{
    new ((void  *)_Ptr) _T1();
}

/// 声明变长指令
#define BUFFER_CMD(cmd,name,len) char buffer##name[len];\
cmd *name=(cmd *)buffer##name;constructInPlace(name);

#endif /* defined(__lion__template__) */
