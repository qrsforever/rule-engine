/***************************************************************************
 *  Rule.cpp - Rule Impl
 *
 *  Created: 2018-06-07 12:57:05
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Rule.h"

extern "C" {
#include "clips.h"
};

#include "Environment.h"

namespace CLIPS {

Rule::Rule(Environment &environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("Rule", cobj)
#endif
    , m_environment(environment)
{
}

Rule::pointer Rule::create(Environment &environment, void *cobj)
{
    return Rule::pointer(new Rule(environment, cobj));
}

Rule::~Rule()
{
}

bool Rule::is_breakpoint_set()
{
    if (m_cobj)
        return EnvDefruleHasBreakpoint(m_environment.cobj(), m_cobj);
    return false;
}

std::string Rule::module_name()
{
    if (m_cobj)
        return EnvDefruleModule(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string Rule::name()
{
    if (m_cobj)
        return EnvGetDefruleName(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string Rule::formatted()
{
    const char *format = 0;
    if (m_cobj) {
        format = EnvGetDefrulePPForm(m_environment.cobj(), m_cobj);
        if (format)
            return format;
    }
    return std::string();
}

bool Rule::activations_watched()
{
    if (m_cobj)
        return EnvGetDefruleWatchActivations(m_environment.cobj(), m_cobj);
    return false;
}

bool Rule::firings_watched()
{
    if (!m_cobj)
        return false;
    return EnvGetDefruleWatchFirings(m_environment.cobj(), m_cobj);
}

Rule::pointer Rule::next()
{
    void *nxt;
    if (m_cobj) {
        nxt = EnvGetNextDefrule(m_environment.cobj(), m_cobj);
        if (nxt)
            return Rule::create(m_environment, nxt);
    }
    return Rule::pointer();
}

bool Rule::is_deletable()
{
    if (m_cobj)
        return EnvIsDefruleDeletable(m_environment.cobj(), m_cobj);
    return false;
}

void Rule::refresh()
{
    if (m_cobj)
        EnvRefresh(m_environment.cobj(), m_cobj);
}

bool Rule::remove_break()
{
    if (m_cobj)
        return EnvRemoveBreak(m_environment.cobj(), m_cobj);
    return false;
}

void Rule::set_break()
{
    if (m_cobj)
        EnvSetBreak(m_environment.cobj(), m_cobj);
}

void Rule::set_watch_activations(bool set)
{
    if (m_cobj)
        EnvSetDefruleWatchActivations(m_environment.cobj(), set, m_cobj);
}

void Rule::set_watch_firings(bool set)
{
    if (m_cobj)
        EnvSetDefruleWatchFirings(m_environment.cobj(), set, m_cobj);
}

bool Rule::retract()
{
    if (m_cobj)
        return EnvUndefrule(m_environment.cobj(), m_cobj);
    return false;
}

} /* namespace CLIPS */
