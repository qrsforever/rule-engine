/***************************************************************************
 *  MessageHandler.h - Message Handler Handler
 *
 *  Created: 2018-06-01 10:12:50
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __MessageHandler_H__
#define __MessageHandler_H__

#include <stdint.h>

#ifdef USE_SHARED_PTR
#include <memory>
#endif

#ifdef __cplusplus

namespace UTILS {

class Object;
class Message;
class MessageQueue;

class MessageHandler {
protected:
    virtual void handleMessage(Message *msg) = 0;

public:
    class Callback {
        public: virtual ~Callback() {}
        public: virtual bool handleMessage(Message *msg) = 0;
    }; /* calss Callback */

    virtual void dispatchMessage(Message *msg);

    MessageHandler();
    MessageHandler(MessageQueue *queue);

    virtual ~MessageHandler();

    Message *obtainMessage();
    Message *obtainMessage(int what);
    Message *obtainMessage(int what, int arg1, int arg2);

    bool sendMessage(Message *msg);
    bool sendEmptyMessage(int what);
    bool sendEmptyMessageDelayed(int what, uint32_t delayMillis);
    bool sendEmptyMessageAtTime(int what, uint32_t uptimeMillis);
    bool sendMessageDelayed(Message *msg, uint32_t delayMillis);
    bool sendMessageAtTime(Message *msg, uint32_t uptimeMillis);
    bool sendMessageAtFrontOfQueue(Message *msg);

    void removeMessages(int what);
    void removeMessages(int what, int arg1, int arg2);

    bool hasMessages(int what);

#ifdef USE_SHARED_PTR
    Message *obtainMessage(int what, std::shared_ptr<Object> obj);
    Message *obtainMessage(int what, int arg1, int arg2, std::shared_ptr<Object> obj);
    void removeMessages(int what, std::shared_ptr<Object> obj);
    void removeAllMessages(std::shared_ptr<Object> token);
    bool hasMessages(int what, std::shared_ptr<Object> obj);
#else
    Message *obtainMessage(int what, Object *obj);
    Message *obtainMessage(int what, int arg1, int arg2, Object *obj);
    void removeMessages(int what, Object *object);
    void removeAllMessages(Object *token);
    bool hasMessages(int what, Object *object);
#endif

    MessageQueue *mQueue;
    Callback *mCallback;

}; /* class MessageHandler */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* _MessageHandler_H_ */
