/***************************************************************************
 *  LogThread.h - Log Thread Header
 *
 *  Created: 2018-06-04 09:52:47
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __LogThread_H__
#define __LogThread_H__

#include "MessageLooper.h"

#ifdef __cplusplus

namespace UTILS {

class LogThread : public MessageLooper {
public:
    LogThread();
    ~LogThread();

    void start();

    virtual void run();
};

} /* namespace UTILS */

#endif // __cplusplus

#endif // __LogThread_H__
