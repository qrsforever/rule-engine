/***************************************************************************
 *  DataChannel.h - Data Channel Header
 *
 *  Created: 2018-06-14 14:33:48
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __DataChannel_H__
#define __DataChannel_H__

#include "Payload.h"

#include <string>
#include <memory>

#ifdef __cplusplus

namespace HB {

class DataChannel {
public:
    typedef std::shared_ptr<DataChannel> pointer;
    DataChannel() {}
    virtual ~DataChannel() {}

    virtual int init() = 0;
    virtual bool send(int action, std::shared_ptr<Payload> payload) = 0;
}; /* class DataChannel */

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __DataChannel_H__ */
