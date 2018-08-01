/***************************************************************************
 *  RuleEngineTimer.h - Rule Engine Timer
 *
 *  Created: 2018-07-13 18:28:29
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RuleEngineTimer_H__
#define __RuleEngineTimer_H__

#include "TimerEvent.h"
#include "Mutex.h"

#include <map>
#include <signal.h>

#ifdef __cplusplus

namespace HB {

class RuleEventHandler;

class RuleEngineTimer {
public:
    typedef std::shared_ptr<RuleEngineTimer> pointer;
    RuleEngineTimer(RuleEventHandler &handler);
    ~RuleEngineTimer();

    int addEvent(std::string &ruleId, TimerEvent::pointer e);
    int delEvent(int eID);

    int start(int eID = -1);
    int stop(int eID = -1);
    int start(const std::string &ruleId);
    int stop(const std::string &ruleId);

    bool send(int arg1, int arg2);

    struct TimerEnity {
        TimerEnity(std::string &ruleId, TimerEvent::pointer e, int id, RuleEngineTimer *timer)
            : mRuleId(ruleId), mEventPtr(e), mEventID(id)
              , mToplayTimerID(0), mDurationTimerID(0), mEngineTimer(timer) {}
        std::string mRuleId;
        TimerEvent::pointer mEventPtr;
        int mEventID;
        timer_t mToplayTimerID;
        timer_t mDurationTimerID;
        RuleEngineTimer *mEngineTimer;
    }; /* struct TimerEnity */

private:
    int _StartTimer(TimerEnity *timer);
    int _CancelTimer(TimerEnity *timer);

    static void _sToplayTimerThread(union sigval v);
    static void _sDurationTimerThread(union sigval v);

private:
    RuleEventHandler &mH;
    UTILS::Mutex mEventsMutex;
    static std::map<int, TimerEnity*> mEventsMap;
}; /* class RuleEngineTimer */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __RuleEngineTimer_H__ */
