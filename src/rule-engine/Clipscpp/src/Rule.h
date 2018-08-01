/***************************************************************************
 *  Rule.h - Rule Header
 *
 *  Created: 2018-06-07 13:01:58
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Rule_H__
#define __Rule_H__

#include "ClipsObject.h"

namespace CLIPS {

class Environment;

class Rule: public ClipsObject {
public:
    typedef std::shared_ptr<Rule> pointer;

    Rule(Environment &environment, void *cobj = 0);

    static Rule::pointer create(Environment &environment, void *cobj = 0);

    ~Rule();

    bool is_breakpoint_set();

    std::string module_name();

    std::string name();

    std::string formatted();

    bool activations_watched();

    bool firings_watched();

    Rule::pointer next();

    bool is_deletable();

    void refresh();

    bool remove_break();

    void set_break();

    void set_watch_activations(bool set=true);

    void set_watch_firings(bool set=true);

    bool retract();

protected:
    Environment& m_environment;

}; /* class Rule */

} /* namespace CLIPS */

#endif /*  __Rule_H__ */
