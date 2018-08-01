/***************************************************************************
 *  RuleEventThread.h - Rule Event Thread Header
 *
 *  Created: 2018-06-12 19:05:03
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RuleEventThread_H__
#define __RuleEventThread_H__

#include "MessageLooper.h"
#include "Singleton.h"

#ifdef __cplusplus

namespace HB {

class RuleEventThread : public ::UTILS::MessageLooper {
public:
    RuleEventThread();
    ~RuleEventThread();
    virtual void start();
    virtual void run();
private:
    bool mRun;
}; /* class RuleEventThread */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __RuleEventThread_H__ */
