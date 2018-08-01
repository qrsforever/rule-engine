/***************************************************************************
 *  DBTable.cpp - Define Table Base Impl
 *
 *  Created: 2018-06-26 12:48:04
 *
 *  Copyright QRS
 ****************************************************************************/

#include "DBTable.h"
#include "SQLiteLog.h"

#include <algorithm>

#define VERSIONS_TABLE_NAME     "versions"
#define VERSIONS_FIELD_NAME     "Name"
#define VERSIONS_FIELD_VERSION  "Version"

#define VERSIONS_TABLE_CREATE "CREATE TABLE "\
    VERSIONS_TABLE_NAME "(" \
        VERSIONS_FIELD_NAME " TEXT UNIQUE NOT NULL PRIMARY KEY, " \
        VERSIONS_FIELD_VERSION " INT)"

#define VERSIONS_TABLE_QUERY_VERSION "SELECT " VERSIONS_FIELD_VERSION " FROM " \
        VERSIONS_TABLE_NAME " WHERE " VERSIONS_FIELD_NAME " = ?"

#define VERSIONS_TABLE_REPLACE_VERSION  "REPLACE INTO "\
    VERSIONS_TABLE_NAME "(" \
        VERSIONS_FIELD_NAME ", " \
        VERSIONS_FIELD_VERSION " ) VALUES(?, ?)"


namespace HB {

DBTable::DBTable(SQLiteDatabase &db, const char *tabName)
    : mDB(db)
    , mTabName(tabName)
{
}

DBTable::~DBTable()
{
    mUpdateHistoryList.clear();
}

bool DBTable::init()
{
    if (!mDB.tableExists(VERSIONS_TABLE_NAME))
        if (!mDB.exec(VERSIONS_TABLE_CREATE))
            return false;

    sort(mUpdateHistoryList.begin(), mUpdateHistoryList.end());
    if (mUpdateHistoryList.size() > 0) {
        std::pair<int, std::string> elem = mUpdateHistoryList.back();
        int current = version();
        if (elem.first > current)
            return _Update(current, elem.first);
    }
    return true;
}

int DBTable::version()
{
    int ver = -1;
    SQLiteResultSet* rs = mDB.query(VERSIONS_TABLE_QUERY_VERSION, SQLText(tableName()));
    if (rs) {
        if (rs->next())
            ver = rs->columnInt(0);
        rs->close();
    }
    return ver;
}

bool DBTable::_Update(int currentVersion, int latestVersion)
{
    int count = (int)mUpdateHistoryList.size();
    for (int i = 0; i < count; ++i) {
        if (i > currentVersion) {
            if (!mDB.exec(mUpdateHistoryList[i].second.c_str()))
                return false;
        }
    }
    return mDB.exec(VERSIONS_TABLE_REPLACE_VERSION, SQLText(tableName()), SQLInt(latestVersion));
}

std::string assignSafe(const char *str)
{
    if (!str)
        return std::string();
    return std::string(str);
}

} /* namespace HB */
