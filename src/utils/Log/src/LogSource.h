/***************************************************************************
 *  LogSource.h - LogSource Header
 *
 *  Created: 2018-06-04 11:12:52
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __LogSource_H__
#define __LogSource_H__

#include "Singleton.h"
#include "Mutex.h"

#ifdef __cplusplus

namespace UTILS {

class DataSink;

class LogSource : public Singleton<LogSource> {
public:
    LogSource();
    ~LogSource();
    bool attachSink(DataSink *);
    bool detachSink(DataSink *);
    void logVerbose(const char *file, int line, const char *function, int level, const char *fmt, va_list args);
private:
    int logPrefix(char *buffer, int length, const char *file, int line, const char *function, int level);
    bool mPrefix;
    bool mShowThreadID;
    Mutex mMutex;
    DataSink *mDataSink;
}; /* class LogSource */

} /* namespace UTILS */

#endif /* __cplusplus */
#endif /* __LogSource_H__ */
