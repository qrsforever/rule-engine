/***************************************************************************
 *  Router.h - Router header for clipsc++
 *
 *  Created: 2018-06-06 19:04:43
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Router_H__
#define __Router_H__

/* #define USE_ROUTER_LOG */

#ifdef USE_ROUTER_LOG

#include <stdio.h>
#include <stdint.h>

enum RouterLevel {
    LOG_LEVEL_ERROR   = 0x01,
    LOG_LEVEL_WARNING = 0x02,
    LOG_LEVEL_DEBUG  = 0x03,
    LOG_LEVEL_INFO = 0x04,
    LOG_LEVEL_TRACE = 0x05
};

#ifdef NDEBUGX

#define LOGE(args...) ((void)0)
#define LOGW(args...) ((void)0)
#define LOGD(args...) ((void)0)
#define LOGI(args...) ((void)0)

#else /* NDEBUGX */

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

#define LOGTT() LOGT("run here!\n");

#endif /* NDEBUGX */

#ifdef __cplusplus
extern "C" {
#endif


void logVerbose(const char *file, int line, const char *function, int level, const char *fmt, ...);
void setLogLevel(int level);

#ifdef __cplusplus
}
#endif

#else

#include "Log.h"

#endif /* USE_ROUTER_LOG */

#include <string>
#include <memory>

namespace CLIPS {

class Environment;

class ILogger {
public:
    typedef std::shared_ptr<ILogger> pointer;

    virtual void clips_loge(const char *str) = 0;
    virtual void clips_logw(const char *str) = 0;
    virtual void clips_logd(const char *str) = 0;
    virtual void clips_logi(const char *str) = 0;
    virtual void clips_logt(const char *str) = 0;
}; /* class LoggerCallback */

class Router {
public:
    Router(Environment &environment, const std::string &name, ILogger::pointer logger);
    ~Router();

    static int s_router_query(void *env, const char *logicalName);
    static int s_router_print(void *env, const char *logicalName, const char *str);
    static int s_router_exit(void *env, int exitCode);

    int initiate();
    int finalize();

    std::string& buffer() { return m_buffer; }
    void loge() { return m_logger->clips_loge(m_buffer.c_str()); }
    void logw() { return m_logger->clips_logw(m_buffer.c_str()); }
    void logd() { return m_logger->clips_logd(m_buffer.c_str()); }
    void logi() { return m_logger->clips_logi(m_buffer.c_str()); }
    void logt() { return m_logger->clips_logt(m_buffer.c_str()); }

private:
    Environment& m_environment;
    std::string m_name;
    std::string m_buffer;
    ILogger::pointer m_logger;
}; /* class Router */

} /* namespace CLIPS */

int init_clips_logger(void *env);
int finalize_clips_logger(void *env);

#endif /* __Router_H__ */
