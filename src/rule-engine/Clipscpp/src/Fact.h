/***************************************************************************
 *  Fact.h - Fact Header
 *
 *  Created: 2018-06-05 12:22:50
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Fact_H__
#define __Fact_H__

#include "ClipsObject.h"
#include "Value.h"
#include "Template.h"

#include <string>
#include <vector>

namespace CLIPS {

class Environment;

class Fact: public ClipsObject {
public:
    typedef std::shared_ptr<Fact> pointer;

    Fact(Environment &environment, void *cobj=0);

    static Fact::pointer create(Environment &environment, void *cobj=0);
    static Fact::pointer create(Environment &environment, Template::pointer temp);

    ~Fact();

    bool assign_slot_defaults();

    Template::pointer get_template();

    bool exists() const;

    long int index() const;

    std::vector<std::string> slot_names();

    Values slot_value(const std::string &slot_name);

    Fact::pointer next();

    bool set_slot(const std::string &slot_name, const Values &values);

    bool set_slot(const std::string &slot_name, const Value &value);

    bool retract();

    bool operator==(const Fact &other) const;
    bool operator==(const Fact::pointer &other) const;

    unsigned int refcount() const;

protected:
    Environment& m_environment;

}; /* class Fact */

} /* namespace CLIPS */

#endif /* __Fact_H__ */
