/***************************************************************************
 *  SQLiteResultSet.h - SQLiteResultSet Header
 *
 *  Created: 2018-06-21 09:53:55
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __SQLiteResultSet_H__
#define __SQLiteResultSet_H__

#include "SQLiteValue.h"

#ifdef __cplusplus

namespace UTILS {

class SQLiteDatabase;
class SQLiteResultSet {
public:
    bool next();
    void close() { _closeNeedNoticeDb(true); }
    /* Routines below return information about a single column of the current result row of a query */
    int columnCount()                   const { return sqlite3_column_count(mStmt); }
    double columnDouble(int idx)        const { return sqlite3_column_double(mStmt, idx); }
    int columnInt(int idx)              const { return sqlite3_column_int(mStmt, idx); }
    sqlite3_int64 columnInt64(int idx)  const { return sqlite3_column_int64(mStmt, idx); }
    const char* columnText(int idx)     const { return (const char*)sqlite3_column_text(mStmt, idx); }

    SQLiteBlobValue columnBlob(int idx) const {
        int length = sqlite3_column_bytes(mStmt, idx);
        const void *bytes = sqlite3_column_blob(mStmt, idx);
        SQLiteBlobValue value = { bytes, length };
        return value;
    }

    int columnBytes(int idx)        const { return sqlite3_column_bytes(mStmt, idx); }
    int columnType(int idx)         const { return sqlite3_column_type(mStmt, idx); }
    const char* columnName(int idx) const { return (const char*)sqlite3_column_name(mStmt, idx); }
private:
    SQLiteResultSet();
    SQLiteResultSet& operator = (const SQLiteResultSet &rhs);
    SQLiteResultSet(const SQLiteResultSet &rhs);
    ~SQLiteResultSet()  {}
    void _closeNeedNoticeDb(bool flag);

    bool isOpen() const { return mDB != 0; }
    void open(SQLiteDatabase *db)  { mDB = db; }

private:
    friend class SQLiteDatabase;
    SQLiteDatabase *mDB;
    sqlite3_stmt *mStmt;
    bool mCached; /* record whether result set is in cacheResultSets, if false we need delete the result to free resource */
}; /* SQLiteResultSet */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __SQLiteResultSet_H__ */
