/***************************************************************************
 *  Utility.cpp - Utility Impl
 *
 *  Created: 2018-06-05 13:20:57
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Utility.h"
#include "Environment.h"

#include <string>
#include <stdexcept>

extern "C" {
#include "clips.h"
};

namespace CLIPS {

std::vector<std::string> data_object_to_strings(void *cdo)
{
    void* mfptr;
    long int end, i;
    std::string s;
    std::vector<std::string> strings;

    DATA_OBJECT clipsdo = *(DATA_OBJECT*)cdo;

    switch (GetType(clipsdo)) {
    case SYMBOL:
    case INSTANCE_NAME:
    case STRING:
        strings.push_back(DOToString(clipsdo));
        break;
    case MULTIFIELD:
        end = GetDOEnd(clipsdo);
        mfptr = GetValue(clipsdo);
        for (i = GetDOBegin(clipsdo); i <= end; i++) {
            switch (GetMFType(mfptr, i)) {
            case SYMBOL:
            case STRING:
            case INSTANCE_NAME:
                strings.push_back(ValueToString(GetMFValue(mfptr, i)));
                break;
            default:
                break;
            }
        }
    default:
        break;
    }

    return strings;
}

Values data_object_to_values(void *cdo)
{
    Values values;

    std::string s;
    double d;
    long int i;
    void* p;

    void* mfptr;
    long int end;

    DATA_OBJECT clipsdo = *(DATA_OBJECT*)cdo;

    switch (GetType(clipsdo)) {
    case RVOID:
        return values;
    case STRING:
        s = DOToString(clipsdo);
        values.push_back(Value(s, TYPE_STRING));
        return values;
    case INSTANCE_NAME:
        s = DOToString(clipsdo);
        values.push_back(Value(s, TYPE_INSTANCE_NAME));
        return values;
    case SYMBOL:
        s = DOToString(clipsdo);
        values.push_back(Value(s, TYPE_SYMBOL));
        return values;
    case FLOAT:
        d = DOToDouble(clipsdo);
        values.push_back(Value(d));
        return values;
    case INTEGER:
        i = DOToLong(clipsdo);
        values.push_back(Value(i));
        return values;
    case INSTANCE_ADDRESS:
        p = DOToPointer(clipsdo);
        values.push_back(Value(p, TYPE_INSTANCE_ADDRESS));
        return values;
    case EXTERNAL_ADDRESS:
        p = DOToExternalAddress(clipsdo);
        values.push_back(Value(p, TYPE_EXTERNAL_ADDRESS));
        return values;
    case MULTIFIELD:
        end = GetDOEnd(clipsdo);
        mfptr = GetValue(clipsdo);
        for (int iter = GetDOBegin(clipsdo); iter <= end; iter++) {
            switch (GetMFType(mfptr, iter)) {
            case STRING:
                s = ValueToString(GetMFValue(mfptr, iter));
                values.push_back(Value(s, TYPE_STRING));
                break;
            case SYMBOL:
                s = ValueToString(GetMFValue(mfptr, iter));
                values.push_back(Value(s, TYPE_SYMBOL));
                break;
            case FLOAT:
                d = ValueToDouble(GetMFValue(mfptr, iter));
                values.push_back(Value(d));
                break;
            case INTEGER:
                i = ValueToLong(GetMFValue(mfptr, iter));
                values.push_back(Value(i));
                break;
            case EXTERNAL_ADDRESS:
                p = ValueToExternalAddress(GetMFValue(mfptr, iter));
                values.push_back(Value(p, TYPE_EXTERNAL_ADDRESS));
                break;
            default:
                throw std::logic_error("clipsmm::data_object_to_values: Unhandled multifield type");
            }
        }
        return values;
    default:
        throw std::logic_error("clipsmm::data_object_to_values: Unhandled data object type");
    }
}

void *value_to_data_object(const Environment &env, const Value &value)
{
    void *p;
    DATA_OBJECT *clipsdo = new DATA_OBJECT;

    SetpType(clipsdo, value.type());
    switch (value.type()) {
    case TYPE_SYMBOL:
    case TYPE_STRING:
    case TYPE_INSTANCE_NAME:
        p = EnvAddSymbol(env.cobj(),
            const_cast<char*>(value.as_string().c_str())
           );
        SetpValue(clipsdo, p);
        return clipsdo;
    case TYPE_INTEGER:
        p = EnvAddLong(env.cobj(), value.as_integer());
        SetpValue(clipsdo, p);
        return clipsdo;
    case TYPE_FLOAT:
        p = EnvAddDouble(env.cobj(), value.as_float());
        SetpValue(clipsdo, p);
        return clipsdo;
    case TYPE_EXTERNAL_ADDRESS:
        p = EnvAddExternalAddress(env.cobj(), value.as_address(), EXTERNAL_ADDRESS);
        SetpValue(clipsdo, p);
        return clipsdo;
    default:
        throw std::logic_error("clipsmm::value_to_data_object: Unhandled data object type");
    }
    return NULL;
}

void* value_to_data_object(const Environment &env, const Values &values)
{
    void *p, *p2;

    if (values.size() == 0)
        return NULL;

    if (values.size() == 1)
        return value_to_data_object(env, values[0]);

    DATA_OBJECT *clipsdo = new DATA_OBJECT;

    p = EnvCreateMultifield(env.cobj(), values.size());
    for (unsigned int iter = 0; iter < values.size(); iter++) {
        unsigned int mfi = iter + 1; // mfptr indices start at 1
        SetMFType(p, mfi, values[iter].type());
        switch (values[iter].type()) {
        case TYPE_SYMBOL:
        case TYPE_STRING:
        case TYPE_INSTANCE_NAME:
            p2 = EnvAddSymbol(env.cobj(),
                const_cast<char*>(values[iter].as_string().c_str())
               );
            SetMFValue(p, mfi, p2);
            break;
        case TYPE_INTEGER:
            p2 = EnvAddLong(env.cobj(), values[iter].as_integer());
            SetMFValue(p, mfi, p2);
            break;
        case TYPE_FLOAT:
            p2 = EnvAddDouble(env.cobj(), values[iter].as_float());
            SetMFValue(p, mfi, p2);
            break;
        case TYPE_EXTERNAL_ADDRESS:
            p2 = EnvAddExternalAddress(env.cobj(), values[iter].as_address(), EXTERNAL_ADDRESS);
            SetMFValue(p, mfi, p2);
            break;
        default:
            throw std::logic_error("clipsmm::value_to_data_object: Unhandled data object type");
        }
    }
    SetpType(clipsdo, MULTIFIELD);
    SetpValue(clipsdo, p);
    SetpDOBegin(clipsdo, 1);
    SetpDOEnd(clipsdo, values.size());
    return clipsdo;
}

void get_argument(void *env, int argposition, void * &value)
{
    DATA_OBJECT obj;
    EnvRtnUnknown(env, argposition, &obj);
    if (obj.type == EXTERNAL_ADDRESS) {
        value = obj.value;
    }
}

void get_argument(void *env, int argposition, double &value)
{
    value = EnvRtnDouble(env, argposition);
}

void get_argument(void *env, int argposition, float &value)
{
    value = static_cast<float>(EnvRtnDouble(env, argposition));
}

void get_argument(void *env, int argposition, short &value)
{
    value = static_cast<short>(EnvRtnLong(env, argposition));
}

void get_argument(void *env, int argposition, short unsigned &value)
{
    value = static_cast<short unsigned>(EnvRtnLong(env, argposition));
}

void get_argument(void *env, int argposition, int &value)
{
    value = static_cast<int>(EnvRtnLong(env, argposition));
}

void get_argument(void *env, int argposition, unsigned &value)
{
    value = static_cast<unsigned>(EnvRtnLong(env, argposition));
}

void get_argument(void *env, int argposition, long &value)
{
    value = EnvRtnLong(env, argposition);
}

void get_argument(void *env, int argposition, std::string &value)
{
    value = EnvRtnLexeme(env, argposition);
}

void get_argument(void *env, int argposition, Values &values)
{
    DATA_OBJECT arg;
    if (EnvArgTypeCheck(env, (char *)"clipsmm get_argument",
            argposition, MULTIFIELD, &arg) == 0)   return;

    values.clear();

    int end = EnvGetDOEnd(env, arg);
    void *mfp = EnvGetValue(env, arg);
    for (int i = EnvGetDOBegin(env, arg); i <= end; ++i) {
        switch (GetMFType(mfp, i)) {
        case SYMBOL:
        case STRING:
        case INSTANCE_NAME:
            values.push_back(Value(ValueToString(GetMFValue(mfp, i))));
            break;
        case FLOAT:
            values.push_back(Value(ValueToDouble(GetMFValue(mfp, i))));
            break;
        case INTEGER:
            values.push_back(Value(ValueToInteger(GetMFValue(mfp, i))));
            break;
        default:
            continue;
            break;
        }
    }
}

} /* namespace CLIPS */
