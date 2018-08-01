/***************************************************************************
 *  Singleton.h - Singleton Header
 *
 *  Created: 2018-06-04 10:48:49
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#ifdef __cplusplus

namespace UTILS {

template<typename TYPE>
class Singleton {
public:
    static TYPE& getInstance(){
        if (0 == _sInstance)
            _sInstance = new TYPE();
        return *_sInstance;
    }
protected:
    Singleton() {}
    ~Singleton() {}

private:
    Singleton(const Singleton&);
    Singleton& operator= (const Singleton&);
    static TYPE* _sInstance;
};

#define SINGLETON_STATIC_INSTANCE(TYPE)                          \
    template<> TYPE* ::UTILS::Singleton< TYPE >::_sInstance(0);  \
    template class ::UTILS::Singleton< TYPE >;

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* ifndef __SINGLETON_H__ */
