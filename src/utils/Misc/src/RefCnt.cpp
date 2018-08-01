/***************************************************************************
 *  RefCnt.cpp - Reference Count Impl
 *
 *  Created: 2018-05-31 14:18:41
 *
 *  Copyright QRS
 ****************************************************************************/

#include "RefCnt.h"

namespace UTILS {

AutoUnref::~AutoUnref()
{
    if (mObj) {
        mObj->unref();
    }
}

bool AutoUnref::ref()
{
    if (mObj) {
        mObj->ref();
        return true;
    }
    return false;
}

bool AutoUnref::unref()
{
    if (mObj) {
        mObj->unref();
        mObj = NULL;
        return true;
    }
    return false;
}

RefCnt* AutoUnref::detach()
{
    RefCnt* obj = mObj;
    mObj = NULL;
    return obj;
}

} /* namespace UTILS */
