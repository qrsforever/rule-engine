/***************************************************************************
 *  SQLiteDatabase.h - SQLiteDatabase Header
 *
 *  Created: 2018-06-21 10:01:47
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __SQLiteDatabase_H__
#define __SQLiteDatabase_H__

#include <vector>
#include <string>
#include <map>
#include "SQLiteResultSet.h"
#include "SQLiteValue.h"

#ifdef __cplusplus

namespace UTILS {

class SQLiteDatabase {
public:
    SQLiteDatabase();
    ~SQLiteDatabase();

    bool open(std::string filepath);
    bool close();

    /* save and load: convert between file database and memory database */
    int load(const char *filepath);
    int save(const char *filepath);

    bool cacheSQL(const char  *sql);
    bool uncacheSQL(const char *sql);

    /* sqlite operation: open close update drop insert and so on, all these operations only execute once sqlite3_step() */
    bool exec(const char  *sql);
    bool exec(const char  *sql, const SQLiteValue &v0);
    bool exec(const char  *sql, const SQLiteValue &v0, const SQLiteValue &v1);
    bool exec(const char  *sql, const SQLiteValue *values, int length);

    /* sqlite operation: select, need execute one or more times of sqlite3_step() */
    SQLiteResultSet* query(const char *sql);
    SQLiteResultSet* query(const char *sql, const SQLiteValue &v0);
    SQLiteResultSet* query(const char *sql, const SQLiteValue &v0, const SQLiteValue &v1);
    SQLiteResultSet* query(const char *sql, const SQLiteValue *values, int length);

	bool tableExists(std::string name);

private:
    SQLiteDatabase(const SQLiteDatabase &rhs);
    SQLiteDatabase& operator = (const SQLiteDatabase &rhs);

    void closeOpenedResultSets();
    void uncacheCachesResultSets();
    void resultSetDidfinalize(SQLiteResultSet *resultSet);

    SQLiteResultSet* findClosedCache(const char *sql);
    sqlite3_stmt* stmtPrepare(const char *sql);

    void bindValue(const SQLiteValue& value, int idx, sqlite3_stmt *stmt);
    void bindValues(const SQLiteValue* values, int length, sqlite3_stmt *stmt);

public:
    friend class SQLiteResultSet;
    sqlite3 *mSQLiteDB;
    std::string mSQLiteDBPath;
    std::vector<SQLiteResultSet*> mOpenReusltSets; /* put query result set here when calling query() */
    std::map<std::string, SQLiteResultSet*> mCacheResultSets;
    SQLiteResultSet mReusedResultSet;
}; /* class SQLiteDatabase */

} /* namespace  UTILS */

#endif /* __cplusplus */

#endif /* __SQLiteDatabase_H__ */
