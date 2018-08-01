/***************************************************************************
 *  MessageHandler.cpp - Message Handler Impl
 *
 *  Created: 2018-06-01 10:18:06
 *
 *  Copyright QRS
 ****************************************************************************/

#include "MessageHandler.h"
#include "Message.h"
#include "MessageQueue.h"
#include "MessageLooper.h"

#include "SysTime.h"

#include <typeinfo>

namespace UTILS {

void MessageHandler::dispatchMessage(Message *msg)
{
    if (mCallback != NULL) {
        if (mCallback->handleMessage(msg)) {
            return;
        }
    }
    handleMessage(msg);
}

MessageHandler::MessageHandler(): mQueue(0)
{
    /* TODO */
    Thread *curr = Thread::currentThread();
    MessageLooper *looper = 0;
    try {
        looper = dynamic_cast<MessageLooper*>(curr);
        if (0 == looper)
            looper = &Looper::getDefaultLooper();
    } catch (const std::bad_cast& e) {
        looper = &Looper::getDefaultLooper();
    }
    mQueue = looper->getMessageQueue();
    mCallback = NULL;
}

MessageHandler::MessageHandler(MessageQueue *queue): mQueue(queue)
{
    mCallback = NULL;
}

MessageHandler::~MessageHandler()
{
    removeAllMessages(NULL);
}

Message *MessageHandler::obtainMessage()
{
    return Message::obtain(this);
}

Message *MessageHandler::obtainMessage(int what)
{
    return Message::obtain(this, what);
}

#ifdef USE_SHARED_PTR
Message *MessageHandler::obtainMessage(int what, std::shared_ptr<Object> obj)
#else
Message *MessageHandler::obtainMessage(int what, Object *obj)
#endif
{
    return Message::obtain(this, what, obj);
}

Message *MessageHandler::obtainMessage(int what, int arg1, int arg2)
{
    return Message::obtain(this, what, arg1, arg2);
}

#ifdef USE_SHARED_PTR
Message *MessageHandler::obtainMessage(int what, int arg1, int arg2, std::shared_ptr<Object> obj)
#else
Message *MessageHandler::obtainMessage(int what, int arg1, int arg2, Object *obj)
#endif
{
    return Message::obtain(this, what, arg1, arg2, obj);
}

bool MessageHandler::sendMessage(Message *msg)
{
    return sendMessageDelayed(msg, 0);
}

bool MessageHandler::sendEmptyMessage(int what)
{
    return sendEmptyMessageDelayed(what, 0);
}

bool MessageHandler::sendEmptyMessageDelayed(int what, uint32_t delayMillis)
{
    Message *msg = Message::obtain();
    msg->what = what;
    return sendMessageDelayed(msg, delayMillis);
}

bool MessageHandler::sendEmptyMessageAtTime(int what, uint32_t uptimeMillis)
{
    Message *msg = Message::obtain();
    msg->what = what;
    return sendMessageAtTime(msg, uptimeMillis);
}

bool MessageHandler::sendMessageDelayed(Message *msg, uint32_t delayMillis)
{
    return sendMessageAtTime(msg, SysTime::GetMSecs() + delayMillis);
}

bool MessageHandler::sendMessageAtTime(Message *msg, uint32_t uptimeMillis)
{
    bool sent = false;
    MessageQueue *queue = mQueue;
    if (queue != NULL) {
        msg->target = this;
        sent = queue->enqueueMessage(msg, uptimeMillis);
    }
    return sent;
}

bool MessageHandler::sendMessageAtFrontOfQueue(Message *msg)
{
    bool sent = false;
    MessageQueue *queue = mQueue;
    if (queue != NULL) {
        msg->target = this;
        sent = queue->enqueueMessage(msg, 0);
    }
    return sent;
}

void MessageHandler::removeMessages(int what)
{
    mQueue->removeMessages(this, what, NULL, true);
}

void MessageHandler::removeMessages(int what, int arg1, int arg2)
{
    mQueue->removeMessages(this, what, arg1, arg2, true);
}

#ifdef USE_SHARED_PTR
void MessageHandler::removeMessages(int what, std::shared_ptr<Object> object)
#else
void MessageHandler::removeMessages(int what, Object *object)
#endif
{
    mQueue->removeMessages(this, what, object, true);
}

#ifdef USE_SHARED_PTR
void MessageHandler::removeAllMessages(std::shared_ptr<Object> token)
#else
void MessageHandler::removeAllMessages(Object *token)
#endif
{
    mQueue->removeMessages(this, token);
}

bool MessageHandler::hasMessages(int what)
{
    return mQueue->removeMessages(this, what, NULL, false);
}

#ifdef USE_SHARED_PTR
bool MessageHandler::hasMessages(int what, std::shared_ptr<Object> object)
#else
bool MessageHandler::hasMessages(int what, Object *object)
#endif
{
    return mQueue->removeMessages(this, what, object, false);
}

} /* namespace UTILS */
