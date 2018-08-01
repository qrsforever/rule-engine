/***************************************************************************
 *  Fact.cpp - Fact Impl
 *
 *  Created: 2018-06-05 12:24:52
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Fact.h"

#include "Environment.h"
#include "Utility.h"
#include "Value.h"

extern "C" {
#include "clips.h"
};

namespace CLIPS {

Fact::Fact(Environment &environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("Fact", cobj)
#endif
    , m_environment(environment)
{
    if (m_cobj)
        EnvIncrementFactCount(m_environment.cobj(), m_cobj);
}

Fact::pointer Fact::create(Environment &environment, void *cobj)
{
    return Fact::pointer(new Fact(environment, cobj));
}

Fact::pointer Fact::create(Environment &environment, Template::pointer temp)
{
    struct fact *f = EnvCreateFact(environment.cobj(), temp->cobj());
    return Fact::pointer(new Fact(environment, f));
}

Fact::~Fact()
{
    if (m_cobj)
        EnvDecrementFactCount(m_environment.cobj(), m_cobj);
}

bool Fact::assign_slot_defaults()
{
    if (m_cobj)
        return EnvAssignFactSlotDefaults(m_environment.cobj(), m_cobj);
    return false;
}

Template::pointer Fact::get_template()
{
   if (!m_cobj)
     return Template::pointer();

   void* tem = EnvFactDeftemplate(m_environment.cobj(), m_cobj);

   if (tem)
     return Template::create(m_environment, tem);
   else
     return Template::pointer();
 }

bool Fact::exists() const
{
    if (m_cobj)
        return EnvFactExistp(m_environment.cobj(), m_cobj);
    return false;
}

long int Fact::index() const
{
    if (m_cobj)
        return EnvFactIndex(m_environment.cobj(), m_cobj);
    return -1;
}

std::vector<std::string> Fact::slot_names()
{
    DATA_OBJECT outdata;

    if (!m_cobj)
        return std::vector<std::string>();

    EnvFactSlotNames(m_environment.cobj(), m_cobj, &outdata);

    return data_object_to_strings(&outdata);
}

Values Fact::slot_value(const std::string &name)
{
    DATA_OBJECT outdata;
    int result;

    if (!m_cobj)
        return Values();

    if (name == "")
        result = EnvGetFactSlot(m_environment.cobj(), m_cobj, NULL, &outdata);
    else
        result = EnvGetFactSlot(m_environment.cobj(), m_cobj, name.c_str(), &outdata);
    if (result)
        return data_object_to_values(&outdata);
    else
        return Values();
}

Fact::pointer Fact::next()
{
    void *next_fact;

    if (!m_cobj)
        return Fact::pointer();

    if (!this->exists())
        return Fact::pointer();

    next_fact = EnvGetNextFact(m_environment.cobj(), m_cobj);
    if (next_fact)
        return Fact::create(m_environment, next_fact);
    else
        return Fact::pointer();
}

bool Fact::retract()
{
    if (!m_cobj)
        return false;
    return EnvRetract(m_environment.cobj(), m_cobj);
}

bool Fact::set_slot(const std::string &slot_name, const Value &value)
{
    DATA_OBJECT *clipsdo = (DATA_OBJECT*)value_to_data_object(m_environment, value);
    if (!clipsdo || !m_cobj) {
        delete clipsdo;
        return false;
    }
    bool rv = EnvPutFactSlot(m_environment.cobj(),
        m_cobj,
        slot_name.c_str(),
        clipsdo);
    delete clipsdo;
    return rv;
}

bool Fact::set_slot(const std::string &slot_name, const Values &values)
{
    DATA_OBJECT *clipsdo = (DATA_OBJECT*)value_to_data_object(m_environment, values);
    if (!clipsdo || !m_cobj) {
        delete clipsdo;
        return false;
    }
    bool rv = EnvPutFactSlot(m_environment.cobj(),
        m_cobj,
        slot_name.c_str(),
        clipsdo);
    delete clipsdo;
    return rv;
}

bool Fact::operator==(const Fact &other) const
{
    return (index() == other.index());
}

bool Fact::operator==(const Fact::pointer &other) const
{
    return (index() == other->index());
}

unsigned int Fact::refcount() const
{
    if (!m_cobj)  return 0;

    struct fact *f = (struct fact *)m_cobj;
    return f->factHeader.busyCount;
}

} /* namespace CLIPS */
