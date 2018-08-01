/***************************************************************************
 *  StringData.h - String Data Header
 *
 *  Created: 2018-06-14 22:27:50
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __StringData_H__
#define __StringData_H__

#include "Object.h"

#ifdef __cplusplus

namespace UTILS {

class StringData : public Object {
public:
    StringData(const char* data);
    ~StringData();
    int getSize() const { return mSize; }
    const char* getData() const { return mData; }

private:
    char* mData;
    int mSize;
};

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __StringData_H__*/
