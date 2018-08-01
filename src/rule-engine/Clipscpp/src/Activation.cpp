/***************************************************************************
 *  Activation.cpp - Activation Impl
 *
 *  Created: 2018-06-07 12:40:11
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Activation.h"

extern "C" {
#include "clips.h"
};

#include "Environment.h"

#include <limits.h>

namespace CLIPS {

Activation::Activation(Environment& environment, void *cobj)
#ifndef DEBUG_MEMORY
    : ClipsObject(cobj)
#else
    : ClipsObject("Activation", cobj)
#endif
    , m_environment(environment)
{

}

Activation::pointer Activation::create(Environment &environment, void *cobj)
{
    return Activation::pointer(new Activation(environment, cobj));
}


Activation::~Activation()
{
}

std::string Activation::name()
{
    if (m_cobj)
        return EnvGetActivationName(m_environment.cobj(), m_cobj);
    return std::string();
}

std::string Activation::formatted()
{
    char temp[2001];
    if (m_cobj) {
        EnvGetActivationPPForm(m_environment.cobj(), temp, 2000, m_cobj);
        return temp;
    }
    return std::string();
}

bool Activation::deactivate()
{
    if (m_cobj)
        return EnvDeleteActivation(m_environment.cobj(), m_cobj);
    return false;
}

int Activation::salience()
{
    if (m_cobj)
        return EnvGetActivationSalience(m_environment.cobj(), m_cobj);
    return INT_MIN;
}

int Activation::set_salience(int sal)
{
    if (m_cobj)
        return EnvSetActivationSalience(m_environment.cobj(), m_cobj, sal);
    return INT_MIN;
}

Activation::pointer Activation::next()
{
    void* nxt = NULL;
    if (m_cobj) {
        nxt = EnvGetNextActivation(m_environment.cobj(), m_cobj);
        if (nxt)
            return Activation::create(m_environment, nxt);
    }
    return Activation::pointer();
}

} /* namespace CLIPS */
