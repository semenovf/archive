/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   May 15, 2013 4:37:44 PM
 *
 * @brief
 */


#include "../include/cwt/bytearray.hpp"
#include "../include/cwt/hash.hpp"

CWT_NS_BEGIN

DLL_API uint_t hash_func(const ByteArray &key, uint_t seed)
{
	return hash_bytes(reinterpret_cast<const byte_t *>(key.constData()), key.size(), seed);
}

CWT_NS_END
