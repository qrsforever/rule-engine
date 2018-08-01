/***************************************************************************
 *  LogFilter.h - Log Filter Header
 *
 *  Created: 2018-06-04 10:03:16
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __LogFilter_H__
#define __LogFilter_H__

#include <stdint.h>

#ifdef __cplusplus

namespace UTILS {

class LogFilter {
public:
    LogFilter();
    ~LogFilter();

    virtual bool pushBlock(uint8_t* blockHead, uint32_t blockLength) = 0;

    LogFilter* m_next;
};

} // namespace UTILS

#endif // __cplusplus

#endif // __LogFilter_H__
