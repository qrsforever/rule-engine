/***************************************************************************
 *  SysTime.h - System Time Header
 *
 *  Created: 2018-05-31 18:13:16
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __SysTime_H__
#define __SysTime_H__

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus

namespace UTILS {

class SysTime {
public:
    struct DateTime {
        uint16_t mYear;          /* e.g. 2005 */
        uint8_t  mMonth;         /* 1..12 */
        uint8_t  mDayOfWeek;     /* 0..6, 0==Sunday */
        uint8_t  mDay;           /* 1..31 */
        uint8_t  mHour;          /* 0..23 */
        uint8_t  mMinute;        /* 0..59 */
        uint8_t  mSecond;        /* 0..59 */
    };

    static void GetDateTime(DateTime*);
    static uint32_t GetMSecs();

}; /* class SysTime */

time_t dateTimeToSeconds(SysTime::DateTime &dt);
SysTime::DateTime secondsToDateTime(time_t secs);

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* _SysTime_H_ */
