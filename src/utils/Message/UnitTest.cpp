/***************************************************************************
 *  UnitTest.cpp - UnitTest
 *
 *  Created: 2018-06-01 14:58:51
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Thread.h"
#include "Message.h"
#include "MessageLooper.h"
#include "MessageHandler.h"

#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <memory>

using namespace UTILS;

enum {
    MT_SYSTEM = 1,
    MT_UPGRADE,
    MT_TIMER,
};

class MainMessageHandler;

class MyObject : public Object {
public:
    MyObject() { printf("MyObject construct\n"); }
    ~MyObject(){ printf("MyObject destruct\n"); }
};

class MainMessageHandler : public MessageHandler {
public:
    MainMessageHandler(): MessageHandler() { }
    MainMessageHandler(MessageQueue *queue) : MessageHandler(queue) {  }
protected:
    void handleMessage(Message *msg);
};

void MainMessageHandler::handleMessage(Message *msg)
{
    printf("MainMessageHandler::handleMessage ppid[%lu]\n", pthread_self());

    switch (msg->what) {
    case MT_SYSTEM:
        if (msg->obj) {
#ifdef USE_SHARED_PTR
            printf("handle what[system] obj2 count = %ld\n", msg->obj.use_count());
#else
            printf("handle what[system] obj count = %d\n", msg->obj->getRefCnt());
#endif
        }
        break;
    case MT_UPGRADE:
        std::cout << "handle what[upgrade] " << " arg1: "<< msg->arg1 << std::endl;
        break;
    case MT_TIMER:
        break;
    default:
        ;
    }
}

class MyThread : public Thread {
public:
    MyThread() { }
    ~MyThread() { }
    void run();
};

void MyThread::run()
{
    /***************************************************************
     **   Test message handle construct and send API
     ***************************************************************/

    /* not message thread, using default loop thread (handleMessage in another thread) */
    printf("MyThread: send message ppid[%lu]\n", pthread_self());
    MainMessageHandler *handler = new MainMessageHandler();
    printf("Send message MT_SYSTEM with 1s delay!\n");
    handler->sendEmptyMessageDelayed(MT_SYSTEM, 1000 /* ms */);
    /* do something task */
    sleep(1);

    /***************************************************************
     **   Test send obj and ref API
     ***************************************************************/
#ifdef USE_SHARED_PTR
    std::shared_ptr<MyObject> obj2 = std::make_shared<MyObject>();
    Message *msg4 = handler->obtainMessage(MT_SYSTEM, obj2);
    Message *msg5 = handler->obtainMessage(MT_SYSTEM, obj2);
    Message *msg6 = handler->obtainMessage(MT_SYSTEM, obj2);
    printf("before send message obj2 count = %ld\n", obj2.use_count());
    handler->sendMessageDelayed(msg4, 1000);
    handler->sendMessageDelayed(msg5, 2000);
    handler->sendMessageDelayed(msg6, 3000);
    obj2.reset();
    printf("after send message obj2 count = %ld\n", obj2.use_count());
#else
    MyObject *obj = new MyObject();
    Message *msg1 = handler->obtainMessage(MT_SYSTEM, obj);
    Message *msg2 = handler->obtainMessage(MT_SYSTEM, obj);
    Message *msg3 = handler->obtainMessage(MT_SYSTEM, obj);
    printf("Before send message obj count = %d\n", obj->getRefCnt());
    handler->sendMessageDelayed(msg1, 1000);
    handler->sendMessageDelayed(msg2, 2000);
    handler->sendMessageDelayed(msg3, 3000);
    obj->safeUnref(); /* ref count - 1 */
    printf("after send message obj count = %d\n", obj->getRefCnt());
#endif
    sleep(5);

    /***************************************************************
     **   Test decontruct message handler
     ***************************************************************/

    std::cout << "Send message with delay, but destruct handler!, so never handle the msg." << std::endl;
    handler->sendEmptyMessageDelayed(MT_UPGRADE, 3000 /* ms */);
    delete handler;
    handler = 0;
    std::cout << "MyThread quit!" << std::endl;
}

class MyLooper : public MessageLooper {
public:
    MyLooper() : MessageLooper() { }
    ~MyLooper() { }
};

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    MyThread *mythread = new MyThread();
    sleep(3);
    mythread->start();

    MyLooper *mylooper = new MyLooper();
    MainMessageHandler *handler = new MainMessageHandler(mylooper->getMessageQueue());
    mylooper->start();
    printf("Send message MT_SYSTEM with 1s delay!\n");
    handler->sendEmptyMessageDelayed(MT_SYSTEM, 2000 /* ms */);
    handler->sendEmptyMessageDelayed(MT_SYSTEM, 2000 /* ms */);
    handler->sendEmptyMessageDelayed(MT_SYSTEM, 2000 /* ms */);
    handler->sendEmptyMessageDelayed(MT_SYSTEM, 2000 /* ms */);
    handler->sendEmptyMessageDelayed(MT_SYSTEM, 2000 /* ms */);

    /* Never return: default loop handle message */
    Looper::getDefaultLooper().run();
    return 0;
}
