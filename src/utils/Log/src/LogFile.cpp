/***************************************************************************
 *  LogFile.cpp - Log File Impl
 *
 *  Created: 2018-06-04 10:08:34
 *
 *  Copyright QRS
 ****************************************************************************/

#include "LogFile.h"
#include "SysTime.h"

#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "/tmp"
#endif

namespace UTILS {

LogFile::LogFile() : mFp(0)
{
    /* TODO time need after ntp sync */
    SysTime::DateTime dt;
    SysTime::GetDateTime(&dt);
    char filePath[128] = { 0 };
    snprintf(filePath, 127, "%s/log.%04d%02d%02d%02d%02d%02d"
        , LOG_FILE_PATH
        , dt.mYear, dt.mMonth, dt.mDay, dt.mHour, dt.mMinute, dt.mSecond);
    mFp = fopen(filePath, "w");
}

LogFile::~LogFile()
{
    if (mFp)
        fclose(mFp);

    mFp = 0;
}

bool LogFile::pushBlock(uint8_t* blockHead, uint32_t blockLength)
{
    if (mFp)
        fwrite(blockHead, blockLength, 1, mFp);
    return false;
}

} /* namespace UTILS */
