/**
 * @file   string.cpp
 * @author wladt
 * @date   Jan 24, 2013 2:32:45 PM
 *
 * @brief
 */

#include "../../include/cwt/string.hpp"
#include "../../include/cwt/hash.hpp"
#include <cstring>

CWT_NS_BEGIN

DLL_API uint_t hash_func(const String &key, uint_t seed)
{
    return hash_chars(key.unicode(), key.size(), seed);
}

CWT_NS_END
