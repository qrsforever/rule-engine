/***************************************************************************
 *  Function.h - Function Header
 *
 *  Created: 2018-06-07 15:05:44
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Function_H__
#define __Function_H__

#include "ClipsObject.h"

namespace CLIPS {

class Environment;

class Function : public ClipsObject {
public:
    typedef std::shared_ptr<Function> pointer;

    Function(Environment &environment, void *cobj = 0);

    static Function::pointer create(Environment &environment, void *cobj = 0);

    ~Function();

    std::string name();

    std::string formatted();

    std::string module_name();

    bool is_watched();

    void set_watch(bool watch=true);

    bool is_deletable();

    Function::pointer next();

    bool undefine();

protected:
    Environment& m_environment;

}; /* class Function */

} /* namespace CLIPS */

#endif /* __Function_H__ */
