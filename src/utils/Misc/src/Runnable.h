/***************************************************************************
 *  Runnable.h - Runnable Header
 *
 *  Created: 2018-05-31 14:25:01
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RUNNABLE_H__
#define __RUNNABLE_H__

#include "Object.h"

#ifdef __cplusplus

namespace UTILS {

class Runnable : public Object {
public:
    Runnable() {};
    virtual ~Runnable() {};
    virtual void run() = 0;
};

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __RUNNABLE_H__ */
