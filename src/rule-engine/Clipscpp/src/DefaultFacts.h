/***************************************************************************
 *  DefaultFacts.h - Default Facts Header
 *
 *  Created: 2018-06-07 14:49:58
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef _DefaultFacts_H__
#define _DefaultFacts_H__

#include "ClipsObject.h"

namespace CLIPS {

class Environment;

class DefaultFacts : public ClipsObject {
public:
    typedef std::shared_ptr<DefaultFacts> pointer;

    DefaultFacts(Environment &environment, void *cobj = 0);

    static DefaultFacts::pointer create(Environment &environment, void *cobj = 0);

    ~DefaultFacts();

    std::string name();

    std::string module_name();

    std::string formatted();

    DefaultFacts::pointer next();

    bool is_deletable();

    bool retract();

protected:
    Environment& m_environment;

}; /* class DefaultFacts */

} /* namespace CLIPS */

#endif /* _DefaultFacts_H__ */
