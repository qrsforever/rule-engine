/***************************************************************************
 *  Instance.h - Class Instance Header
 *
 *  Created: 2018-06-09 08:45:44
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Instance_H__
#define __Instance_H__

#include "ClipsObject.h"
#include "Value.h"
#include "Class.h"

#include <string.h>

namespace CLIPS {

class Environment;

class Instance: public ClipsObject {
public:
    typedef std::shared_ptr<Instance> pointer;

    Instance(Environment &environment, void *obj=0);

    static Instance::pointer create(Environment &environment, void *cobj=0);

    std::string name();

    std::string formatted();

    Class::pointer getClass();

    Instance::pointer next();

    Values send(const std::string &msg);

    Values send(const std::string &msg, const std::string &args);

    ~Instance();

    unsigned int refcount() const;

protected:
    Environment& m_environment;

}; /* class Instance */

} /* namespace */

#endif /* __Instance_H__ */
