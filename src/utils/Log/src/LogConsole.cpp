/***************************************************************************
 *  LogConsole.cpp - Log Console Impl
 *
 *  Created: 2018-06-04 10:34:15
 *
 *  Copyright QRS
 ****************************************************************************/

#include "LogConsole.h"

#include <stdio.h>

namespace UTILS {

bool LogConsole::pushBlock(uint8_t* blockHead, uint32_t blockLength)
{
    fwrite(blockHead, blockLength, 1, stdout);
    return false;
}

} /* namespace UTILS */
