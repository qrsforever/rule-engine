/***************************************************************************
 *  StringArray.cpp - String Array Impl
 *
 *  Created: 2018-06-15 18:10:00
 *
 *  Copyright QRS
 ****************************************************************************/

#include "StringArray.h"

namespace UTILS {

StringArray::StringArray()
{
}

StringArray::~StringArray()
{
    mData.clear();
}

size_t StringArray::size() const
{
    return mData.size();
}

StringArray& StringArray::put(size_t index, const char *val)
{
    if (val)
        mData.insert(std::map<int, std::string>::value_type(index, val));
    return *this;
}

const char* StringArray::get(size_t index) const
{
    DataIt it = mData.find(index);
    if (it == mData.end())
        return 0;
    return it->second.c_str();
}

const char* StringArray::operator[](size_t index) const
{
    return get(index);
}

} /* namespace  HB */
