/***************************************************************************
 *  Mutex.h - Mutex Class Header
 *
 *  Created: 2018-05-31 14:59:43
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __MUTEX__H__
#define __MUTEX__H__

#include "Noncopyable.h"
#include <pthread.h>

#ifdef __cplusplus

namespace UTILS {

class Mutex : Noncopyable {
public:
    enum {
        PRIVATE = 0,
        SHARED = 1,
    };
    Mutex();
    Mutex(const char* name);
    Mutex(int type, const char* name = 0);
    ~Mutex();

    int lock();
    void unlock();
    int trylock();

    class Autolock {
    public:
        inline Autolock(Mutex& mutex) : _mLock(mutex) { _mLock.lock(); }
        inline Autolock(Mutex* mutex) : _mLock(*mutex) { _mLock.lock(); }
        inline ~Autolock() { _mLock.unlock(); }
    private:
        Mutex& _mLock;
    }; /* class Autolock */

private:
    char mName[32];
    pthread_mutex_t mMutex;
}; /* class Mutex */

inline int Mutex::lock()
{
    /* Use mName to debug */
    return pthread_mutex_lock(&mMutex);
}

inline void Mutex::unlock()
{
    /* Use mName to debug */
    pthread_mutex_unlock(&mMutex);
}

inline int Mutex::trylock()
{
    return pthread_mutex_trylock(&mMutex);
}

typedef Mutex::Autolock AutoMutex;

} /* namespace UTILS */

#endif /*__cplusplus */

#endif /* __MUTEX__H__ */
