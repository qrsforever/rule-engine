/***************************************************************************
 *  DBTable.h - Database Table Base Class Header
 *
 *  Created: 2018-06-26 12:29:15
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __DBTable_H__
#define __DBTable_H__

#include "SQLiteDatabase.h"
#include <string>
#include <vector>

using namespace UTILS;

namespace HB {

typedef enum {
    TT_DEFTEMPLATE = 0,
    TT_DEFCLASS = 1,
    TT_DEFRULE,
    TT_TIMER_EVENT,
} TableType;

class DBTable {
public:
    virtual ~DBTable();

    virtual TableType type() = 0;
    bool init();
    int version();
    const std::string& tableName() const { return mTabName; }

private:
    bool _Update(int currentVersion, int latestVersion);

protected:
    DBTable(SQLiteDatabase &db, const char *tabName);
    SQLiteDatabase& mDB;
	std::vector<std::pair<int, std::string>> mUpdateHistoryList; /* for iterate alter update table */
    std::string mTabName;
}; /* class DBTable */

std::string assignSafe(const char *str);

} /* namespace HB */

#ifdef __cplusplus


#endif /* __cplusplus */

#endif /* __DBTable_H__ */
