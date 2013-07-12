/**
 * @file   hash.cpp
 * @author wladt
 * @date   Feb 27, 2013 1:30:49 PM
 *
 * @brief
 */

#include "../include/cwt/hash.hpp"
#include "../include/cwt/char.hpp"
#include "../include/cwt/string.hpp"
//CWT_NS_BEGIN

static inline uint_t hash(const byte_t *p, int len, uint_t seed)
{
    uint_t h = seed;

    for (int i = 0; i < len; ++i)
        h = 31 * h + p[i];

    return h;
}

static inline uint_t hash(const cwt::Char *p, int len, uint_t seed)
{
    uint_t h = seed;

    for (int i = 0; i < len; ++i)
        h = 31 * h + p[i].unicode();

    return h;
}

#if QT_VERSION < 0x050000
DLL_API uint_t qHash(const cwt::String &key)
{
	CWT_ASSERT(key.size() <= CWT_INT_MAX);
	return hash(reinterpret_cast<const cwt::Char*>(key.data()), int(key.size()), 0);
}
DLL_API uint_t qHash(const cwt::ByteArray &key)
{
	CWT_ASSERT(key.size() <= CWT_INT_MAX);
	return hash(reinterpret_cast<const byte_t*>(key.data()), int(key.size()), 0);
}
DLL_API uint_t qHash(const uuid_t &key)         { return hash(reinterpret_cast<const byte_t*>(&key), sizeof(key), 0); }

#else

DLL_API uint_t qHash(const cwt::String &key, uint_t seed)
{
	CWT_ASSERT(key.size() <= CWT_INT_MAX);
	return hash(reinterpret_cast<const cwt::Char*>(key.data()), int(key.size()), seed);
}
DLL_API uint_t qHash(const cwt::ByteArray &key, uint_t seed)
{
	CWT_ASSERT(key.size() <= CWT_INT_MAX);
	return hash(reinterpret_cast<const byte_t*>(key.data()), int(key.size()), seed);
}
DLL_API uint_t qHash(const uuid_t &key, uint_t seed)         { return hash(reinterpret_cast<const byte_t*>(&key), sizeof(key), seed); }

#endif

//CWT_NS_END
