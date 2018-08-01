/***************************************************************************
 *  RingBuffer.h - Ring Buffer Header
 *
 *  Created: 2018-05-31 17:30:05
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RingBuffer_H__
#define __RingBuffer_H__

#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus

namespace UTILS {

class RingBuffer {
public:
    RingBuffer(uint8_t *bufHead, uint32_t bufLength, bool waitLock = false);
    ~RingBuffer();

    int getWriteHead(uint8_t **bufPointer, uint32_t *bufLength);
    int submitWrite(uint8_t *bufPointer, uint32_t bufLength);

    int getReadHead(uint8_t **bufPointer, uint32_t *bufLength);
    int submitRead(uint8_t *bufPointer, uint32_t bufLength);

private:
    uint8_t    *mBufHead;
    uint32_t    mBufLength;
    uint32_t    mWrite;
    uint32_t    mRead;

    bool mWaitLock;
    pthread_cond_t mCond;
    pthread_mutex_t mMutex;

}; /* class RingBuffer */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __RingBuffer_H__ */
