/***************************************************************************
 *  Activation.h - Activation Header
 *
 *  Created: 2018-06-07 12:37:16
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Activation_H__
#define __Activation_H__

#include "ClipsObject.h"

#include <string>

namespace CLIPS {

class Environment;

class Activation : public ClipsObject {
public:
    typedef std::shared_ptr<Activation> pointer;

    Activation(Environment &environment, void *cobj = 0);

    static Activation::pointer create(Environment &environment, void *cobj = 0);

    ~Activation();

    std::string name();

    std::string formatted();

    bool deactivate();

    int salience();

    int set_salience(int sal);

    Activation::pointer next();

protected:
    Environment& m_environment;

}; /* class Activation */

} /* namespace CLIPS */

#endif /* __Activation_H__ */
