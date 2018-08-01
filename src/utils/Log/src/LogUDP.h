/***************************************************************************
 *  LogUDP.h - Log UDP Header
 *
 *  Created: 2018-07-23 15:22:33
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __LogUDP_H__
#define __LogUDP_H__

#include "LogFilter.h"

#include <stdio.h>
#include <netinet/in.h>

#ifdef __cplusplus

namespace UTILS {

class LogUDP : public LogFilter {
public:
    LogUDP(const char *addr, int port);
    virtual ~LogUDP();
    virtual bool pushBlock(uint8_t* blockHead, uint32_t blockLength);
private:
    int mSockFd;
    struct sockaddr_in mRemoteAddr;
}; /* class LogUDP */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __LogUDP_H__ */
