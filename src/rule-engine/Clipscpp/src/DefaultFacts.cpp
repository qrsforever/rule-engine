/***************************************************************************
 *  DefaultFacts.cpp - Default Facts Impl
 *
 *  Created: 2018-06-07 14:52:36
 *
 *  Copyright QRS
 ****************************************************************************/

#include "DefaultFacts.h"

extern "C" {
#include "clips.h"
};

#include "Environment.h"

namespace CLIPS {

DefaultFacts::DefaultFacts(Environment &environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("DefaultFacts", cobj)
#endif
    , m_environment(environment)
{

}

DefaultFacts::pointer DefaultFacts::create(Environment &environment, void *cobj)
{
    return DefaultFacts::pointer(new DefaultFacts(environment, cobj));
}

DefaultFacts::~DefaultFacts()
{

}

std::string DefaultFacts::name()
{
    if (m_cobj)
        return EnvGetDeffactsName(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string DefaultFacts::module_name()
{
    if (m_cobj)
        return EnvDeffactsModule(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string DefaultFacts::formatted()
{
    const char *format = 0;
    if (m_cobj) {
         format = EnvGetDeffactsPPForm(m_environment.cobj(), m_cobj);
         if (format)
             return format;
    }
    return std::string();
}

DefaultFacts::pointer DefaultFacts::next()
{
    void *nxt;
    if (m_cobj) {
        nxt = EnvGetNextDeffacts(m_environment.cobj(), m_cobj);
        if (nxt)
            return DefaultFacts::create(m_environment, nxt);
    }
    return DefaultFacts::pointer();
}

bool DefaultFacts::is_deletable()
{
    if (m_cobj)
        return EnvIsDeffactsDeletable(m_environment.cobj(), m_cobj);
    return false;
}

bool DefaultFacts::retract()
{
    if (m_cobj)
        return EnvUndeffacts(m_environment.cobj(), m_cobj);
    return false;
}

} /* namespace CLIPS */
