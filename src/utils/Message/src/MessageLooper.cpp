/***************************************************************************
 *  MessageLooper - Message Looper Impl
 *
 *  Created: 2018-06-02 08:46:57
 *
 *  Copyright QRS
 ****************************************************************************/

#include "MessageLooper.h"
#include "MessageHandler.h"
#include "Message.h"

namespace UTILS {

namespace Looper {

static MessageLooper *gDefaultLooper = 0;

MessageLooper& getDefaultLooper()
{
    /* TODO */
    if (!gDefaultLooper) {
        gDefaultLooper = new MessageLooper(pthread_self());
        gDefaultLooper->start();
        /* gDefaultLooper->run(); */
    }
    return *gDefaultLooper;
}

} /* namespace Looper */

void MessageLooper::run()
{
    MessageQueue *queue = getMessageQueue();

    while (true) {
        Message *msg = queue->next();
        if (msg != NULL) {
            if (0 == msg->target) {
                return;
            }
            msg->target->dispatchMessage(msg);
            msg->recycle();
        }
    }
}

} /* namespace UITLS */

