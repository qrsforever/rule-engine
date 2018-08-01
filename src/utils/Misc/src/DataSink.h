/***************************************************************************
 *  DataSink.h - Data Sink Header
 *
 *  Created: 2018-06-04 09:58:55
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __DataSink_H__
#define __DataSink_H__

#include <stdint.h>

#ifdef __cplusplus

namespace UTILS {

class RingBuffer;

class DataSink {
public:
    DataSink();
    ~DataSink();

    void setBuffer(RingBuffer* buffer) { mRingBuffer = buffer; }
    RingBuffer* getBuffer() { return mRingBuffer; }

    virtual bool onStart();
    virtual bool onDataArrive();
    virtual bool onError();
    virtual bool onEnd();

protected:
    RingBuffer* mRingBuffer;
};

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __DataSink_H__ */
