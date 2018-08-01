/***************************************************************************
 *  TimerEvent.cpp - Rule Event Timer
 *
 *  Created: 2018-07-09 18:18:38
 *
 *  Copyright QRS
 ****************************************************************************/

#include "TimerEvent.h"
#include "RuleEngineLog.h"
#include "Mutex.h"
#include "Common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#define DAY_SECONDS 86400
#define MAX_TIMER_EVENTS 36

namespace HB {

TimeNode::TimeNode(TimeNodeType ntype, TimeValueType vtype)
    : mNextNode(0), mPreNode(0)
    , mNodeType(ntype), mValueType(vtype)
    , mReset(false), mCurrent(RET_NOVAL)
{
    if (mValueType == eNull)
        mCurrent = _GetRealMinValue();
}

TimeNode::~TimeNode()
{
    mValues.clear();
    if (mNextNode)
        delete mNextNode;
    mNextNode = 0;
}

TimeNode& TimeNode::setValueType(TimeValueType type)
{
    if (type == eNull)
        mCurrent = _GetRealMinValue();
    mValueType = type;
    return *this;
}

TimeNode* TimeNode::setNextNode(TimeNode *node)
{
    mNextNode = node;
    node->mPreNode = this;
    return node;
}

int TimeNode::_UpdateValue(int max)
{
    if (getResetFlag()) {
        setResetFlag(false);
        return mCurrent;
    }
    int val = mCurrent;
    switch (mValueType) {
        case eAny:
            if (mCurrent < max)
                val++;
            break;
        case eRange:
            if (mMax > max)
                mMax = max;
            if (mCurrent < mMax)
                val++;
            break;
        case eSet:
            for (auto it = mValues.begin(); it != mValues.end(); ++it) {
                if (mCurrent < *it) {
                    val = *it;
                    break;
                }
            }
            break;
        case eNull:
            return RET_NULL;
        case eNot:
            if (mCurrent < max)
                val++;
            for (; mCurrent < max; mCurrent++) {
                for (auto it = mValues.begin(); it != mValues.end(); ++it) {
                    if (mCurrent != (*it))
                        break;
                }
            }
            break;
    }
    if (mCurrent != val) {
        mCurrent = val;
        if (nextNode())
            nextNode()->_ResetValue();
        return mCurrent;
    }
    return RET_NOVAL;
}

int TimeNode::_ResetValue(bool flag)
{
    if (flag) {
        if (mNextNode)
            mNextNode->_ResetValue(flag);
    }

    if (getResetFlag())
        return mCurrent;

    switch (mValueType) {
        case eAny:
            mCurrent = _GetRealMinValue();
            break;
        case eRange:
            mCurrent = mMin;
            break;
        case eSet:
            mCurrent = *(mValues.begin());
            break;
        case eNull:
            mCurrent = _GetRealMinValue();
            break;
        case eNot:
            mCurrent = _GetRealMinValue();
            for (; mCurrent < _GetRealMaxValue(); mCurrent++) {
                for (auto it = mValues.begin(); it != mValues.end(); ++it) {
                    if (mCurrent != (*it))
                        break;
                }
            }
            break;
        default:
            break;
    }
    setResetFlag(true);
    return mCurrent;
}

int TimeNode::_GetMaxDay(int year, int month)
{
    int day = 30;
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            day = 31;
            break;
        case 2:
            day = (year % 4) ? 28 : 29; /* I can live over 2100 year */
            break;
    }
    return day;
}

int TimeNode:: _GetRealMinValue()
{
    int val;
    switch (mNodeType) {
        case eYear:
            val = 2018;
            break;
        case eDay:
        case eMonth:
            val = 1;
            break;
        default:
            val = 0;
    }
    return val;
}

int TimeNode:: _GetRealMaxValue()
{
    int val;
    switch (mNodeType) {
        case eYear:
            val = 2099;
            break;
        case eDay:
            val = 31;
        case eMonth:
            val = 12;
            break;
        case eWeek:
            val = 6;
            break;
        default:
            val = 59;
    }
    return val;
}

std::string TimeNode::toString()
{
    std::string str;
    switch (valueType()) {
        case eAny:
            str.append("any;");
            break;
        case eRange:
            str.append("range;");
            str.append(int2String(mMin)).append(";");
            str.append(int2String(mMax)).append(";");
            break;
        case eSet:
            str.append("set;");
            for (auto it = mValues.begin(); it != mValues.end(); ++it) {
                str.append(int2String(*it)).append(";");
            }
            break;
        case eNull:
            str.append("null;");
            break;
        case eNot:
            str.append("not;");
            for (auto it = mValues.begin(); it != mValues.end(); ++it) {
                str.append(int2String(*it)).append(";");
            }
            break;
            break;
        default:
            break;
    }
    return std::move(str);
}

TimeNode& TimeNode::resetFromString(const std::string &str)
{
    char *type = strtok((char*)str.c_str(), ";");
    if (!type) {
        RE_LOGW("FIXME!\n");
        return *this;
    }

    if (!strcmp(type, "any")) {
        setValueType(eAny);
    } else if (!strcmp(type, "range")) {
        setValueType(eRange);
        char *min = strtok(NULL, ";");
        char *max = strtok(NULL, ";");
        if (!min || !max) {
            RE_LOGW("FIXME!\n");
            return *this;
        }
        setRange(atoi(min), atoi(max));
    } else if (!strcmp(type, "set")) {
        setValueType(eSet);
        char *set = strtok(NULL, ";");
        while (set) {
            append(atoi(set));
            set = strtok(NULL, ";");
        }
    } else if (!strcmp(type, "null")) {
        setValueType(eNull);
    } else if (!strcmp(type, "not")) {
        setValueType(eNot);
        char *set = strtok(NULL, ";");
        while (set) {
            append(atoi(set));
            set = strtok(NULL, ";");
        }
    }
    return *this;
}

int TimeYear::nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust)
{
    RE_LOGI("adjust = [%d]\n", adjust);
    int val = current();
    if (adjust) {
        while (val < dt.mYear) {
            val = _UpdateValue(2099);
            if (val == RET_NOVAL)
                return RET_RESET;
        }
        RE_LOGD("TimeYear = [%d]\n", val);
    }
    int ret = nextNode()->nextValue(dt, duration, adjust ? val <= dt.mYear : false);
    if (ret == RET_IGNORE)
        return ret;
    switch (ret) {
        case RET_NULL:
        case RET_RESET:
            val = _UpdateValue(2099);
            if (val == RET_NOVAL)
                return RET_RESET;
            if (ret != RET_NULL)
                nextNode()->nextValue(dt, duration, false);
            break;
        case RET_OK:
        default:
            ;
    }
    dt.mYear = current();
    setResetFlag(false);
    if (0 == duration)
        duration = 366 * DAY_SECONDS;
    return RET_OK;
}

int TimeMonth::nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust)
{
    RE_LOGI("adjust = [%d]\n", adjust);
    int val = current();
    if (adjust) {
        while (val < dt.mMonth) {
            val = _UpdateValue(12);
            if (val == RET_NOVAL)
                return RET_RESET;
        }
        RE_LOGD("TimeMonth = [%d]\n", val);
    }
    int ret = nextNode()->nextValue(dt, duration, adjust ? val <= dt.mMonth : false);
    if (ret == RET_IGNORE)
        return ret;
    switch (ret) {
        case RET_NULL:
        case RET_RESET:
            val = _UpdateValue(12);
            if (val == RET_NOVAL)
                return RET_RESET;
            if (ret != RET_NULL)
                nextNode()->nextValue(dt, duration, false);
            break;
        case RET_OK:
        default:
            ;
    }
    dt.mMonth = current();
    setResetFlag(false);
    if (0 == duration)
        duration = _GetMaxDay(dt.mYear, dt.mMonth) * DAY_SECONDS;
    RE_LOGD("TimeMonth = [%d]\n", current());
    return RET_OK;
}

int TimeWeek::nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust)
{
    RE_LOGI("adjust = [%d]\n", adjust);
    int val = current();
    if (adjust) {
        while (val < dt.mDayOfWeek) {
            val = _UpdateValue(6);
            if (val == RET_NOVAL)
                return RET_RESET;
        }
        RE_LOGD("TimeWeek = [%d]\n", val);
    }
    int ret = nextNode()->nextValue(dt, duration, adjust ? val <= dt.mDayOfWeek : false);
    switch (ret) {
        case RET_NULL:
        case RET_RESET:
            val = _UpdateValue(6);
            if (val == RET_NOVAL)
                val = _ResetValue() + 7;
            if (ret != RET_NULL)
                nextNode()->nextValue(dt, duration, false);
            dt = secondsToDateTime(
                dateTimeToSeconds(dt) + (val-dt.mDayOfWeek)*DAY_SECONDS);
            return RET_IGNORE;
        case RET_OK:
        default:
            ;
    }
    dt.mDayOfWeek = current();
    setResetFlag(false);
    if (0 == duration)
        duration = 7 * DAY_SECONDS;
    return RET_OK;
}

int TimeDay::nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust)
{
    RE_LOGI("adjust = [%d]\n", adjust);
    int days = _GetMaxDay(dt.mYear, dt.mMonth);
    int val = current();
    if (adjust) {
        while (val < dt.mDay) {
            val = _UpdateValue(days);
            RE_LOGD("TimeDay [%d] vs [%d]\n", val, dt.mDay);
            if (val == RET_NOVAL)
                return RET_RESET;
        }
        RE_LOGD("TimeDay = [%d]\n", val);
    }
    int ret = nextNode()->nextValue(dt, duration, adjust ? val <= dt.mDay : false);
    switch (ret) {
        case RET_NULL:
        case RET_RESET:
            val = _UpdateValue(days);
            if (val == RET_NOVAL)
                return RET_RESET;
            if (ret != RET_NULL)
                nextNode()->nextValue(dt, duration, false);
            break;
        case RET_OK:
        default:
            ;
    }
    dt.mDay = current();
    setResetFlag(false);
    if (0 == duration)
        duration = DAY_SECONDS;
    return RET_OK;
}

int TimeHour::nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust)
{
    RE_LOGI("adjust = [%d]\n", adjust);
    if (valueType() == eNull) {
        dt.mHour = current();
        return nextNode()->nextValue(dt, duration, adjust);
    }
    int val = current();
    if (adjust) {
        while (val < dt.mHour) {
            val = _UpdateValue(23);
            if (val == RET_NOVAL)
                return RET_RESET;
        }
        RE_LOGD("TimeHour = [%d]\n", val);
    }
    int ret = nextNode()->nextValue(dt, duration, adjust ? val <= dt.mHour : false);
    switch (ret) {
        case RET_NULL:
        case RET_RESET:
            val = _UpdateValue(23);
            if (val == RET_NOVAL)
                return RET_RESET;
            if (ret != RET_NULL)
                nextNode()->nextValue(dt, duration, false);
            break;
        case RET_OK:
        default:
            ;
    }
    dt.mHour = current();
    setResetFlag(false);
    if (0 == duration)
        duration = 3600;
    return RET_OK;
}

int TimeMinute::nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust)
{
    RE_LOGI("adjust = [%d]\n", adjust);
    if (valueType() == eNull) {
        dt.mMinute = current();
        return nextNode()->nextValue(dt, duration, adjust);
    }
    int val = current();
    if (adjust) {
        while (val < dt.mMinute) {
            val = _UpdateValue(59);
            if (val < 0)
                return RET_RESET;
        }
        RE_LOGD("TimeMinute = [%d]\n", val);
    }
    int ret = nextNode()->nextValue(dt, duration, adjust ? val <= dt.mMinute : false);
    switch (ret) {
        case RET_NULL:
        case RET_RESET:
            val = _UpdateValue(59);
            if (val == RET_NOVAL)
                return RET_RESET;
            if (ret != RET_NULL)
                nextNode()->nextValue(dt, duration, false);
            break;
        case RET_OK:
        default:
            ;
    }
    dt.mMinute = current();
    setResetFlag(false);
    if (0 == duration)
        duration = 60;
    return RET_OK;
}

int TimeSecond::nextValue(SysTime::DateTime &dt, time_t &duration, bool adjust)
{
    RE_LOGI("adjust = [%d]\n", adjust);
    if (valueType() == eNull) {
        dt.mSecond = current();
        return RET_NULL;
    }
    int val = current();
    if (adjust) {
        while (val < dt.mSecond) {
            val = _UpdateValue(59);
            RE_LOGD("TimeSecond [%d] vs [%d]\n", val, dt.mSecond);
            if (val == RET_NOVAL)
                return RET_RESET;
        }
        RE_LOGD("TimeSecond = [%d]\n", val);
    }
    val = _UpdateValue(59);
    if (val == RET_NOVAL)
        return RET_RESET;
    dt.mSecond = current();
    setResetFlag(false);
    duration = 1;
    return RET_OK;
}

TimerEvent::TimerEvent(int eventid, bool weekflag)
    : mEventID(eventid), mWeekFlag(weekflag)

{
    /* Year / Month / Week|Day / Hour / Minute / Second */
    mHeader = new TimeYear(eAny);
    TimeNode *node = mHeader;
    node = node->setNextNode(new TimeMonth(eAny));
    node = node->setNextNode(weekflag ? (TimeNode*)(new TimeWeek(eAny)) : (TimeNode*)(new TimeDay(eAny)));
    node = node->setNextNode(new TimeHour(eNull));
    node = node->setNextNode(new TimeMinute(eNull));
    node = node->setNextNode(new TimeSecond(eNull));
}

TimerEvent::~TimerEvent()
{
    RE_LOGTT();
    if (mHeader)
        delete mHeader;
    mHeader = 0;
}

TimeNode* TimerEvent::getTimeNode(TimeNodeType type)
{
    TimeNode *node = mHeader;
    while (node) {
        if (node->type() == type)
            return node;
        node = node->mNextNode;
    }
    return 0;
}

int TimerEvent::nextDate(SysTime::DateTime &dt, time_t &duration)
{
    if (!mHeader)
        return -1;
    int ret = mHeader->nextValue(dt, duration, true);
    if (ret != RET_OK)
        return -1;
    return 0;
}

} /* namespace HB */
