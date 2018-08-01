/***************************************************************************
 *  SysTime.cpp - System Time Impl
 *
 *  Created: 2018-05-31 18:14:53
 *
 *  Copyright QRS
 ****************************************************************************/

#include "SysTime.h"

#include <sys/time.h>
#include <stdio.h>

namespace UTILS {

void SysTime::GetDateTime(DateTime* dt)
{
    if (dt) {
        struct timeval current;
        struct tm temp_time;

        if (!gettimeofday(&current, NULL)){
            localtime_r(&current.tv_sec, &temp_time);
            dt->mYear       = temp_time.tm_year + 1900;
            dt->mMonth      = temp_time.tm_mon + 1;
            dt->mDayOfWeek  = temp_time.tm_wday;
            dt->mDay        = temp_time.tm_mday;
            dt->mHour       = temp_time.tm_hour;
            dt->mMinute     = temp_time.tm_min;
            dt->mSecond     = temp_time.tm_sec;
        }
    }
}

uint32_t SysTime::GetMSecs()
{
    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
}

time_t dateTimeToSeconds(SysTime::DateTime &dt)
{
    struct tm timeptr;
    timeptr.tm_year = dt.mYear - 1900;
    timeptr.tm_mon  = dt.mMonth - 1;
    timeptr.tm_mday = dt.mDay;
    timeptr.tm_hour = dt.mHour;
    timeptr.tm_min  = dt.mMinute;
    timeptr.tm_sec  = dt.mSecond;

    return mktime(&timeptr);
}

SysTime::DateTime secondsToDateTime(time_t secs)
{
    struct tm timeptr;
    localtime_r(&secs, &timeptr);
    SysTime::DateTime dt;
    dt.mYear       = timeptr.tm_year + 1900;
    dt.mMonth      = timeptr.tm_mon + 1;
    dt.mDayOfWeek  = timeptr.tm_wday;
    dt.mDay        = timeptr.tm_mday;
    dt.mHour       = timeptr.tm_hour;
    dt.mMinute     = timeptr.tm_min;
    dt.mSecond     = timeptr.tm_sec;
    return dt;
}

} /* namespace UTILS */
