/***************************************************************************
 *  LogThread.cpp - Log Thread Impl
 *
 *  Created: 2018-06-04 09:54:22
 *
 *  Copyright QRS
 ****************************************************************************/

#include "LogThread.h"

#include <unistd.h>

extern "C" void logInit();

namespace UTILS {

LogThread::LogThread()
	: MessageLooper()
{

}

LogThread::~LogThread()
{

}

void LogThread::start()
{
    MessageLooper::start();

    /* wait creating thread and init log */
    usleep(200 * 1000);
}

void LogThread::run()
{
    printf("Log Thread:[%u]\n", (unsigned int)id());
    logInit();
    MessageLooper::run();
}

} /* namespace UTILS */

