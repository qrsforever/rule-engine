/***************************************************************************
 *  InstancePayload.h - Instance Payload Header
 *
 *  Created: 2018-06-21 13:52:17
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __InstancePayload_H__
#define __InstancePayload_H__

#include "Payload.h"

#include <string>
#include <vector>

#ifdef __cplusplus

namespace HB {

class InstancePayload : public Payload {
public:
    InstancePayload();
    ~InstancePayload();
    PayloadType type() { return PT_INSTANCE_PAYLOAD; }

    std::string mInsName;
    std::string mClsName;

    struct SlotInfo {
        SlotInfo(std::string name, std::string value)
            : nName(name), nValue(value) {}
        std::string nName;
        std::string nValue;
    };
    std::vector<struct SlotInfo> mSlots;
}; /* class InstancePayload */

std::string innerOfInsname(std::string name);
std::string outerOfInsname(std::string name);

} /* namespace HB */

#endif /* __cplusplus */

#endif /* __InstancePayload_H__ */
