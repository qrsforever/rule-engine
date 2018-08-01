/***************************************************************************
 *  HomeBrainMain.cpp - HomeBrainMain
 *
 *  Created: 2018-06-13 15:25:27
 *
 *  Copyright
 ****************************************************************************/

#include "Log.h"
#include "LogFile.h"
#include "LogThread.h"
#include "LogPool.h"
#include "Message.h"
#include "MessageTypes.h"
#include "RuleEngineService.h"
#include "RuleEventHandler.h"
#include "DeviceDataChannel.h"
#include "RuleDataChannel.h"

#include "MainPublicHandler.h" /* temp use, not rule module */
#include "MonitorTool.h"

#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

#ifdef SIM_SUITE
#include "TempSimulateSuite.h"
#else
#include "HBDeviceManager.h"
#endif

using namespace HB;
using namespace UTILS;

#ifndef SIM_SUITE
using namespace OIC::Service::HB;

static HBDeviceManager *gDeviceManager = 0;

extern "C" HBDeviceManager& deviceManager()
{
    if (!gDeviceManager)
        gDeviceManager = new HBDeviceManager();
    return *gDeviceManager;
}
#endif

extern "C" int initMainThread();
extern "C" int mainThreadRun();


void _CatchSignal(int signo)
{
    switch (signo) {
        case SIGINT:
            monitor().finalize(); /* release socket resource */
            exit(0);
    }
}

/* init all modules thread */
class InitThread : public Thread {
public:
    InitThread(){}
    ~InitThread(){}
    void run();
};

void InitThread::run()
{
    printf("\n-----------Init Thread:[%u]---------\n", (unsigned int)pthread_self());

    /*-----------------------------------------------------------------
     *  Rule Engine module init
     *-----------------------------------------------------------------*/
    std::shared_ptr<DeviceDataChannel> deviceChnnl = std::make_shared<ElinkDeviceDataChannel>();
    ruleEngine().setServerRoot("clips");
    ruleEngine().setDeviceChannel(deviceChnnl);
    ruleEngine().init();

    /*-----------------------------------------------------------------
     *  Monitor Tool module init
     *-----------------------------------------------------------------*/
    printf("\nMonitor Init!\n");
    monitor().init(8192);
    monitor().start();
    signal(SIGINT, _CatchSignal);

    /*-----------------------------------------------------------------
     *  Device Manager module init
     *-----------------------------------------------------------------*/
    deviceManager().SetCallback(deviceChnnl.get());
    deviceManager().Init();

#ifdef SIM_SUITE
    /*-----------------------------------------------------------------
     *  Simulate Test module init
     *-----------------------------------------------------------------*/
    mainHandler().sendMessageDelayed(mainHandler().obtainMessage(MT_SIMULATE, 0, 0), 1000);
#endif
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    printf("\n-----------Main Thread:[%u]---------\n", (unsigned int)pthread_self());

    /*-----------------------------------------------------------------
     *  1. init log module
     *-----------------------------------------------------------------*/
    initLogThread();
    setLogLevel(LOG_LEVEL_WARNING);

    LogPool::getInstance().attachFilter(new LogFile());

    /*-----------------------------------------------------------------
     *  2. init main module
     *-----------------------------------------------------------------*/
    initMainThread();

    /*-----------------------------------------------------------------
     *  3. init others modules
     *-----------------------------------------------------------------*/
    InitThread init;
    init.start();

    /*-----------------------------------------------------------------
     *  4. dispach the message, never return;
     *-----------------------------------------------------------------*/
    return mainThreadRun();
}
