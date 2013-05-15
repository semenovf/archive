/**
 * @file   char.hpp
 * @author wladt
 * @date   Jan 24, 2013 3:56:14 PM
 *
 * @brief
 */

#include "../include/cwt/char.hpp"
#include "../include/cwt/hash.hpp"

CWT_NS_BEGIN

DLL_API uint_t hash_func(Char key, uint_t seed)
{
	return hash_func(key.unicode(), seed);
}

CWT_NS_END


