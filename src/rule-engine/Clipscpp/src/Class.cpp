/***************************************************************************
 *  Class.cpp - Class Impl
 *
 *  Created: 2018-06-09 16:36:37
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Class.h"

extern "C" {
#include "clips.h"
};

#include "Environment.h"

namespace CLIPS {

Class::Class(Environment &environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("Class", cobj)
#endif
    , m_environment(environment)
{

}

Class::pointer Class::create(Environment &environment, void *cobj)
{
    return Class::pointer(new Class(environment, cobj));
}

Class::~Class()
{
}

std::string Class::name()
{
    if (m_cobj)
        return EnvGetDefclassName(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string Class::formatted()
{
    const char *format = 0;
    if (m_cobj) {
        format = EnvGetDefclassPPForm(m_environment.cobj(), m_cobj);
        if (format)
            return format;
    }
    return std::string();
}

std::string Class::module_name()
{
    if (m_cobj)
        return EnvDefclassModule(m_environment.cobj(), m_cobj);
    return std::string();
}

bool Class::is_watched(int what)
{
    if (m_cobj) {
        if (what == WATCH_INSTANCES)
            return EnvGetDefclassWatchInstances(m_environment.cobj(), m_cobj);
        else
            return EnvGetDefclassWatchSlots(m_environment.cobj(), m_cobj);
    }
    return false;
}

void Class::set_watch(bool watch, int what)
{
    if (m_cobj) {
        if (what == WATCH_INSTANCES)
            EnvSetDefclassWatchSlots(m_environment.cobj(), watch, m_cobj);
        else
            EnvSetDefclassWatchSlots(m_environment.cobj(), watch, m_cobj);
    }
}

bool Class::slot_exists(const std::string &slot_name, bool inheritFlag)
{
    if (m_cobj)
        return EnvSlotExistP(m_environment.cobj(),
            m_cobj,
            slot_name.c_str(),
            inheritFlag ? 1: 0);

    return false;
}

std::vector<std::string> Class::slot_names(bool inheritFlag)
{
    DATA_OBJECT clipsdo;
    if (m_cobj) {
        EnvClassSlots(m_environment.cobj(),
            m_cobj,
            &clipsdo,
            inheritFlag ? 1 : 0);
        return data_object_to_strings(&clipsdo);
    }
    return std::vector<std::string>();
}

Values Class::slot_allowed_values(const std::string &slot_name)
{
    DATA_OBJECT clipsdo;
    if (m_cobj) {
        EnvSlotAllowedValues(m_environment.cobj(),
            m_cobj,
            slot_name.c_str(),
            &clipsdo);
        return data_object_to_values(&clipsdo);
    }
    return Values();
}

Values Class::slot_cardinality(const std::string &slot_name)
{
    DATA_OBJECT clipsdo;
    if (m_cobj) {
        EnvSlotCardinality(m_environment.cobj(),
            m_cobj,
            slot_name.c_str(),
            &clipsdo);
        return data_object_to_values(&clipsdo);
    }
    return Values();
}

Values Class::slot_default_value(const std::string &slot_name)
{
    DATA_OBJECT clipsdo;
    if (m_cobj) {
        EnvSlotDefaultValue(m_environment.cobj(),
            m_cobj,
            slot_name.c_str(),
            &clipsdo);
        return data_object_to_values(&clipsdo);
    }
    return Values();
}

Values Class::slot_types(const std::string &slot_name)
{
    DATA_OBJECT clipsdo;
    if (m_cobj) {
        EnvSlotTypes(m_environment.cobj(),
            m_cobj,
            slot_name.c_str(),
            &clipsdo);
        return data_object_to_values(&clipsdo);
    }
    return Values();
}

Values Class::slot_sources(const std::string &slot_name)
{
    DATA_OBJECT clipsdo;
    if (m_cobj) {
        EnvSlotSources(m_environment.cobj(),
            m_cobj,
            slot_name.c_str(),
            &clipsdo);
        return data_object_to_values(&clipsdo);
    }
    return Values();
}

Values Class::slot_range(const std::string &slot_name)
{
    DATA_OBJECT clipsdo;
    if (m_cobj) {
        EnvSlotRange(m_environment.cobj(),
            m_cobj,
            slot_name.c_str(),
            &clipsdo);
        return data_object_to_values(&clipsdo);
    }
    return Values();
}

Class::pointer Class::next()
{
    void *nxt;
    if (m_cobj) {
        nxt = EnvGetNextDefclass(m_environment.cobj(), m_cobj);
        if (nxt)
            return Class::create(m_environment, nxt);
    }
    return Class::pointer();
}

bool Class::is_deletable()
{
    if (m_cobj)
        return EnvIsDefclassDeletable(m_environment.cobj(), m_cobj);
    return false;
}

bool Class::undefine()
{
    if (m_cobj)
        return EnvUndefclass(m_environment.cobj(), m_cobj);
    return false;
}

} /* namespace CLIPS */
