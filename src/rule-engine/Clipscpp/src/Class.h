/***************************************************************************
 *  Class.h - Class Header
 *
 *  Created: 2018-06-09 16:34
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Class_H__
#define __Class_H__

#include "ClipsObject.h"
#include "Value.h"

#define WATCH_INSTANCES 0
#define WATCH_SLOTS 1

namespace CLIPS {

class Environment;

class Class : public ClipsObject {
public:
    typedef std::shared_ptr<Class> pointer;

    Class(Environment &environment, void *cobj = 0);

    static Class::pointer create(Environment &environment, void *cobj = 0);

    ~Class();

    std::string name();

    std::string module_name();

    std::string formatted();

    bool is_watched(int what = WATCH_INSTANCES);

    void set_watch(bool watch, int what = WATCH_INSTANCES);

    bool slot_exists(const std::string &slot_name, bool inheritFlag = true);

    std::vector<std::string> slot_names(bool inheritFlag = true);

    Values slot_allowed_values(const std::string &slot_name);

    Values slot_cardinality(const std::string &slot_name);

    Values slot_default_value(const std::string &slot_name);

    Values slot_types(const std::string &slot_name);

    Values slot_range(const std::string &slot_name);

    Values slot_sources(const std::string &slot_name);

    Class::pointer next();

    bool is_deletable();

    bool undefine();

protected:
    Environment& m_environment;

}; /* class Class */

} /* namespace CLIPS */

#endif /* __Class_H__ */
