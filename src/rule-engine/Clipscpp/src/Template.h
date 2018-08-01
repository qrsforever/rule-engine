/***************************************************************************
 *  Template.h - Template Header
 *
 *  Created: 2018-06-05 14:45:20
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Template_H__
#define __Template_H__

#include "ClipsObject.h"
#include "Value.h"

#include <vector>

namespace CLIPS {

class Environment;

class Template: public ClipsObject {
public:
    typedef std::shared_ptr<Template> pointer;

    Template(Environment &environment, void *cobj=0);

    static Template::pointer create(Environment &environment, void *cobj=0);

    ~Template();

    std::string name();

    std::string module_name();

    std::string formatted();

    Values slot_allowed_values(const std::string &slot_name);

    Values slot_cardinality(const std::string &slot_name);

    int slot_default_type(const std::string &slot_name);

    Values slot_default_value(const std::string &slot_name);

    Values slot_range(const std::string &slot_name);

    bool slot_exists(const std::string &slot_name);

    bool is_multifield_slot(const std::string &slot_name);

    bool is_single_field_slot(const std::string &slot_name);

    std::vector<std::string> slot_names();

    bool is_watched();

    Template::pointer next();

    bool is_deletable();

    void set_watch(unsigned state);

    bool retract();

protected:
    Environment& m_environment;

}; /* class Template */

} /* namespace CLIPS */

#endif /* __Template_H__ */
