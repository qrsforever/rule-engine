/***************************************************************************
 *  Value.h - Value Header
 *
 *  Created: 2018-06-05 13:40:50
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Value_H__
#define __Value_H__

#include <string>
#include <vector>

namespace CLIPS {

typedef enum Type {
  TYPE_FLOAT = 0,
  TYPE_INTEGER = 1,
  TYPE_SYMBOL = 2,
  TYPE_STRING = 3,
  TYPE_EXTERNAL_ADDRESS = 5,
  TYPE_INSTANCE_ADDRESS = 7,
  TYPE_INSTANCE_NAME = 8,
} Type;

class Value {
public:

    Value (Type type);

    Value(float x);

    Value(double x);

    Value(short int x);

    Value(unsigned short int x);

    Value(int x);

    Value(unsigned int x);

    Value(long int x);

    Value(char* x, Type type=TYPE_STRING);

    Value(const std::string& x, Type type=TYPE_STRING);

    Value(void* x, Type type=TYPE_EXTERNAL_ADDRESS);

    Value(const Value& value);

    ~Value();

    double as_float() const;
    long int as_integer() const;
    std::string& as_string() const;
    void* as_address() const;

    Value& set(float x, bool change_type=false);
    Value& set(double x, bool change_type=false);
    Value& set(short int x, bool change_type=false);
    Value& set(unsigned short int x, bool change_type=false);
    Value& set(int x, bool change_type=false );
    Value& set(unsigned int x, bool change_type=false);
    Value& set(long int x, bool change_type=false );
    Value& set(const std::string& x, bool change_type=false, Type type=TYPE_STRING);
    Value& set(const char* x, bool change_type=false, Type type=TYPE_STRING);
    Value& set(void* x, bool change_type=false, Type type=TYPE_EXTERNAL_ADDRESS);

    operator float() const;
    operator double() const;
    operator short int() const;
    operator unsigned short int() const;
    operator int() const;
    operator unsigned int() const;
    operator long int() const;
    operator std::string&() const;
    operator const char*() const;
    operator void*() const;
    size_t size() const;

    Value& operator=(float x);
    Value& operator=(double x);
    Value& operator=(short int x);
    Value& operator=(unsigned short int x);
    Value& operator=(int x);
    Value& operator=(unsigned int x);
    Value& operator=(long int x);
    Value& operator=(const std::string& x);
    Value& operator=(const char* x);
    Value& operator=(void* x);
    Value& operator=(const Value& x);

    bool operator==(float x) const;
    bool operator==(double x) const;
    bool operator==(short int x) const;
    bool operator==(unsigned short int x) const;
    bool operator==(int x) const;
    bool operator==(unsigned int x) const;
    bool operator==(long int x) const;
    bool operator==(const std::string& x) const;
    bool operator==(const char* x) const;
    bool operator==(void* x) const;

    bool operator!=(float x) const;
    bool operator!=(double x) const;
    bool operator!=(short int x) const;
    bool operator!=(unsigned short int x) const;
    bool operator!=(int x) const;
    bool operator!=(unsigned int x) const;
    bool operator!=(long int x) const;
    bool operator!=(const std::string& x) const;
    bool operator!=(const char* x) const;
    bool operator!=(void* x) const;

    Type type() const;

    Type set_type(Type type);

protected:
    void *m_value;

    Type m_clips_type;

    void deallocate_storage();
}; /* class Value */

typedef std::vector<Value> Values;

} /* namespace Value */

#endif /* __Value_H__ */
