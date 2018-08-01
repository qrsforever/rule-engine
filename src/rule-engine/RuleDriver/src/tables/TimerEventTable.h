/***************************************************************************
 *  TimerEventTable.h - Timer Event Table header
 *
 *  Created: 2018-07-13 15:05:06
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __TimerEventTable_H__
#define __TimerEventTable_H__

#include "DBTable.h"

#ifdef __cplusplus

namespace HB {

#define TE_FIELD_ID     "ID"
#define TE_FIELD_YEAR   "Year"
#define TE_FIELD_MONTH  "Month"
#define TE_FIELD_DAY    "Day"
#define TE_FIELD_HOUR   "Hour"
#define TE_FIELD_MINUTE "Minute"
#define TE_FIELD_SECOND "Second"
#define TE_FIELD_WEEK   "Week"

struct TimerEventInfo {
    int mID;
    std::string mYear;
    std::string mMonth;
    std::string mDay;
    std::string mHour;
    std::string mMinute;
    std::string mSecond;
    std::string mWeek;
}; /* struct TimerEventInfo */

class TimerEventTable : public DBTable {
public:
    TimerEventTable(SQLiteDatabase &db);
    ~TimerEventTable();

    TableType type() { return TT_TIMER_EVENT; }

    bool deleteByKey(int eID);
    bool updateOrInsert(const TimerEventInfo &info);
    std::vector<TimerEventInfo> getTimerEventInfos();
    bool getTimerEventInfoByKey(int eID, TimerEventInfo &info);

private:

}; /* class TimerEventTable */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __TimerEventTable_H__ */
