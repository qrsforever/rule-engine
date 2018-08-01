/***************************************************************************
 *  MessageQueue.h - Message Queue Header
 *
 *  Created: 2018-06-01 10:56:57
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __MessageQueue_H__
#define __MessageQueue_H__

#include "Message.h"

#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus

namespace UTILS {

class Object;
class MessageHandler;

class MessageQueue {
public:
    MessageQueue();

    Message *next();
    Message *pullNextLocked(uint32_t now);
    bool enqueueMessage(Message *msg, uint32_t when);
    bool removeMessages(MessageHandler *h, int what, int arg1, int arg2, bool doRemove);

#ifdef USE_SHARED_PTR
    bool removeMessages(MessageHandler *h, int what, std::shared_ptr<Object> object, bool doRemove);
    void removeMessages(MessageHandler *h, std::shared_ptr<Object> object);
#else
    bool removeMessages(MessageHandler *h, int what, Object *object, bool doRemove);
    void removeMessages(MessageHandler *h, Object *object);
#endif

    pthread_cond_t mCond;
    pthread_mutex_t mMutex;
private:
    Message *mMessages;
    bool mQuiting;
};

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __MessageQueue_H__ */
