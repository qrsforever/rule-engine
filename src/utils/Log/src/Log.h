/***************************************************************************
 *  Log.h - Log Header
 *
 *  Created: 2018-06-04 13:39:03
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Log_H__
#define __Log_H__

#include <stdio.h>
#include <stdint.h>

#define CRASH() do { \
    *(int *)(uintptr_t)0xbbadbeef = 0; \
    ((void(*)())0)(); \
} while(false)

enum LogLevel {
    LOG_LEVEL_ERROR   = 0x01,
    LOG_LEVEL_WARNING = 0x02,
    LOG_LEVEL_DEBUG  = 0x03,
    LOG_LEVEL_INFO = 0x04,
    LOG_LEVEL_TRACE = 0x05
};

#ifdef NULLDEBUG

#define LOGE(args...) ((void)0)
#define LOGW(args...) ((void)0)
#define LOGD(args...) ((void)0)
#define LOGI(args...) ((void)0)
#define LOGT(args...) ((void)0)

#else /* NULLDEBUG */

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

#define LOGTT()  LOGT("run here!\n")

struct LogModule {
	LogModule(const char*, int&);
	~LogModule();

	const char* m_name;
	int&        m_level;
	LogModule*  m_next;
};

#define _LOGE(level,args...) \
do { \
    if (level >= LOG_LEVEL_ERROR) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_ERROR, args); \
} while(0)

#define _LOGW(level,args...) \
do { \
    if (level >= LOG_LEVEL_WARNING) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_WARNING, args); \
} while(0)

#define _LOGD(level,args...) \
do { \
    if (level >= LOG_LEVEL_DEBUG) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_DEBUG, args); \
} while(0)

#define _LOGI(level,args...) \
do { \
    if (level >= LOG_LEVEL_INFO) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_INFO, args); \
} while(0)

#define _LOGT(level,args...) \
do { \
    if (level >= LOG_LEVEL_TRACE) \
        logVerbose(__FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_TRACE, args); \
} while(0)

#endif /* NDEBUG */

#ifdef __cplusplus
extern "C" {
#endif

void initLogThread();
void logInit();
void logVerbose(const char *file, int line, const char *function, int level, const char *fmt, ...);
int getLogLevel();
void setLogLevel(int level);

int setModuleLogLevel(const char *name, int level);
int getModuleLogLevel(const char *name);
char* getModuleLogNames(char *names, int len);

#ifdef __cplusplus
}
#endif

#endif /* __Log_H__ */
