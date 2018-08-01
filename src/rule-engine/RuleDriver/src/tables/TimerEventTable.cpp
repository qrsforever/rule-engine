/***************************************************************************
 *  TimerEventTable.cpp - Timer Event Table impl
 *
 *  Created: 2018-07-13 15:05:34
 *
 *  Copyright QRS
 ****************************************************************************/

#include "TimerEventTable.h"
#include "Common.h"
#include "SQLiteLog.h"

#define TIMER_EVENT_TABLE_NAME "timerevent"

namespace HB {

TimerEventTable::TimerEventTable(SQLiteDatabase &db)
    : DBTable(db, TIMER_EVENT_TABLE_NAME)
{
    /* v0: */
    std::string sql0("CREATE TABLE ");
    sql0.append(tableName()).append("(");
    sql0.append(TE_FIELD_ID).append(" INTEGER UNIQUE NOT NULL PRIMARY KEY, ");
    sql0.append(TE_FIELD_YEAR).append(" TEXT, ");
    sql0.append(TE_FIELD_MONTH).append(" TEXT, ");
    sql0.append(TE_FIELD_DAY).append(" TEXT, ");
    sql0.append(TE_FIELD_HOUR).append(" TEXT, ");
    sql0.append(TE_FIELD_MINUTE).append(" TEXT, ");
    sql0.append(TE_FIELD_SECOND).append(" TEXT, ");
    sql0.append(TE_FIELD_WEEK).append(" TEXT)");

    mUpdateHistoryList.push_back(std::make_pair(0, sql0));

    init();
}

TimerEventTable::~TimerEventTable()
{
}

bool TimerEventTable::deleteByKey(int eID)
{
    SQL_LOGTT();
    std::string sql("DELETE FROM ");
    sql.append(tableName()).append(" WHERE ");
    sql.append(TE_FIELD_ID).append(" = ").append(int2String(eID));
    return mDB.exec(sql.c_str());
}

bool TimerEventTable::updateOrInsert(const TimerEventInfo &info)
{
    SQL_LOGTT();
    SQLiteValue values[8];
    values[0] = SQLInt(info.mID);
    values[1] = SQLText(info.mYear);
    values[2] = SQLText(info.mMonth);
    values[3] = SQLText(info.mDay);
    values[4] = SQLText(info.mHour);
    values[5] = SQLText(info.mMinute);
    values[6] = SQLText(info.mSecond);
    values[7] = SQLText(info.mWeek);

    std::string sql("REPLACE INTO ");
    sql.append(tableName()).append("(");
    sql.append(TE_FIELD_ID).append(", ");
    sql.append(TE_FIELD_YEAR).append(", ");
    sql.append(TE_FIELD_MONTH).append(", ");
    sql.append(TE_FIELD_DAY).append(", ");
    sql.append(TE_FIELD_HOUR).append(", ");
    sql.append(TE_FIELD_MINUTE).append(", ");
    sql.append(TE_FIELD_SECOND).append(", ");
    sql.append(TE_FIELD_WEEK).append(") VALUES(?, ?, ?, ?, ?, ?, ?, ?)");

    return mDB.exec(sql.c_str(), values, sizeof(values) / sizeof(values[0]));
}

std::vector<TimerEventInfo> TimerEventTable::getTimerEventInfos()
{
    SQL_LOGTT();
    std::vector<TimerEventInfo> infos;

    std::string sql("SELECT ");
    sql.append(TE_FIELD_ID).append(", ");
    sql.append(TE_FIELD_YEAR).append(", ");
    sql.append(TE_FIELD_MONTH).append(", ");
    sql.append(TE_FIELD_DAY).append(", ");
    sql.append(TE_FIELD_HOUR).append(", ");
    sql.append(TE_FIELD_MINUTE).append(", ");
    sql.append(TE_FIELD_SECOND).append(", ");
    sql.append(TE_FIELD_WEEK).append(" FROM ").append(tableName());

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs) {
        while (rs->next()) {
            TimerEventInfo info;
            info.mID = rs->columnInt(0);
            info.mYear = assignSafe(rs->columnText(1));
            info.mMonth = assignSafe(rs->columnText(2));
            info.mDay = assignSafe(rs->columnText(3));
            info.mHour = assignSafe(rs->columnText(4));
            info.mMinute = assignSafe(rs->columnText(5));
            info.mSecond = assignSafe(rs->columnText(6));
            info.mWeek = assignSafe(rs->columnText(7));
            infos.push_back(std::move(info));
        }
        rs->close();
    }
    return std::move(infos);
}

bool TimerEventTable::getTimerEventInfoByKey(int eID, TimerEventInfo &info)
{
    SQL_LOGTT();

    std::string sql("SELECT ");
    sql.append(TE_FIELD_ID).append(", ");
    sql.append(TE_FIELD_YEAR).append(", ");
    sql.append(TE_FIELD_MONTH).append(", ");
    sql.append(TE_FIELD_DAY).append(", ");
    sql.append(TE_FIELD_HOUR).append(", ");
    sql.append(TE_FIELD_MINUTE).append(", ");
    sql.append(TE_FIELD_SECOND).append(", ");
    sql.append(TE_FIELD_WEEK).append(" FROM ").append(tableName());
    sql.append(" WHERE ").append(TE_FIELD_ID).append(" = ").append(int2String(eID));

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs && rs->next()) {
        info.mID = rs->columnInt(0);
        info.mYear = assignSafe(rs->columnText(1));
        info.mMonth = assignSafe(rs->columnText(2));
        info.mDay = assignSafe(rs->columnText(3));
        info.mHour = assignSafe(rs->columnText(4));
        info.mMinute = assignSafe(rs->columnText(5));
        info.mSecond = assignSafe(rs->columnText(6));
        info.mWeek = assignSafe(rs->columnText(7));
        rs->close();
        return true;
    }
    return false;
}

} /* namespace HB */
