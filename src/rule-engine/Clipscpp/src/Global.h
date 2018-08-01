/***************************************************************************
 *  Global.h - Global Header
 *
 *  Created: 2018-06-07 12:45:45
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Global_H__
#define __Global_H__

#include "ClipsObject.h"
#include "Value.h"

namespace CLIPS {

class Environment;

class Global : public ClipsObject {
public:
    typedef std::shared_ptr<Global> pointer;

    Global(Environment &environment, void *cobj = 0);

    static Global::pointer create(Environment &environment, void *cobj = 0);

    ~Global();

    std::string name();

    std::string formatted();

    std::string module_name();

    Values value();

    void set_value(const Values& values);
    void set_value(const Value& values);

    void reset_value();

    bool is_watched();

    void set_watch(bool watch=true);

    bool is_deletable();

    Global::pointer next();

    bool undefine();

protected:
    Environment& m_environment;

}; /* class Global */

} /* namespace CLIPS */

#endif /* __Global_H__ */
