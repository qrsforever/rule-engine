/***************************************************************************
 *  LogConsole.h - Log Console Header
 *
 *  Created: 2018-06-04 10:33:15
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __LogConsole_H__
#define __LogConsole_H__

#include "LogFilter.h"

#ifdef __cplusplus

namespace UTILS {

class LogConsole : public LogFilter {
public:
    virtual bool pushBlock(uint8_t* blockHead, uint32_t blockLength);
}; /* class LogConsole */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* _LogConsole_H__ */
