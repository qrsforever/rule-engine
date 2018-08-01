/***************************************************************************
 *  Mutex.cpp - Mutex Class Impl
 *
 *  Created: 2018-05-31 15:01:20
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Mutex.h"

#include <string.h>

namespace UTILS {

Mutex::Mutex()
{
    pthread_mutex_init(&mMutex, 0);
}

Mutex::Mutex(const char* name)
{
    pthread_mutex_init(&mMutex, 0);
    if (name)
        strncpy(mName, name, 31);
}

Mutex::Mutex(int type, const char* name)
{
    if (type == SHARED) {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mMutex, &attr);
        pthread_mutexattr_destroy(&attr);
    } else {
        pthread_mutex_init(&mMutex, 0);
    }
    if (name)
        strncpy(mName, name, 31);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mMutex);
}

} /* namespace UTILS */
