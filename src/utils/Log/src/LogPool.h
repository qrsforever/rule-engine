/***************************************************************************
 *  LogPool.h - Log Pool Header
 *
 *  Created: 2018-06-04 09:56:46
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __LogPool_H__
#define __LogPool_H__

#include "Mutex.h"
#include "Singleton.h"
#include "DataSink.h"
#include "MessageHandler.h"

#ifdef __cplusplus

namespace UTILS {

class LogFilter;

class LogPool : public DataSink, public MessageHandler, public Singleton<LogPool> {
public:
    LogPool();
    ~LogPool();

    virtual bool attachFilter(LogFilter*, int index = 0);
    virtual bool detachFilter(LogFilter*);

    virtual bool onDataArrive();
    virtual bool onError();
	virtual bool onEnd();

protected:
    enum MessageCode {
    	MC_DataArrive,
    	MC_End,
    	MC_Error
    };
    virtual void handleMessage(Message* msg);

    void receiveData();
    void receiveError();
    void receiveEnd();

private:
    Mutex mFilterMutex;
    LogFilter* mFilterHead;
}; /* class LogPool */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __LogPool_H__ */
