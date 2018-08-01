/***************************************************************************
 *  RingBuffer.cpp - Ring Buffer Impl
 *
 *  Created: 2018-05-31 17:37:30
 *
 *  Copyright QRS
 ****************************************************************************/

#include "RingBuffer.h"

namespace UTILS {

RingBuffer::RingBuffer(uint8_t *bufHead, uint32_t bufLength, bool waitLock)
	: mBufHead(bufHead)
	, mBufLength(bufLength)
	, mWrite(0)
	, mRead(0)
	, mWaitLock(waitLock)
{
    if (mWaitLock) {
        pthread_cond_init(&mCond, NULL);
        pthread_mutex_init(&mMutex, NULL);
    }
}

RingBuffer::~RingBuffer()
{
    if (mWaitLock) {
        pthread_mutex_destroy(&mMutex);
        pthread_cond_destroy(&mCond);
    }
}

int RingBuffer::getWriteHead(uint8_t **bufPointer, uint32_t *bufLength)
{
    if (mWaitLock)
        pthread_mutex_lock(&mMutex);
Retry:
    uint32_t bakRead = mRead;
    uint32_t maxWriteLength;
    if (mWrite >= bakRead) {
        maxWriteLength = mBufLength - mWrite;
        if (bakRead == 0)
            maxWriteLength = maxWriteLength - 1;
    } else {
        maxWriteLength = (bakRead - 1) - mWrite;
    }

    if (mWaitLock && !maxWriteLength) {
        pthread_cond_wait(&mCond, &mMutex);
        goto Retry;
    }

   	*bufPointer = mBufHead + mWrite;
   	*bufLength = maxWriteLength;
    if (mWaitLock)
        pthread_mutex_unlock(&mMutex);
    return 0;
}

int RingBuffer::submitWrite(uint8_t *bufPointer, uint32_t bufLength)
{
    if ((mBufHead + mWrite) != bufPointer)
        return -1;

    uint32_t bakRead = mRead;
    uint32_t maxWriteLength;
    if (mWrite >= bakRead) {
        maxWriteLength = mBufLength - mWrite;
        if (bakRead == 0)
            maxWriteLength = maxWriteLength - 1;
    } else {
        maxWriteLength = (bakRead - 1) - mWrite;
    }

    if (maxWriteLength < bufLength)
        return -1;

    mWrite += bufLength;
    if (mWrite == mBufLength)
        mWrite = 0;

    return 0;
}

int RingBuffer::getReadHead(uint8_t **bufPointer, uint32_t *bufLength)
{
    uint32_t bakWrite = mWrite;
    uint32_t maxReadLength;
    if (mRead > bakWrite) {
        maxReadLength = mBufLength - mRead;
    } else {
        maxReadLength = bakWrite - mRead;
    }

   	*bufPointer = mBufHead + mRead;
   	*bufLength = maxReadLength;

    return 0;
}

int RingBuffer::submitRead(uint8_t *bufPointer, uint32_t bufLength)
{
    if ((mBufHead + mRead) != bufPointer)
        return -1;

    if (mWaitLock)
        pthread_mutex_lock(&mMutex);

    uint32_t bakWrite = mWrite;
    uint32_t maxReadLength;
    if (mRead > bakWrite) {
        maxReadLength = mBufLength - mRead;
    } else {
        maxReadLength = bakWrite - mRead;
    }

    int ret = -1;
    if (bufLength <= maxReadLength) {
        mRead += bufLength;
        if (mRead == mBufLength)
            mRead = 0;
        ret = 0;
    }

    if (mWaitLock) {
        if (!ret)
            pthread_cond_signal(&mCond);
        pthread_mutex_unlock(&mMutex);
    }
    return ret;
}

} /* namespace UTILS */
