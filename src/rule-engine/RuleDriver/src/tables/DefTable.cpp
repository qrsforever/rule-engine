/***************************************************************************
 *  DefTable.cpp - Define Table Base Impl
 *
 *  Created: 2018-06-26 12:48:04
 *
 *  Copyright QRS
 ****************************************************************************/

#include "DefTable.h"
#include "SQLiteLog.h"

namespace HB {

DefTable::DefTable(SQLiteDatabase &db, const char *tabName)
    : DBTable(db, tabName)
{
    /* v0: create base table */
    std::string sql0("CREATE TABLE ");
    sql0.append(tableName()).append("(");
    sql0.append(DEF_FIELD_DEFNAME).append(" TEXT UNIQUE NOT NULL PRIMARY KEY, ");
    sql0.append(DEF_FIELD_VERSION).append(" TEXT, ");
    sql0.append(DEF_FIELD_CLPPATH).append(" TEXT, ");
    sql0.append(DEF_FIELD_RAWDATA).append(" TEXT)");
    mUpdateHistoryList.push_back(std::make_pair(0, sql0));
}

DefTable::~DefTable()
{
}

bool DefTable::deleteByKey(const std::string &defName)
{
    SQL_LOGTT();
    std::string sql("DELETE FROM ");
    sql.append(tableName()).append(" WHERE ");
    sql.append(DEF_FIELD_DEFNAME).append(" = '").append(defName).append("'");
    return mDB.exec(sql.c_str());
}

bool DefTable::updateOrInsert(const DefInfo &info)
{
    SQL_LOGTT();
    SQLiteValue values[3];
    values[0] = SQLText(info.mDefName);
    values[1] = SQLText(info.mVersion);
    values[2] = SQLText(info.mFilePath);

    std::string sql("REPLACE INTO ");
    sql.append(tableName()).append("(");
    sql.append(DEF_FIELD_DEFNAME).append(", ");
    sql.append(DEF_FIELD_VERSION).append(", ");
    sql.append(DEF_FIELD_CLPPATH).append(") VALUES(?, ?, ?)");
    return mDB.exec(sql.c_str(), values, sizeof(values) / sizeof(values[0]));
}

std::string DefTable::getVersion(std::string defName)
{
    SQL_LOGTT();
    std::string ver("");
    std::string sql("SELECT ");
    sql.append(DEF_FIELD_VERSION).append(" FROM ").append(tableName());
    sql.append(" WHERE ").append(DEF_FIELD_DEFNAME).append(" = '").append(defName).append("'");

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs) {
        if (rs->next())
            ver = assignSafe(rs->columnText(0));
        rs->close();
    }
    return std::move(ver);
}

std::string DefTable::getFilePath(std::string defName)
{
    SQL_LOGTT();
    std::string filepath("");
    std::string sql("SELECT ");
    sql.append(DEF_FIELD_CLPPATH).append(" FROM ").append(tableName());
    sql.append(" WHERE ").append(DEF_FIELD_DEFNAME).append(" = '").append(defName).append("'");

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs) {
        if (rs->next())
            filepath = assignSafe(rs->columnText(0));
        rs->close();
    }
    return std::move(filepath);
}

std::vector<DefInfo> DefTable::getDefInfos()
{
    SQL_LOGTT();
    std::vector<DefInfo> infos;
    std::string sql("SELECT ");
    sql.append(DEF_FIELD_DEFNAME).append(", ");
    sql.append(DEF_FIELD_VERSION).append(", ");
    sql.append(DEF_FIELD_CLPPATH).append(" FROM ").append(tableName());

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs) {
        while (rs->next()) {
            DefInfo info;
            info.mDefName = assignSafe(rs->columnText(0));
            info.mVersion = assignSafe(rs->columnText(1));
            info.mFilePath = assignSafe(rs->columnText(2));
            infos.push_back(info);
        }
        rs->close();
    }
    return std::move(infos);
}

std::vector<std::string> DefTable::getFilePaths()
{
    SQL_LOGTT();
    std::vector<std::string> files;
    std::string sql("SELECT ");
    sql.append(DEF_FIELD_CLPPATH).append(" FROM ").append(tableName());

    SQLiteResultSet *rs = mDB.query(sql.c_str());
    if (rs) {
        while (rs->next())
            files.push_back(assignSafe(rs->columnText(0)));
        rs->close();
    }
    return std::move(files);
}

#ifdef TABLE_DEBUG
void DefTable::showTable()
{
    std::vector<DefInfo> infos = getDefInfos();
    SQL_LOGD("Table[%s]:\n", tableName().c_str());
    for (size_t i = 0; i < infos.size(); ++i) {
        SQL_LOGD("\t %s:[%s] %s[%s] %s[%s]\n",
            DEF_FIELD_DEFNAME, infos[i].mDefName.c_str(),
            DEF_FIELD_VERSION, infos[i].mVersion.c_str(),
            DEF_FIELD_CLPPATH, infos[i].mFilePath.c_str());
    }
}
#endif


} /* namespace HB */
