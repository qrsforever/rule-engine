/***************************************************************************
 *  Module.h - Module Header
 *
 *  Created: 2018-06-07 12:54:30
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Moudle_H__
#define __Moudle_H__

#include "ClipsObject.h"

namespace CLIPS {

class Environment;

class Module: public ClipsObject {
public:
    typedef std::shared_ptr<Module> pointer;

    Module(Environment &environment, void *cobj = 0);

    static Module::pointer create(Environment &environment, void *cobj = 0);

    ~Module();

    std::string name();

    std::string formatted();

    Module::pointer next();

    Module::pointer set_current();

    void focus();

protected:
    Environment& m_environment;

}; /* class Module */

} /* namespace CLIPS */

#endif /*  __Moudle_H__ */
