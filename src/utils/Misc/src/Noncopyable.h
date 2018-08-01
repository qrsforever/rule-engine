/***************************************************************************
 *  Noncopyable.h - Non Copyable Class Header
 *
 *  Created: 2018-05-31 14:22:38
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Nocoyable_H__
#define __Nocoyable_H__

#include <stdio.h>

#ifdef __cplusplus

namespace UTILS {

class Noncopyable {
public:
    Noncopyable() {}

private:
    Noncopyable(const Noncopyable&);
    Noncopyable& operator=(const Noncopyable&);
}; /* class Noncopyable */

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __Nocoyable_H__ */
