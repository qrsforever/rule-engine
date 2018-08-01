/***************************************************************************
 *  MainPublicHandler.cpp - Main public handler impl
 *
 *  Created: 2018-06-13 15:21:05
 *
 *  Copyright QRS
 ****************************************************************************/

#include "MainPublicHandler.h"
#include "Log.h"
#include "StringData.h"
#include "MessageTypes.h"
#include "MessageLooper.h"
#include "RuleEventHandler.h"
#include "RuleEngineService.h"
#include "MonitorTool.h"

#ifdef SIM_SUITE
#include "TempSimulateSuite.h" /* TODO only for test */
#endif

namespace HB {

static MessageLooper *gMainThread = 0;
static MainPublicHandler *gMainHander = 0;

MainPublicHandler::MainPublicHandler()
{

}

MainPublicHandler::MainPublicHandler(MessageQueue *queue)
    : MessageHandler(queue)
{

}

MainPublicHandler::~MainPublicHandler()
{

}

void MainPublicHandler::doDeviceEvent(Message *msg)
{
    LOGTT();
}

void MainPublicHandler::doRuleEvent(Message *msg)
{
    LOGTT();
    switch (msg->arg1) {
        case RULE_EVENT_SYNC:
            switch (msg->arg2) {
                case RULE_SYNC_NONE:
                    /* TODO Do nothing */
                    break;
                case RULE_SYNC_NEW_VERSION:
                    break;
                case RULE_SYNC_CHECK_TIMER:
                    sendMessageDelayed(obtainMessage(msg->what, msg->arg1, msg->arg2), 900000);
                    break;
                default:
                    break;
            }
            break;
    }
}

void MainPublicHandler::doMonitorEvent(Message *msg)
{
    switch (msg->arg1) {
        case MONITOR_CLOSE_CLIENT:
            monitor().delClient(msg->arg2);
            break;
        default:
            ;
    }
}

void MainPublicHandler::doSimulateEvent(Message *msg)
{
    /* TODO only for test */
#ifdef SIM_SUITE
    tempSimulateTest(msg);
#endif
}

void MainPublicHandler::handleMessage(Message *msg)
{
    LOGD("msg: [%d] [%d] [%d]\n", msg->what, msg->arg1, msg->arg2);
    switch (msg->what) {
        case MT_DEVICE:
            doDeviceEvent(msg);
            break;
        case MT_RULE:
            doRuleEvent(msg);
            break;
        case MT_MONITOR:
            doMonitorEvent(msg);
            break;
        case MT_SIMULATE:
            doSimulateEvent(msg);
            break;
        default:
            break;
    }
}

MainPublicHandler& mainHandler()
{
    if (0 == gMainHander) {
        LOGW("check me\n");
        gMainHander = new MainPublicHandler(gMainThread->getMessageQueue());
    }
    return *gMainHander;
}

extern "C" {

int initMainThread()
{
    if (0 == gMainThread) {
        /* TODO call this API in main thread */
        gMainThread = new MessageLooper(pthread_self());
        gMainHander = new MainPublicHandler();
    }
    return 0;
}

int mainThreadRun()
{
    if (gMainThread)
        gMainThread->run();
    return 0;
}

} /* extern C */

} /* namespace HB */
