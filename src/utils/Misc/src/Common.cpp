/***************************************************************************
 *  Common.cpp - Common api
 *
 *  Created: 2018-07-13 21:33:03
 *
 *  Copyright QRS
 ****************************************************************************/

#include "Common.h"
#include <string.h>

namespace UTILS {

std::string& stringTrim(std::string& text)
{
    if (!text.empty()) {
        text.erase(0, text.find_first_not_of((" \n\r\t\f\v")));
        text.erase(text.find_last_not_of((" \n\r\t\f\v")) + 1);
    }
    return text;
}

std::string int2String(int num)
{
    char id[32] = { 0 };
    sprintf(id, "%d", num);
    return std::string(id);
}

std::string double2String(double num)
{
    char id[64] = { 0 };
    sprintf(id, "%f", num);
    return std::string(id);
}

std::vector<std::string> stringSplit(const std::string &text, const std::string &delim)
{
    std::vector<std::string> ss;
    char *ptr = strtok((char*)text.c_str(), delim.c_str());
    while (ptr) {
        ss.push_back(ptr);
        ptr = strtok(NULL, delim.c_str());
    }
    return std::move(ss);
}


} /* namespace UTILS */
