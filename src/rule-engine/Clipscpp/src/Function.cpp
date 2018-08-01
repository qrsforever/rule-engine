/***************************************************************************
 *  Function.cpp - Function Impl.
 *
 *  Created: 2018-06-09 16:35:42
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Function.h"

extern "C" {
#include "clips.h"
};

#include "Environment.h"

namespace CLIPS {

Function::Function(Environment &environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("Function", cobj)
#endif
    , m_environment(environment)
{

}

Function::pointer Function::create(Environment &environment, void *cobj)
{
    return Function::pointer(new Function(environment, cobj));
}

Function::~Function()
{
}

std::string Function::name()
{
    if (m_cobj)
        return EnvGetDeffunctionName(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string Function::formatted()
{
    const char *format = 0;
    if (m_cobj) {
        format = EnvGetDeffunctionPPForm(m_environment.cobj(), m_cobj);
        if (format)
            return format;
    }
    return std::string();
}

std::string Function::module_name()
{
    if (m_cobj)
        return EnvDeffunctionModule(m_environment.cobj(), m_cobj);
    return std::string();
}

bool Function::is_watched()
{
    if (m_cobj)
        return EnvGetDeffunctionWatch(m_environment.cobj(), m_cobj);
    return false;
}

void Function::set_watch(bool watch)
{
    if (m_cobj)
        EnvSetDeffunctionWatch(m_environment.cobj(), watch, m_cobj);
}

Function::pointer Function::next()
{
    void *nxt;
    if (m_cobj) {
        nxt = EnvGetNextDeffunction(m_environment.cobj(), m_cobj);
        if (nxt)
            return Function::create(m_environment, nxt);
    }
    return Function::pointer();
}

bool Function::is_deletable()
{
    if (m_cobj)
        return EnvIsDeffunctionDeletable(m_environment.cobj(), m_cobj);
    return false;
}

bool Function::undefine()
{
    if (m_cobj)
        return EnvUndeffunction(m_environment.cobj(), m_cobj);
    return false;
}

} /* namespace CLIPS */
