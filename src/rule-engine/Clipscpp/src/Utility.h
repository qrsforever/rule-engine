/***************************************************************************
 *  Utility.h - Utility Header
 *
 *  Created: 2018-06-05 13:20:09
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Utility_H__
#define __Utility_H__

#include "Value.h"

#include <stdexcept>
#include <vector>

namespace CLIPS {

class Environment;

std::vector<std::string> data_object_to_strings(void *cdo);
Values data_object_to_values(void *cdo);
void *value_to_data_object(const Environment &env, const Value &value);
void *value_to_data_object(const Environment &env, const Values &values);

void get_argument(void *env, int argposition, double &value);
void get_argument(void *env, int argposition, float &value);
void get_argument(void *env, int argposition, short &value);
void get_argument(void *env, int argposition, short unsigned &value);
void get_argument(void *env, int argposition, int &value);
void get_argument(void *env, int argposition, unsigned &value);
void get_argument(void *env, int argposition, long &value);
void get_argument(void *env, int argposition, std::string &value);
void get_argument(void *env, int argposition, Values &values);

template <typename T_return> inline char get_return_code() {
    throw std::logic_error("clipsmm: Adding function with invalid return type");
}
template <> inline char get_return_code<void *>()      { return 'a'; }
template <> inline char get_return_code<bool>()        { return 'b'; }
template <> inline char get_return_code<char>()        { return 'c'; }
template <> inline char get_return_code<double>()      { return 'd'; }
template <> inline char get_return_code<float>()       { return 'f'; }
template <> inline char get_return_code<int>()         { return 'i'; }
template <> inline char get_return_code<long>()        { return 'l'; }
template <> inline char get_return_code<std::string>() { return 's'; }
template <> inline char get_return_code<void>()        { return 'v'; }
template <> inline char get_return_code<Values>()      { return 'm'; }

template <typename T_return> inline char get_argument_code() {
    throw std::logic_error("clipsmm: Adding function with invalid argument type");
}
template <> inline char get_argument_code<void *>()      { return 'a'; }
template <> inline char get_argument_code<double>()      { return 'd'; }
template <> inline char get_argument_code<float>()       { return 'f'; }
template <> inline char get_argument_code<int>()         { return 'i'; }
template <> inline char get_argument_code<long>()        { return 'l'; }
template <> inline char get_argument_code<std::string>() { return 's'; }
template <> inline char get_argument_code<Values>()      { return 'm'; }

} /* namespace CLIPS */

#endif /* __Utility_H__ */
