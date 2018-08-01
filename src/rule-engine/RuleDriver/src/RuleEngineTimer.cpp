/***************************************************************************
 *  RuleEngineTimer.cpp - Rule Engine Timer
 *
 *  Created: 2018-07-13 18:29:53
 *
 *  Copyright QRS
 ****************************************************************************/

#include "RuleEngineTimer.h"
#include "RuleEventHandler.h"
#include "RuleEventTypes.h"
#include "RuleEngineLog.h"

#include <string.h>

namespace HB {

using namespace UTILS;

std::map<int, RuleEngineTimer::TimerEnity*> RuleEngineTimer::mEventsMap;

RuleEngineTimer::RuleEngineTimer(RuleEventHandler &handler)
    : mH(handler)
{

}

RuleEngineTimer::~RuleEngineTimer()
{
    std::map<int, TimerEnity*>::iterator it;
    for (it = mEventsMap.begin(); it != mEventsMap.end(); ++it) {
        _CancelTimer(it->second);
        delete it->second;
    }
    mEventsMap.clear();
}

int RuleEngineTimer::addEvent(std::string &ruleId, TimerEvent::pointer e)
{
    Mutex::Autolock _l(mEventsMutex);

    std::map<int, TimerEnity*>::iterator it;
    it = mEventsMap.find(e->getID());
    if (it != mEventsMap.end())
        return RES_FAIL;
    mEventsMap.insert(std::make_pair(
            e->getID(), new TimerEnity(ruleId, e, e->getID(), this)));
    return RES_SUCCESS;
}

int RuleEngineTimer::delEvent(int eID)
{
    Mutex::Autolock _l(mEventsMutex);

    std::map<int, TimerEnity*>::iterator it = mEventsMap.find(eID);
    if (it == mEventsMap.end())
        return RES_FAIL;
    delete it->second;
    mEventsMap.erase(it);
    return RES_SUCCESS;
}

int RuleEngineTimer::start(int eID)
{
    Mutex::Autolock _l(mEventsMutex);

    std::map<int, TimerEnity*>::iterator it;
    if (eID != -1) {
        it = mEventsMap.find(eID);
        if (it == mEventsMap.end())
            return RES_FAIL;
        return _StartTimer(it->second);
    }

    int res = RES_SUCCESS;
    for (it = mEventsMap.begin(); it != mEventsMap.end(); ++it) {
        if (_StartTimer(it->second) != RES_SUCCESS) {
           RE_LOGW("Start Timer[%d]\n", it->first);
           res = RES_FAIL;
        }
    }
    return res;
}

int RuleEngineTimer::stop(int eID)
{
    Mutex::Autolock _l(mEventsMutex);

    std::map<int, TimerEnity*>::iterator it;
    if (eID != -1) {
        it = mEventsMap.find(eID);
        if (it == mEventsMap.end())
            return RES_FAIL;
        return _CancelTimer(it->second);
    }

    int res = RES_SUCCESS;
    for (it = mEventsMap.begin(); it != mEventsMap.end(); ++it) {
        if (_CancelTimer(it->second) != RES_SUCCESS) {
           RE_LOGW("Canel Timer[%d]\n", it->first);
           res = RES_FAIL;
        }
    }
    return res;
}

int RuleEngineTimer::start(const std::string &ruleId)
{
    Mutex::Autolock _l(mEventsMutex);
    std::map<int, TimerEnity*>::iterator it;

    int res = RES_SUCCESS;
    for (it = mEventsMap.begin(); it != mEventsMap.end(); ++it) {
        if (it->second->mRuleId == ruleId) {
            if (_StartTimer(it->second) != RES_SUCCESS) {
                RE_LOGW("Start Timer[%d]\n", it->first);
                res = RES_FAIL;
            }
        }
    }
    return res;
}

int RuleEngineTimer::stop(const std::string &ruleId)
{
    Mutex::Autolock _l(mEventsMutex);
    std::map<int, TimerEnity*>::iterator it;

    int res = RES_SUCCESS;
    for (it = mEventsMap.begin(); it != mEventsMap.end(); ++it) {
        if (it->second->mRuleId == ruleId) {
            if (_CancelTimer(it->second) != RES_SUCCESS) {
                RE_LOGW("Start Timer[%d]\n", it->first);
                res = RES_FAIL;
            }
        }
    }
    return res;
}

bool RuleEngineTimer::send(int arg1, int arg2)
{
    return mH.sendMessage(mH.obtainMessage(RET_TIMER_EVENT, arg1, arg2));
}

void RuleEngineTimer::_sToplayTimerThread(union sigval v)
{
    RE_LOGTT();
    int eID = v.sival_int;
    RuleEngineTimer *et = 0;
    std::map<int, TimerEnity*>::iterator it = mEventsMap.find(eID);
    if (it == mEventsMap.end())
        return;

    et = it->second->mEngineTimer;
    et->send(TIMER_TOPLAY, eID);
    et->stop(eID);
    if (et->start(eID) < 0)
        et->delEvent(eID);
}

void RuleEngineTimer::_sDurationTimerThread(union sigval v)
{
    RE_LOGTT();
    int eID = v.sival_int;
    RuleEngineTimer *et = 0;
    std::map<int, TimerEnity*>::iterator it = mEventsMap.find(eID);
    if (it == mEventsMap.end())
        return;
    et = it->second->mEngineTimer;
    et->send(TIMER_DURATION, eID);
}

int RuleEngineTimer::_StartTimer(TimerEnity *timer)
{
    time_t duration;
    SysTime::DateTime dt;
    SysTime::GetDateTime(&dt);

    RE_LOGD("%04d%02d%02d %02d:%02d:%02d %lu\n", dt.mYear, dt.mMonth, dt.mDay, dt.mHour, dt.mMinute, dt.mSecond, duration);
    time_t secs1 = dateTimeToSeconds(dt);
    if (timer->mEventPtr->nextDate(dt, duration) < 0)
        return RES_FAIL;
    time_t secs2 = dateTimeToSeconds(dt);
    RE_LOGD("%04d%02d%02d %02d:%02d:%02d %lu\n", dt.mYear, dt.mMonth, dt.mDay, dt.mHour, dt.mMinute, dt.mSecond, duration);

    if (secs1 < secs2 || duration < 1) {
        RE_LOGW("Seconds [%ld vs %ld] [%ld]\n", secs1, secs2, duration);
        return RES_FAIL;
    }

    timer_t timerid;
    struct itimerspec it;
    struct sigevent evp;

    /*  ToplayTimerID  */
    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_int = timer->mEventID;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = _sToplayTimerThread;
    if (timer_create(CLOCK_REALTIME, &evp, &timerid) < 0) {
        RE_LOGE("create timer error!\n");
        return RES_FAIL;
    }
    RE_LOGD("ToplayTimer [%ld] [%ld] [%ld]\n", secs1, secs2, secs2 - secs1);

    it.it_interval.tv_sec = 0;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = (secs2 - secs1);
    it.it_value.tv_nsec = 0;
    if (timer_settime(timerid, 0, &it, NULL) < 0) {
        RE_LOGE("set timer error!\n");
        timer_delete(timerid);
        return RES_FAIL;
    }
    timer->mToplayTimerID = timerid;

    /* DurationTimerID */
    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_int = timer->mEventID;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = _sDurationTimerThread;
    if (timer_create(CLOCK_REALTIME, &evp, &timerid) < 0) {
        RE_LOGE("create timer error!\n");
        return RES_FAIL;
    }
    RE_LOGD("DurationTimer [%ld]\n", duration);

    it.it_interval.tv_sec = 0;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = duration;
    it.it_value.tv_nsec = 0;
    if (timer_settime(timerid, 0, &it, NULL) < 0) {
        RE_LOGE("set timer error!\n");
        timer_delete(timerid);
        return RES_FAIL;
    }
    timer->mDurationTimerID = timerid;

    return RES_SUCCESS;
}

int RuleEngineTimer::_CancelTimer(TimerEnity *timer)
{
    if (timer->mToplayTimerID != 0) {
        timer_delete(timer->mToplayTimerID);
        timer->mToplayTimerID = 0;
    }
    if (timer->mDurationTimerID != 0) {
        timer_delete(timer->mDurationTimerID);
        timer->mDurationTimerID = 0;
    }
    return RES_SUCCESS;
}

} /* namespace HB */
