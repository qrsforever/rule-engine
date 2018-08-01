/***************************************************************************
 *  UnitTest.cpp - Unit Test for Log
 *
 *  Created: 2018-06-04 13:29:39
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Log.h"
#include "LogThread.h"
#include "LogPool.h"
#include "LogFile.h"

#include <unistd.h>

using namespace UTILS;

static LogThread * g_logThread = 0;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    g_logThread = new LogThread();
    g_logThread->start();

    LogPool &logPool = LogPool::getInstance();

    LOGD("Not Show Debug Log.\n");
    LOGW("Show Warning Log.\n");

    setLogLevel(LOG_LEVEL_INFO);

    LOGD("Show Debug Log.\n");

    LogFile *logFile = new LogFile();
    logPool.attachFilter(logFile);

    for (int i = 0; i < 3; ++i) {
        LOGI("Main Looper!\n");
        sleep(1);
    };

    logPool.detachFilter(logFile);
    for (int i = 0; i < 3; ++i) {
        LOGI("Main Looper!\n");
        sleep(1);
    };

    delete(logFile);
    logFile = 0;

    return 0;
}
