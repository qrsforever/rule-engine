/***************************************************************************
 *  MessageLooper - Message Looper Header
 *
 *  Created: 2018-06-02 08:41:52
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __MessageLooper_H__
#define __MessageLooper_H__

#include "MessageQueue.h"
#include "Thread.h"

#ifdef __cplusplus

namespace UTILS {

class MessageLooper : public Thread {
public:
    MessageLooper(): Thread(), mMsgQueue() { }
    MessageLooper(pthread_t id): Thread(id), mMsgQueue() { }
    ~MessageLooper(){ }

    virtual void run();

    MessageQueue *getMessageQueue() { return &mMsgQueue; }

private:
    MessageQueue mMsgQueue;

}; /* class MessageLooper */

namespace Looper {
MessageLooper& getDefaultLooper();
}

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __MessageLooper_H__ */
