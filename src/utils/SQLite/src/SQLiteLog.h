/***************************************************************************
 *  SQLiteLog.h - SQLite Log Header
 *
 *  Created: 2018-06-21 10:24:45
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __SQLiteLog_H__
#define __SQLiteLog_H__

/* #define USE_SQLITE_LOG */

#ifdef USE_SQLITE_LOG

#include <stdio.h>
#include <stdint.h>

enum LogLevel {
    LOG_LEVEL_ERROR   = 0x01,
    LOG_LEVEL_WARNING = 0x02,
    LOG_LEVEL_DEBUG  = 0x03,
    LOG_LEVEL_INFO = 0x04,
    LOG_LEVEL_TRACE = 0x05
};

extern int g_logLevel;
#define LOGE(args...) \
do { \
    if (g_logLevel >= LOG_LEVEL_ERROR) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_ERROR, args); \
} while(0)

#define LOGW(args...) \
do { \
    if (g_logLevel >= LOG_LEVEL_WARNING) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_WARNING, args); \
} while(0)

#define LOGD(args...) \
do { \
    if (g_logLevel >= LOG_LEVEL_DEBUG) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_DEBUG, args); \
} while(0)

#define LOGI(args...) \
do { \
    if (g_logLevel >= LOG_LEVEL_INFO) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_INFO, args); \
} while(0)

#define LOGT(args...) \
do { \
    if (g_logLevel >= LOG_LEVEL_TRACE) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_TRACE, args); \
} while(0)

#define LOGTT() LOGT("run here!\n")

#ifdef __cplusplus
extern "C" {
#endif

void logVerbose(const char *file, int line, const char *function, int level, const char *fmt, ...);
void setLogLevel(int level);

#ifdef __cplusplus
}
#endif

#else  /* USE_SQLITE_LOG */

#include "Log.h"

extern int gSqliteModuleLevel;

#define SQL_LOGE(args...)  _LOGE(gSqliteModuleLevel, args)
#define SQL_LOGW(args...)  _LOGW(gSqliteModuleLevel, args)
#define SQL_LOGD(args...)  _LOGD(gSqliteModuleLevel, args)
#define SQL_LOGI(args...)  _LOGI(gSqliteModuleLevel, args)
#define SQL_LOGT(args...)  _LOGT(gSqliteModuleLevel, args)
#define SQL_LOGTT()  _LOGT(gSqliteModuleLevel, "run here!")

#endif /* USE_SQLITE_LOG */

#endif /* __SQLiteLog_H__ */
