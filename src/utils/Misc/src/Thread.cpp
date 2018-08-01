/***************************************************************************
 *  Thread.cpp - Thread Base Class Impl
 *
 *  Created: 2018-05-31 14:34:17
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Thread.h"

#include <string.h>
#include <map>

namespace UTILS {

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

static std::map<pthread_t, Thread *> gThreads;
typedef std::map<pthread_t, Thread *>::iterator ThreadsIter_t;

Thread::Thread(): mRunnable(0)
{
    _init();
}

Thread::Thread(Runnable *r): mRunnable(r)
{
    _init();
}

Thread::Thread(pthread_t id)
{
    _init(id);
}

Thread::~Thread()
{
    pthread_mutex_lock(&gMutex);
    ThreadsIter_t it = gThreads.find(mID);
    if (it != gThreads.end())
        gThreads.erase(it);
    pthread_mutex_unlock(&gMutex);

    pthread_mutex_destroy(&mMutex);
}

void Thread::_init(pthread_t id)
{
    pthread_mutex_init(&mMutex, 0);

    pthread_mutex_lock(&mMutex);

    if (id > 0)
        mID = id;
    else
        pthread_create(&mID, NULL, threadEntry, this);

    pthread_mutex_lock(&gMutex);
    gThreads.insert(std::make_pair(mID, this));
    pthread_mutex_unlock(&gMutex);

#ifdef DEBUG_THREAD
    snprintf(mName, 31, "Thread-%s", mID);
#endif
}

#ifdef DEBUG_THREAD
void Thread::setName(const char* name)
{
    if (name)
        strncpy(mName, name, 31);
}
#endif

void Thread::start()
{
    pthread_mutex_unlock(&mMutex);
}

void Thread::join()
{
    pthread_join(mID, 0);
}

Thread* Thread::currentThread()
{
    Thread *curr = 0;
    pthread_t tID = pthread_self();
    pthread_mutex_lock(&gMutex);
    ThreadsIter_t it = gThreads.find(tID);
    if (it != gThreads.end())
        curr = it->second;
    pthread_mutex_unlock(&gMutex);
    return curr;
}

void* Thread::threadEntry(void *r) {
    Thread *self = (Thread *)r;

    pthread_mutex_lock(&(self->mMutex));

    if (self->mRunnable != 0)
        self->mRunnable->run();
    else
        self->run();

    return (void*)0;
}

} /* namespace UTILS */
