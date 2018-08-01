/***************************************************************************
 *  UnitTest.cpp - Unit Test
 *
 *  Created: 2018-06-12 17:23:53
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Log.h"
#include "LogThread.h"
#include "Message.h"
#include "MessageTypes.h"
#include "RuleEngineService.h"
#include "RuleEventHandler.h"
#include "DeviceDataChannel.h"
#include "RuleDataChannel.h"

#include "MainPublicHandler.h" /* temp use, not rule module */

using namespace HB;

extern "C" int initMainThread();
extern "C" int mainThreadRun();

class InitThread : public Thread {
public:
    InitThread(){}
    ~InitThread(){}
    void run();
};

void InitThread::run()
{
    /*************************
     *  Network module init  *
     *************************/

    /*****************************
     *  Rule Engine module init  *
     *****************************/
    ruleEngine().setServerRoot("clips");
    ruleEngine().setDeviceChannel(std::make_shared<DeviceDataChannel>());
    ruleEngine().setRuleChannel(std::make_shared<ElinkRuleDataChannel>());
    ruleEngine().init();


    /*******************************
     *  Device Manger module init  *
     *******************************/

    /* Simulate Test */
    /* mainHandler().sendEmptyMessage(MT_SIMULATE); */
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    printf("\n-----------Test---------\n");

    /***************************
     *  First init log module  *
     ***************************/
    initLogThread();
    setLogLevel(LOG_LEVEL_TRACE);

    /*****************************
     *  Second init main module  *
     *****************************/
    initMainThread();

    /*******************************
     *  Third init others modules  *
     *******************************/
    InitThread init;
    init.start();


    /****************************
    *  Main message queue run  *
    ****************************/
    return mainThreadRun();
}
