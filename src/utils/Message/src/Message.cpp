/***************************************************************************
 *  Message.cpp - Message Impl
 *
 *  Created: 2018-06-01 10:03:45
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Message.h"
#include "MessageHandler.h"

#include <pthread.h>

namespace UTILS {

#define MESSAGE_MAX_POOL_SIZE	50

static Message gMessages[MESSAGE_MAX_POOL_SIZE];

static inline int inHeap(Message *msg)
{
    if ((msg >= &gMessages[0]) && (msg < &gMessages[sizeof(gMessages)/sizeof(gMessages[0])]))
        return 0;
    else
        return 1;
}

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

Message *Message::mPool = NULL;
int Message::mPoolSize = 0;

Message *Message::obtain()
{
    Message *m;

    pthread_mutex_lock(&gMutex);

    if (mPoolSize < MESSAGE_MAX_POOL_SIZE) {
        m = &gMessages[mPoolSize++];
    }
    else if (mPool != NULL) {
        m = mPool;
        mPool = m->next;
        m->next = NULL;
    }
    else
        m = new Message();

    pthread_mutex_unlock(&gMutex);

    return m;
}

Message *Message::obtain(MessageHandler *h)
{
    Message *m = obtain();
    m->target = h;
    return m;
}

Message *Message::obtain(MessageHandler *h, int pWhat)
{
    Message *m = obtain();
    m->target = h;
    m->what = pWhat;
    return m;
}

#ifdef USE_SHARED_PTR
Message *Message::obtain(MessageHandler *h, int pWhat, std::shared_ptr<Object> pObj)
#else
Message *Message::obtain(MessageHandler *h, int pWhat, Object *pObj)
#endif
{
    Message *m = obtain();
    m->target = h;
    m->what = pWhat;
    m->obj = pObj;
#ifdef USE_SHARED_PTR
#else
    m->obj->safeRef();
#endif
    return m;
}

Message *Message::obtain(MessageHandler *h, int pWhat, int pArg1, int pArg2)
{
    Message *m = obtain();
    m->target = h;
    m->what = pWhat;
    m->arg1 = pArg1;
    m->arg2 = pArg2;
    return m;
}

#ifdef USE_SHARED_PTR
Message *Message::obtain(MessageHandler *h, int pWhat, int pArg1, int pArg2, std::shared_ptr<Object> pObj)
#else
Message *Message::obtain(MessageHandler *h, int pWhat, int pArg1, int pArg2, Object *pObj)
#endif
{
    Message *m = obtain();
    m->target = h;
    m->what = pWhat;
    m->arg1 = pArg1;
    m->arg2 = pArg2;
    m->obj = pObj;
#ifdef USE_SHARED_PTR
#else
    m->obj->safeRef();
#endif
    return m;
}

void Message::recycle()
{
#ifdef USE_SHARED_PTR
    obj.reset();
#else
    obj->safeUnref();
    obj = NULL;
#endif
    when = 0;
    target = NULL;

    if (!inHeap(this)) {
        pthread_mutex_lock(&gMutex);
        next = mPool;
        mPool = this;
        pthread_mutex_unlock(&gMutex);
    }
    else
        delete this;
}

Message::Message()
{
    what = 0;
    arg1 = 0;
    arg2 = 0;
    obj = NULL;
    when = 0;
    target = NULL;
    next = NULL;
}

Message::~Message()
{
}

} /* namespace UTILS */
