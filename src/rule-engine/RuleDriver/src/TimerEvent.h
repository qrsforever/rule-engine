/***************************************************************************
 *  TimerEvent.h - Rule Event Timer
 *
 *  Created: 2018-07-09 18:18:29
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __TimerEvent_H__
#define __TimerEvent_H__

#include "Object.h"
#include "SysTime.h"
#include <signal.h>
#include <memory>
#include <set>

#ifdef __cplusplus

using namespace UTILS;

namespace HB {

#define TIMER_VT_ALL    0
#define TIMER_VT_RANGE  1
#define TIMER_VT_SET    2

#define RET_NOVAL  -1
#define RET_OK      0
#define RET_RESET   1
#define RET_NULL    2
#define RET_IGNORE  3

typedef enum {
    eAny,
    eRange,
    eSet,
    eNull,
    eNot,
} TimeValueType;

typedef enum {
    eYear,
    eMonth,
    eWeek,
    eDay,
    eHour,
    eMinute,
    eSecond,
} TimeNodeType;

class TimerEvent;
class TimeNode {
public:
    TimeNode(TimeNodeType ntype, TimeValueType vtype);
    virtual ~TimeNode();

    TimeNodeType type() { return mNodeType; }
    TimeValueType valueType() { return mValueType; }
    TimeNode& setValueType(TimeValueType type);
    TimeNode& setRange(int min, int max) { mMin = min; mMax = max; return *this; }
    TimeNode& append(int val) { mValues.insert(val); return *this; }

    virtual int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust) = 0;

    TimeNode* setNextNode(TimeNode *node);
    TimeNode* nextNode() { return mNextNode; }

    const int& current() const { return mCurrent; }
    void setResetFlag(bool flag) { mReset = flag; }
    bool getResetFlag() { return mReset; }

    std::string toString();
    TimeNode& resetFromString(const std::string &str);

public:
    virtual int _UpdateValue(int max);
    virtual int _GetMaxDay(int year, int month);
    virtual int _ResetValue(bool flag = true);
    int _GetRealMinValue();
    int _GetRealMaxValue();

protected:
    friend class TimerEvent;
    TimeNode *mNextNode;
    TimeNode *mPreNode;
    TimeNodeType mNodeType;
    TimeValueType mValueType;
    bool mReset;
    int mCurrent;
    int mMin;
    int mMax;
    std::set<int> mValues;
}; /* class TimeNode */

class TimeYear : public TimeNode {
public:
    TimeYear(TimeValueType vtype):TimeNode(eYear, vtype){ mCurrent = 2018; }
    ~TimeYear(){}
    int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust);
}; /* class TimeYear */

class TimeMonth : public TimeNode {
public:
    TimeMonth(TimeValueType vtype):TimeNode(eMonth, vtype){};
    ~TimeMonth(){};
    int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust);
}; /* class TimeMonth */

class TimeDay : public TimeNode {
public:
    TimeDay(TimeValueType vtype):TimeNode(eDay, vtype){}
    ~TimeDay(){}
    int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust);
}; /* class TimeDay */

class TimeWeek : public TimeNode {
public:
    TimeWeek(TimeValueType vtype):TimeNode(eWeek, vtype){}
    ~TimeWeek(){}
    int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust);
}; /* class TimeWeek */

class TimeHour : public TimeNode {
public:
    TimeHour(TimeValueType vtype):TimeNode(eHour, vtype){}
    ~TimeHour(){}
    int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust);
}; /* class TimeHour */

class TimeMinute : public TimeNode {
public:
    TimeMinute(TimeValueType vtype):TimeNode(eMinute, vtype){}
    ~TimeMinute(){}
    int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust);
}; /* class TimeMinute */

class TimeSecond : public TimeNode {
public:
    TimeSecond(TimeValueType vtype):TimeNode(eSecond, vtype){}
    ~TimeSecond(){}
    int nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust);
}; /* class TimeSecond */

class TimerEvent : public Object {
public:
    typedef std::shared_ptr<TimerEvent> pointer;
    TimerEvent(int eventid, bool weekday = false);
    ~TimerEvent();
    TimeNode* getTimeNode(TimeNodeType type);
    TimeNode* year() { return getTimeNode(eYear); }
    TimeNode* month() { return getTimeNode(eMonth); }
    TimeNode* day() { return getTimeNode(eDay); }
    TimeNode* hour() { return getTimeNode(eHour); }
    TimeNode* minute() { return getTimeNode(eMinute); }
    TimeNode* second() { return getTimeNode(eSecond); }
    TimeNode* week() { return getTimeNode(eWeek); }
    int nextDate(SysTime::DateTime &dt, time_t &duration);

    int getID() { return mEventID; }
    bool getFlag() { return mWeekFlag; }

private:
    int mEventID;
    bool mWeekFlag;
    TimeNode *mHeader;
}; /* class TimerEvent */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __TimerEvent_H__ */
