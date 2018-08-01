/***************************************************************************
 *  RuleEventThread.cpp - RuleEventThread Impl
 *
 *  Created: 2018-06-12 19:06:29
 *
 *  Copyright QRS
 ****************************************************************************/

#include "RuleEventThread.h"
#include "RuleEngineLog.h"
#include <unistd.h>

using namespace UTILS;

namespace HB {

RuleEventThread::RuleEventThread() : MessageLooper(),  mRun(false)
{

}

RuleEventThread::~RuleEventThread()
{

}

void RuleEventThread::start()
{
    MessageLooper::start();
    /* wait for rule thread up */
    while (!mRun)
        usleep(10 * 1000);
}

void RuleEventThread::run()
{
    RE_LOGI("Rule EventThread:[%u]\n", id());
    mRun = true;
    return MessageLooper::run();
}

} /* namespace HB */
