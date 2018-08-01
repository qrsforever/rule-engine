/***************************************************************************
 *  LogPool.cpp - Log Pool Impl
 *
 *  Created: 2018-06-04 10:45:21
 *
 *  Copyright QRS
 ****************************************************************************/

#include "LogPool.h"

#include "LogFilter.h"
#include "RingBuffer.h"
#include "Message.h"

namespace UTILS {

SINGLETON_STATIC_INSTANCE(LogPool)

LogPool::LogPool()
	: mFilterHead(0)
{

}

LogPool::~LogPool()
{

}

bool LogPool::attachFilter(LogFilter* filter, int index)
{
    Mutex::Autolock _l(&mFilterMutex);

    if (!mFilterHead)
        mFilterHead = filter;
    else {
        LogFilter* pre = mFilterHead;
        for (int i = 0; i < index; i++) {
            if (pre->m_next)
                pre = pre->m_next;
            else
                break;
        }
        filter->m_next = pre->m_next;
        pre->m_next = filter;
    }
    return true;
}

bool LogPool::detachFilter(LogFilter* filter)
{
    Mutex::Autolock _l(&mFilterMutex);

    LogFilter* pre = mFilterHead;

    if (!filter || !pre)
        return false;

    if (mFilterHead == filter)
        mFilterHead = filter->m_next;
    else {
        for (; pre; pre = pre->m_next)
            if (pre->m_next == filter) {
                pre->m_next = filter->m_next;
                break;
            }
    }
    return true;
}

bool LogPool::onDataArrive()
{
    return sendEmptyMessage(MC_DataArrive);
}

bool LogPool::onError()
{
    return sendEmptyMessage(MC_Error);
}

bool LogPool::onEnd()
{
    return sendEmptyMessage(MC_End);
}

void LogPool::handleMessage(Message* msg)
{
    switch (msg->what) {
        case MC_DataArrive:
            receiveData();
            break;
		case MC_End:
			receiveEnd();
			break;
        case MC_Error:
            receiveError();
            break;
		default:
            break;
    }
}

void LogPool::receiveData()
{
    uint8_t* bufPointer = 0;
    uint32_t bufLength = 0;
    int blockSize = 0;
    int payloadSize = 0;

    removeMessages(MC_DataArrive);
    while (1) {
        mRingBuffer->getReadHead(&bufPointer, &bufLength);
        if (bufLength == 0)
            break;

        blockSize = *((int *)(bufPointer + 0));
        payloadSize = *((int *)(bufPointer + 4));

        {
            Mutex::Autolock _l(&mFilterMutex);
            LogFilter* tmp = mFilterHead;
            for (; tmp; tmp = tmp->m_next) {
                if (tmp->pushBlock(bufPointer + 8, payloadSize))
                    break;
            }
        }

        mRingBuffer->submitRead(bufPointer, blockSize);
    }
}

void LogPool::receiveError()
{

}

void LogPool::receiveEnd()
{

}

} /* namespace UTILS */
