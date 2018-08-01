/***************************************************************************
 *  Message.h - Message Header
 *
 *  Created: 2018-06-01 09:55:46
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Message_H__
#define __Message_H__

#include "Object.h"

#ifdef USE_SHARED_PTR
#include <memory>
#endif

#ifdef __cplusplus

namespace UTILS {

class MessageHandler;

class Message {
public:
    int what;
    int arg1;
    int arg2;

#ifdef USE_SHARED_PTR
    std::shared_ptr<Object> obj;
#else
    Object *obj;
#endif

    unsigned long when;

    static Message *obtain();
    static Message *obtain(MessageHandler *h);
    static Message *obtain(MessageHandler *h, int pWhat);
    static Message *obtain(MessageHandler *h, int pWhat, int pArg1, int arg2);

#ifdef USE_SHARED_PTR
    static Message *obtain(MessageHandler *h, int pWhat, std::shared_ptr<Object> pObj);
    static Message *obtain(MessageHandler *h, int pWhat, int pArg1, int arg2, std::shared_ptr<Object> pObj);
#else
    static Message *obtain(MessageHandler *h, int pWhat, Object *pObj);
    static Message *obtain(MessageHandler *h, int pWhat, int pArg1, int arg2, Object *pObj);
#endif

    void recycle();

    Message();
    ~Message();

    Message *next;

    MessageHandler *target;
    inline void setTarget(MessageHandler *h) { target = h; }
    inline MessageHandler* getTarget() { return target; }

private:
    static Message *mPool;
    static int mPoolSize;
}; /* class Message */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __Message_H__ */
