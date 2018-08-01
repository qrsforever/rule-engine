/***************************************************************************
 *  ClipsObject.h - Clips Object Header
 *
 *  Created: 2018-06-05 12:54:11
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __ClipsObject_H__
#define __ClipsObject_H__

#include <memory>

/* #define DEBUG_MEMORY */

#ifdef DEBUG_MEMORY
#include <cstdio>
#include <string>
#include "Router.h"
#endif

namespace CLIPS {

class ClipsObject {
public:
    typedef std::shared_ptr<ClipsObject> pointer;

#ifndef DEBUG_MEMORY
    ClipsObject(void *obj = 0) : m_cobj(obj) { }
#else
    ClipsObject(std::string id, void *obj = 0) : __debugID(id), m_cobj(obj)
    {
        LOGD("MEM >>>>>> create(%p), name(%s), cobj(%p)\n", this, __debugID.c_str(), m_cobj);
    }
#endif

    ~ClipsObject()
    {
#ifdef DEBUG_MEMORY
        LOGD("MEM >>>>>> delete(%p), name(%s), cobj(%p)\n", this, __debugID.c_str(), m_cobj);
#endif
        m_cobj = 0;
    }

#ifdef DEBUG_OBJECT
    virtual void debug() { }
#endif

    void* cobj() const { return m_cobj; }

protected:

#ifdef DEBUG_MEMORY
    std::string __debugID;
#endif
    void *m_cobj;

}; /* class ClipsObject */

} /* namespace CLIPS */

#endif /* __ClipsObject_H__ */
