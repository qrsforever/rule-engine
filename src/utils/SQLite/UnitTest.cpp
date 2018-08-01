/***************************************************************************
 *  UnitTest.cpp - Unite Test for SQLite Encapsulation API
 *
 *  Created: 2018-06-21 11:06:34
 *
 *  Copyright QRS
 ****************************************************************************/

#include "SQLiteDatabase.h"
#include "SQLiteLog.h"

using namespace UTILS;

#define gDB_PATH "output/test.gDB"

static SQLiteDatabase *gDB = 0;

static const char *kUserTable = "CREATE TABLE users"
         "(ID TEXT UNIQUE NOT NULL PRIMARY KEY,"
         "name TEXT,"
         "age INTEGER)";

static const char *kInsertStmt  = "INSERT INTO users(ID, name, age) VALUES(?,?,?)";
static const char *kReplaceStmt = "REPLACE INTO users(ID, name, age) VALUES(?,?,?)";
static const char *kQueryStmt1  = "SELECT * FROM users";
static const char *kQueryStmt2  = "SELECT * FROM users WHERE ID == ?";
static const char *kDeleteStmt  = "DELETE FROM users WHERE ID == ?";
static const char *kUpdateStmt  = "UPDATE users SET age = ? WHERE ID == ?";

/*-----------------------------------------------------------------
 *	SELECT
 *-----------------------------------------------------------------*/
void test_query(const char* stmt)
{
    LOGD("\n========SELECT=============\n");
    SQLiteResultSet *rs = gDB->query(stmt);
    if (rs) {
        while (rs->next()) {
            LOGD("ID: %s\n", rs->columnText(0));
            LOGD("name: %s\n", rs->columnText(1));
            LOGD("age: %d\n", rs->columnInt(2));
        }
        rs->close();
    }
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
#ifndef USE_SQLITE_LOG
    initLogThread();
    setLogLevel(LOG_LEVEL_INFO);
#endif
    gDB = new SQLiteDatabase();
    if (!gDB)
        return -1;

    gDB->open(gDB_PATH);

    /*-----------------------------------------------------------------
    *	CREATE and DROP
    *-----------------------------------------------------------------*/

    LOGD("\n=========CREATE DROP=============\n");
    bool isExist = gDB->tableExists("users");
    LOGD("users table exist: %d\n", isExist);
    if (isExist)
        gDB->exec("DROP TABLE users");
    gDB->exec(kUserTable);

    /*-----------------------------------------------------------------
    *	INSERT and REPLACE
    *-----------------------------------------------------------------*/

    LOGD("\n=========INSERT REPLACE=============\n");
    SQLiteValue values[3];
    values[0] = SQLText("001");
    values[1] = SQLText("aaa");
    values[2] = SQLInt(10);
    gDB->exec(kInsertStmt, values, sizeof(values)/sizeof(values[0]));  /* insert */
    values[0] = SQLText("002");
    values[1] = SQLText("bbb");
    values[2] = SQLInt(20);
    gDB->exec(kReplaceStmt, values, sizeof(values)/sizeof(values[0])); /* insert */
    values[0] = SQLText("002");
    values[1] = SQLText("ccc");
    values[2] = SQLInt(30);
    gDB->exec(kReplaceStmt, values, sizeof(values)/sizeof(values[0])); /* update */

    test_query(kQueryStmt1);

    /*-----------------------------------------------------------------
    *	DELETE
    *-----------------------------------------------------------------*/

    LOGD("\n=========DELETE=============\n");
    gDB->exec(kDeleteStmt, SQLText("002"));

    test_query(kQueryStmt1);

    /*-----------------------------------------------------------------
    *	UPDATE
    *-----------------------------------------------------------------*/

    LOGD("\n=========UPDATE=============\n");
    values[0] = SQLInt(99);
	values[1] = SQLText("001");
    gDB->exec(kUpdateStmt, values[0], values[1]);
    SQLiteResultSet *rs = gDB->query(kQueryStmt2, values[1]);
    if (rs && rs->next()) {
        LOGD("ID: %s\n", rs->columnText(0));
        LOGD("name: %s\n", rs->columnText(1));
        LOGD("age: %d\n", rs->columnInt(2));
        rs->close();
    }

#define VERSIONS_TABLE_NAME     "versions"
#define VERSIONS_FIELD_NAME     "Name"
#define VERSIONS_FIELD_VERSION  "Version"
#if 0 /* error: cannot bind */
#define VERSIONS_TABLE_CREATE   "CREATE TABLE ?(? TEXT UNIQUE NOT NULL PRIMARY KEY, ? INT)"
    values[0] = SQLText(VERSIONS_TABLE_NAME);
    values[1] = SQLText(VERSIONS_FIELD_NAME);
    values[2] = SQLText(VERSIONS_FIELD_VERSION);
    gDB->exec(VERSIONS_TABLE_CREATE,  values, sizeof(values)/sizeof(values[0]));
#else
#define VERSIONS_TABLE_CREATE   "CREATE TABLE "\
    VERSIONS_TABLE_NAME "(" VERSIONS_FIELD_NAME " TEXT UNIQUE NOT NULL PRIMARY KEY, " VERSIONS_FIELD_VERSION " INT)"
    gDB->exec(VERSIONS_TABLE_CREATE);
#endif

    delete gDB;
    return 0;
}
