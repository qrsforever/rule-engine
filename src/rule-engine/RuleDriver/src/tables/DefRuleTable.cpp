/***************************************************************************
 *  DefRuleTable.cpp - Define Rule Table Impl
 *
 *  Created: 2018-06-26 15:57:07
 *
 *  Copyright QRS
 ****************************************************************************/

#include "DefRuleTable.h"
#include "SQLiteLog.h"

#define DEFRULE_TABLE_NAME     "defrule"

#define DEF_FIELD_ENABLE    "Enable"
#define DEF_FIELD_TIMERS    "Timers"

namespace HB {

DefRuleTable::DefRuleTable(SQLiteDatabase &db)
    : DefTable(db, DEFRULE_TABLE_NAME)
{
    /* v1: add "Enable" field, default: 1 */
	std::string sql1 = "ALTER TABLE ";
    sql1.append(DEFRULE_TABLE_NAME).append(" ADD ").append(DEF_FIELD_ENABLE).append(" INTEGER DEFAULT 1;");

    /* v2: add "Timers : 001;002;003 " */
	std::string sql2 = "ALTER TABLE ";
    sql2.append(DEFRULE_TABLE_NAME).append(" ADD ").append(DEF_FIELD_TIMERS).append(" TXT;");

    mUpdateHistoryList.push_back(std::make_pair(1, sql1));
    mUpdateHistoryList.push_back(std::make_pair(2, sql2));

    init();
}

DefRuleTable::~DefRuleTable()
{

}

bool DefRuleTable::updateOrInsert(const DefRuleInfo &info)
{
    SQL_LOGTT();
    SQLiteValue values[6];
    values[0] = SQLText(info.mDefName);
    values[1] = SQLText(info.mVersion);
    values[2] = SQLText(info.mFilePath);
    values[3] = SQLText(info.mRawData);
    values[4] = SQLInt(info.mEnable);
    values[5] = SQLText(info.mTimers);

    std::string sql("REPLACE INTO ");
    sql.append(tableName()).append("(");
    sql.append(DEF_FIELD_DEFNAME).append(", ");
    sql.append(DEF_FIELD_VERSION).append(", ");
    sql.append(DEF_FIELD_CLPPATH).append(", ");
    sql.append(DEF_FIELD_RAWDATA).append(", ");
    sql.append(DEF_FIELD_ENABLE).append(", ");
    sql.append(DEF_FIELD_TIMERS).append(") VALUES(?, ?, ?, ?, ?, ?)");
    return mDB.exec(sql.c_str(), values, sizeof(values) / sizeof(values[0]));
}

std::vector<DefRuleInfo> DefRuleTable::getDefRuleInfos()
{
    SQL_LOGTT();
    std::vector<DefRuleInfo> infos;

    std::string sql("SELECT ");
    sql.append(DEF_FIELD_DEFNAME).append(", ");
    sql.append(DEF_FIELD_VERSION).append(", ");
    sql.append(DEF_FIELD_CLPPATH).append(", ");
    sql.append(DEF_FIELD_RAWDATA).append(", ");
    sql.append(DEF_FIELD_ENABLE).append(", ");
    sql.append(DEF_FIELD_TIMERS).append(" FROM ").append(tableName());

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs) {
        while (rs->next()) {
            DefRuleInfo info;
            info.mDefName = assignSafe(rs->columnText(0));
            info.mVersion = assignSafe(rs->columnText(1));
            info.mFilePath = assignSafe(rs->columnText(2));
            info.mRawData = assignSafe(rs->columnText(3));
            info.mEnable = rs->columnInt(4);
            info.mTimers = assignSafe(rs->columnText(5));
            infos.push_back(std::move(info));
        }
        rs->close();
    }
    return std::move(infos);
}

std::vector<std::string> DefRuleTable::getFilePaths()
{
    SQL_LOGTT();
    std::vector<std::string> files;
    std::string sql("SELECT ");
    sql.append(DEF_FIELD_CLPPATH).append(" FROM ").append(tableName());
    sql.append(" WHERE ").append(DEF_FIELD_ENABLE).append(" = ").append("1");

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs) {
        while (rs->next())
            files.push_back(assignSafe(rs->columnText(0)));
        rs->close();
    }
    return std::move(files);
}

bool DefRuleTable::updateForEnable(const std::string &ruleId, bool value)
{
    std::string sql;
    sql.append("UPDATE ").append(tableName()).append(" SET ");
    sql.append(DEF_FIELD_ENABLE).append(" = ").append(value ? "1" : "0");
    sql.append(" WHERE ").append(DEF_FIELD_DEFNAME).append(" = '").append(ruleId).append("'");
    return mDB.exec(sql.c_str());
}

bool DefRuleTable::updateForTimers(const std::string &ruleId, const std::string &value)
{
    std::string sql;
    sql.append("UPDATE ").append(tableName()).append(" SET ");
    sql.append(DEF_FIELD_TIMERS).append(" = '").append(value).append("'");
    sql.append(" WHERE ").append(DEF_FIELD_DEFNAME).append(" = '").append(ruleId).append("'");
    return mDB.exec(sql.c_str());
}

std::string DefRuleTable::getTimersByKey(const std::string &ruleId)
{
    return std::string();
}

} /* namespace HB */
