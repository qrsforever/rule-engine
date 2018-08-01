/***************************************************************************
 *  LogUDP.cpp - Log UDP impl
 *
 *  Created: 2018-07-23 15:23:10
 *
 *  Copyright QRS
 ****************************************************************************/

#include "LogUDP.h"
#include "SysTime.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

namespace UTILS {

LogUDP::LogUDP(const char *addr, int port) : mSockFd(-1)
{
    if (addr) {
        memset(&mRemoteAddr, 0, sizeof(mRemoteAddr));
        mRemoteAddr.sin_family = AF_INET;
        mRemoteAddr.sin_addr.s_addr = inet_addr(addr);
        mRemoteAddr.sin_port = htons(port);
        if ((mSockFd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
            fprintf(stderr, "socket error[%s]\n", strerror(errno));
    }
}

LogUDP::~LogUDP()
{
    if (mSockFd > 0)
        close(mSockFd);
    mSockFd = -1;
}

bool LogUDP::pushBlock(uint8_t* blockHead, uint32_t blockLength)
{
    if (mSockFd > 0)
        sendto(mSockFd, blockHead, blockLength, 0, (struct sockaddr *)&mRemoteAddr, sizeof(struct sockaddr));
    return false;
}

} /* namespace UTILS */
