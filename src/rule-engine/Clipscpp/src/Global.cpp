/***************************************************************************
 *  Global.cpp - Global Impl
 *
 *  Created: 2018-06-07 12:48:43
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Global.h"
#include "Environment.h"

extern "C" {
#include "clips.h"
};

namespace CLIPS {

Global::Global(Environment& environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("Global", cobj)
#endif
    , m_environment(environment)
{
}

Global::pointer Global::create(Environment &environment, void *cobj)
{
    return Global::pointer(new Global(environment, cobj));
}

Global::~Global()
{
}

std::string Global::name()
{
    if (m_cobj)
        return EnvGetDefglobalName(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string Global::formatted()
{
    const char *format = 0;
    if (m_cobj) {
        format = EnvGetDefglobalPPForm(m_environment.cobj(), m_cobj);
        if (format)
            return format;
    }
    return std::string();
}

std::string Global::module_name()
{
    if (m_cobj)
        return EnvDefglobalModule(m_environment.cobj(), m_cobj);
    return std::string();
}

Values Global::value()
{
    DATA_OBJECT clips_do;
    if (m_cobj) {
        QGetDefglobalValue(m_environment.cobj(), m_cobj, &clips_do);
        return data_object_to_values(&clips_do);
    }
    return Values();
}

void Global::set_value(const Values& value)
{
    DATA_OBJECT *clips_do;
    if (m_cobj) {
        clips_do = (DATA_OBJECT*)value_to_data_object(m_environment, value);
        QSetDefglobalValue(m_environment.cobj(), (defglobal*)m_cobj, clips_do, false);
        delete clips_do;
    }
}

void Global::set_value(const Value& value)
{
    DATA_OBJECT *clips_do;
    if (m_cobj) {
        clips_do = (DATA_OBJECT*)value_to_data_object(m_environment, value);
        QSetDefglobalValue(m_environment.cobj(), (defglobal*)m_cobj, clips_do, false);
        delete clips_do;
    }
}

void Global::reset_value()
{
    if (m_cobj)
        QSetDefglobalValue(m_environment.cobj(), (defglobal*)m_cobj, 0, true);
}

bool Global::is_watched()
{
    if (m_cobj)
        return EnvGetDefglobalWatch(m_environment.cobj(), m_cobj);
    return false;
}

void Global::set_watch(bool watch)
{
    if (m_cobj)
        EnvSetDefglobalWatch(m_environment.cobj(), watch, m_cobj);
}

Global::pointer Global::next()
{
    void* nxt;
    if (m_cobj) {
        nxt = EnvGetNextDefglobal(m_environment.cobj(), m_cobj);
        if (nxt)
            return Global::create(m_environment, nxt);
    }
    return Global::pointer();
}

bool Global::is_deletable()
{
    if (m_cobj)
        return EnvIsDefglobalDeletable(m_environment.cobj(), m_cobj);
    return false;
}

bool Global::undefine()
{
    if (m_cobj)
        return EnvUndefglobal(m_environment.cobj(), m_cobj);
    return false;
}

} /* namespace CLIPS */
