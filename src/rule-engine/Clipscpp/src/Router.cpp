/***************************************************************************
 *  Router.cpp - Router impl for clipsc++
 *
 *  Created: 2018-06-06 19:05:10
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Router.h"
#include "Environment.h"

extern "C" {
#include "clips.h"
}


#define LOG_INFO_NAME   "info"
#define LOG_DEBUG_NAME  "debug"
#define LOG_WARN_NAME   "warn"
#define LOG_ERROR_NAME  "error"
#define LOG_TRACE_NAME  "trace"

#ifdef USE_ROUTER_LOG

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
#endif /* USE_ROUTER_LOG */

namespace CLIPS {

Router::Router(Environment &environment, const std::string &name, ILogger::pointer logger)
    : m_environment(environment)
    , m_name(name), m_logger(logger)
{
    initiate();
}

Router::~Router()
{
    m_logger.reset();
    finalize();
}

int Router::s_router_query(void *env, const char *logicalName)
{
    (void)env;
    if (strcmp(logicalName, LOG_INFO_NAME) == 0) return TRUE;
    if (strcmp(logicalName, LOG_DEBUG_NAME) == 0) return TRUE;
    if (strcmp(logicalName, LOG_WARN_NAME) == 0) return TRUE;
    if (strcmp(logicalName, LOG_ERROR_NAME) == 0) return TRUE;
    if (strcmp(logicalName, LOG_TRACE_NAME) == 0) return TRUE;
    if (strcmp(logicalName, WTRACE) == 0) return TRUE;
    if (strcmp(logicalName, STDOUT) == 0) return TRUE;
    if (strcmp(logicalName, WWARNING) == 0) return TRUE;
    if (strcmp(logicalName, WERROR) == 0) return TRUE;
    if (strcmp(logicalName, WDISPLAY) == 0) return TRUE;
    return FALSE;
}

int Router::s_router_exit(void *env, int exitCode)
{
    (void)env;
    (void)exitCode;
    return TRUE;
}

int Router::s_router_print(void *env, const char *logicalName, const char *str)
{
    void *rc = GetEnvironmentRouterContext(env);
    Router *router = static_cast<Router*>(rc);
    if (!router)
        return FALSE;

    if (strrchr(str, '\n') == 0) {
        router->buffer().append(str);
        return TRUE;
    }
    router->buffer().append(str);
    if (strcmp(logicalName, LOG_ERROR_NAME) == 0
        || strcmp(logicalName, WERROR) == 0) {
        router->loge();
    } else if (strcmp(logicalName, LOG_WARN_NAME) == 0
        || strcmp(logicalName, WWARNING) == 0) {
        router->logw();
    } else if (strcmp(logicalName, LOG_DEBUG_NAME) == 0
        || strcmp(logicalName, STDOUT) == 0) {
        router->logd();
    } else if (strcmp(logicalName, LOG_INFO_NAME) == 0
        || strcmp(logicalName, WDISPLAY) == 0) {
        router->logi();
    } else if (strcmp(logicalName, WTRACE) == 0
        || strcmp(logicalName, LOG_TRACE_NAME) == 0) {
        router->logt();
    }
    router->buffer().clear();
    return TRUE;
}

int Router::initiate()
{
    if (!m_logger)
        return -1;
    return EnvAddRouterWithContext(
        m_environment.cobj(),
        m_name.c_str(),
        30/* priority */,
        Router::s_router_query,
        Router::s_router_print,
        0/*getcFunction*/, 0/*ungetcFunction*/,
        Router::s_router_exit,
        (void*)this);
}

int Router::finalize()
{
    if (!m_logger)
        return -1;
    return EnvDeleteRouter(m_environment.cobj(), m_name.c_str());
}

} /* namespace CLIPS */
