/***************************************************************************
 *  RefCnt.h - Refrence Count Header
 *
 *  Created: 2018-05-31 14:19:36
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __RefCnt_H__
#define __RefCnt_H__

#include "Noncopyable.h"

#ifdef __cplusplus

namespace UTILS {

class RefCnt : Noncopyable {
public:

    RefCnt() : mRefCnt(1) {}

    virtual ~RefCnt() { }

    int getRefCnt() const { return mRefCnt; }

    void ref() const {
        mRefCnt++;
    }

    void unref() const {
        if (mRefCnt-- == 1) {
            mRefCnt = 1;
            delete this;
        }
    }

    void safeRef() const {
        if (this) {
            this->ref();
        }
    }

    void safeUnref() const {
        if (this) {
            this->unref();
        }
    }

private:
    mutable int mRefCnt;
}; /* class RefCnt */

class AutoUnref : Noncopyable {
public:
    AutoUnref(RefCnt* obj) : mObj(obj) {}
    ~AutoUnref();

    RefCnt* get() const { return mObj; }

    bool ref();

    bool unref();

    RefCnt* detach();

private:
    RefCnt*   mObj;
}; /* class AutoUnref */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __RefCnt_H__ */
