/***************************************************************************
 *  Any.h - Any from boost
 *
 *  Created: 2018-06-05 21:16:51
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Any_H__
#define __Any_H__

#include <algorithm>
#include <typeinfo>

namespace CLIPS {

class Any {
public:

    Any() : content(0) {}

    template<typename ValueType>
    Any(const ValueType & value)
        : content(new holder<ValueType>(value)) {}

    Any(const Any & other)
        : content(other.content ? other.content->clone() : 0) {}

    ~Any()
    {
        delete content;
    }

public:
    Any &swap(Any &rhs)
    {
        std::swap(content, rhs.content);
        return *this;
    }

    template<typename ValueType>
    Any &operator=(const ValueType &rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    Any &operator=(const Any &rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

public:
    bool empty() const
    {
        return !content;
    }

    const std::type_info &type() const
    {
        return content ? content->type() : typeid(void);
    }

private: // types

    class placeholder {
    public:
        virtual ~placeholder() {}
    public:
        virtual const std::type_info & type() const = 0;
        virtual placeholder * clone() const = 0;
    }; /* class placeholder */

    template<typename ValueType>
    class holder : public placeholder {
    public:
        holder(const ValueType & value) : held(value) {}
    public:
        virtual const std::type_info & type() const
        {
            return typeid(ValueType);
        }
        virtual placeholder * clone() const
        {
            return new holder(held);
        }
    public:
        ValueType held;
    }; /* class holder */

private:
    template<typename ValueType>
    friend ValueType *any_cast(Any *);
    placeholder *content;
};

class bad_any_cast : public std::bad_cast {
public:
    virtual const char * what() const throw()
    {
        return "boost::bad_any_cast: "
            "failed conversion using boost::any_cast";
    }
}; /* class bad_any_cast */

template<typename ValueType>
ValueType * any_cast(Any * operand)
{
    return operand && operand->type() == typeid(ValueType)
        ? &static_cast<Any::holder<ValueType> *>(operand->content)->held
        : 0;
}

template<typename ValueType>
const ValueType * any_cast(const Any *operand)
{
    return any_cast<ValueType>(const_cast<Any *>(operand));
}

template<typename ValueType>
ValueType any_cast(const Any &operand)
{
    const ValueType * result = any_cast<ValueType>(&operand);
    if(!result)
        throw bad_any_cast();
    return *result;
}

} /* namespace CLIPS */

#endif /* __Any_H__ */
