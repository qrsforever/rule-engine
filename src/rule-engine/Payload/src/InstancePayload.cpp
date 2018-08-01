/***************************************************************************
 *  InstancePayload.cpp - InstancePayload Impl
 *
 *  Created: 2018-06-21 13:54:14
 *
 *  Copyright QRS
 ****************************************************************************/

#include "InstancePayload.h"
#include <string.h>

#define INS_ID_PREFIX "ins-"

namespace HB {

InstancePayload::InstancePayload()
{
}

InstancePayload::~InstancePayload()
{
    mSlots.clear();
}

std::string innerOfInsname(std::string name)
{
    return std::string(INS_ID_PREFIX).append(name);
}

std::string outerOfInsname(std::string name)
{
    int len = strlen(INS_ID_PREFIX);
    if (0 == name.compare(0, len, INS_ID_PREFIX))
        return name.substr(len);
    return name;
}

} /* namespace HB */
