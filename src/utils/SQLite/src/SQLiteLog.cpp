/***************************************************************************
 *  SQLiteLog.cpp - SQLiteLog Impl
 *
 *  Created: 2018-06-21 10:28:19
 *
 *  Copyright QRS
 ****************************************************************************/

#include "SQLiteLog.h"

#ifdef USE_SQLITE_LOG

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int g_logLevel = LOG_LEVEL_TRACE;

static const char* textLevel[] = {"Assert", "Error!", "Warning", "Debug", "Info", "Trace"};

extern "C"
void logVerbose(const char *file, int line, const char *function, int level, const char *fmt, ...)
{
    static char buffer[2048] = { 0 };
    const char* pFile = strrchr(file, '/');
    if (pFile)
        pFile = pFile + 1;
    else
        pFile = file;
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 2047, fmt, args);
    printf("%s:%d | %s | %s | %s",  pFile, line, function, textLevel[level], buffer);
    va_end(args);
}

extern "C"
void setLogLevel(int level)
{
    g_logLevel = level;
}

#else

int gSqliteModuleLevel = LOG_LEVEL_WARNING;

static LogModule RuleEngineModule("sqlite", gSqliteModuleLevel);

#endif /* USE_SQLITE_LOG */
