/***************************************************************************
 *  LogSource.cpp - LogSource Impl
 *
 *  Created: 2018-06-04 11:17:02
 *
 *  Copyright QRS
 ****************************************************************************/

#include "LogSource.h"
#include "DataSink.h"
#include "RingBuffer.h"
#include "SysTime.h"

#include <string.h>

namespace UTILS {

SINGLETON_STATIC_INSTANCE(LogSource)

#define MAX_BLOCK_SIZE	256

static const char* textLevel[] = {"Assert : ", "Error! : ", "Warning: ", "Debug : ", "Info: ", "Trace: "};

LogSource::LogSource() : mPrefix(true), mShowThreadID(true), mDataSink(0)
{
}

LogSource::~LogSource()
{
}

bool LogSource::attachSink(DataSink *sink)
{
    Mutex::Autolock _l(&mMutex);
    if (mDataSink)
        return false;
    mDataSink = sink;
    return true;
}

bool LogSource::detachSink(DataSink *sink)
{
    Mutex::Autolock _l(&mMutex);
    if (mDataSink != sink)
        return false;
    mDataSink = 0;
    return true;
}

void LogSource::logVerbose(const char *file, int line, const char *function, int level, const char *fmt, va_list args)
{
    uint8_t *bufPointer;
    uint32_t bufLength;

    if (!mDataSink && mDataSink->getBuffer())
        return;

    Mutex::Autolock _l(&mMutex);

    mDataSink->getBuffer()->getWriteHead(&bufPointer, &bufLength);
    if (bufLength == 0)
        return;

    int dataSize = 8, blockSize, writeLen;

    if (mPrefix) {
        writeLen = logPrefix((char*)bufPointer + dataSize, bufLength - dataSize, file, line, function, level);
        if ((writeLen < 0) || (writeLen >= ((int)bufLength - dataSize))) {
            /* too long */
            return;
        }
        dataSize += writeLen;
    }

    writeLen = vsnprintf((char*)bufPointer + dataSize, bufLength - dataSize, fmt, args);
    if (writeLen < 0)
        return;
    if (writeLen >= ((int)bufLength - dataSize))
        dataSize = bufLength;
    else
        dataSize += writeLen;

    *((int *)(bufPointer + 4)) = dataSize - 8;
    blockSize = (dataSize + 8 + 3) & 0xfffffffc;
    if ((bufLength - blockSize) > MAX_BLOCK_SIZE) {
        *((int *)(bufPointer + 0)) = blockSize;
        mDataSink->getBuffer()->submitWrite(bufPointer, blockSize);
    } else {
        *((int *)(bufPointer + 0)) = bufLength;
        mDataSink->getBuffer()->submitWrite(bufPointer, bufLength);
    }
    if (mDataSink)
        mDataSink->onDataArrive();
}

int LogSource::logPrefix(char *buffer, int length, const char *file, int line, const char *function, int level)
{
    if (!buffer || !file)
        return -1;

    static SysTime::DateTime sDTime;
    SysTime::GetDateTime(&sDTime);

    const char* pFile = strrchr(file, '/');
    if (pFile)
        pFile = pFile + 1;
    else
        pFile = file;

    static struct timespec sTimeSpec;
    clock_gettime(CLOCK_REALTIME, &sTimeSpec);
    if (mShowThreadID)
        return snprintf(buffer, length, "%u | %02d:%02d:%02d.%03d | %s:%d | %s | %s", (unsigned int)pthread_self(),
            sDTime.mHour, sDTime.mMinute, sDTime.mSecond, (int)sTimeSpec.tv_nsec / 1000000,
            pFile, line, function, textLevel[level]);
    else
        return snprintf(buffer, length, "%02d:%02d:%02d.%03d | %s:%d | %s | %s",
            sDTime.mHour, sDTime.mMinute, sDTime.mSecond, (int)sTimeSpec.tv_nsec / 1000000,
            pFile, line, function, textLevel[level]);
}

} /* namespace UTILS */
