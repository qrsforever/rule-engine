/***************************************************************************
 *  Module.cpp - Module Impl
 *
 *  Created: 2018-06-07 13:01:23
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Module.h"

extern "C" {
#include "clips.h"
};

#include "Environment.h"

namespace CLIPS {

Module::Module(Environment &environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("Module", cobj)
#endif
    , m_environment(environment)
{

}

Module::pointer Module::create(Environment &environment, void *cobj)
{
    return Module::pointer(new Module(environment, cobj));
}

Module::~Module()
{
}

std::string Module::name()
{
    if (m_cobj)
        return EnvGetDefmoduleName(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string Module::formatted()
{
    const char *format = 0;
    if (m_cobj) {
        format = EnvGetDefmodulePPForm(m_environment.cobj(), m_cobj);
        if (format)
            return format;
    }
    return std::string();
}

Module::pointer Module::next()
{
    void *nxt;
    if (m_cobj) {
        nxt = EnvGetNextDefmodule(m_environment.cobj(), m_cobj);
        if (nxt)
            return Module::create(m_environment, nxt);
    }
    return Module::pointer();
}

Module::pointer Module::set_current()
{
    void *old;
    if (m_cobj) {
        old = EnvSetCurrentModule(m_environment.cobj(), m_cobj);
        if (old)
            return Module::create(m_environment, old);
    }
    return Module::pointer();
}

void Module::focus()
{
    if (m_cobj)
        EnvFocus(m_environment.cobj(), m_cobj);
}

} /* namespace CLIPS */

