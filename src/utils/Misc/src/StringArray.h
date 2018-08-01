/***************************************************************************
 *  StringArray.h - String Array
 *
 *  Created: 2018-06-15 17:51:50
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __StringArray_H__
#define __StringArray_H__

#include "Object.h"
#include <string>
#include <map>

#ifdef __cplusplus

namespace UTILS {

class StringArray : public Object {
public:
    StringArray();
    ~StringArray();

    typedef std::map<int, std::string>::const_iterator DataIt;

    size_t size() const;
    StringArray& put(size_t index, const char *val);
    const char* get(size_t index) const;
    const char* operator[](size_t index) const;

private:
    std::map<int, std::string> mData;
}; /* class StringArray */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __StringArray_H__ */
