/***************************************************************************
 *  LogFile.h - Log File Header
 *
 *  Created: 2018-06-04 10:06:52
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __LogFile_H__
#define __LogFile_H__

#include "LogFilter.h"

#include <stdio.h>

#ifdef __cplusplus

namespace UTILS {

class LogFile : public LogFilter {
public:
    LogFile();
    virtual ~LogFile();
    virtual bool pushBlock(uint8_t* blockHead, uint32_t blockLength);
private:
    FILE *mFp;
}; /* class LogFile */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __LogFile_H__ */
