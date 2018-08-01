/***************************************************************************
 *  Common.h - Common api
 *
 *  Created: 2018-07-13 21:32:07
 *
 *  Copyright QRS
 ****************************************************************************/

#ifndef __Common_H__
#define __Common_H__

#include <string>
#include <stdio.h>
#include <vector>

#ifdef __cplusplus

namespace UTILS {

std::string& stringTrim(std::string &text);
std::string double2String(double num);
std::string int2String(int num);
std::vector<std::string> stringSplit(const std::string &text, const std::string &delim);

} /* namespace UTILS */

#endif /* __cplusplus */

#endif /* __Common_H__ */
