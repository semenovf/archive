/**
 * @file   hash_p.hpp
 * @author wladt
 * @date   Feb 27, 2013 1:39:16 PM
 *
 * @brief
 */

#ifndef __CWT_HASH_P_HPP__
#define __CWT_HASH_P_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/bytearray.hpp>
#include <QtCore/QObject>

#if QT_VERSION < 0x050000
DLL_API uint_t qHash(const cwt::String &key);
DLL_API uint_t qHash(const cwt::ByteArray &key);
DLL_API uint_t qHash(const uuid_t &key);
#else
DLL_API uint_t qHash(const cwt::String &key, uint_t seed);
DLL_API uint_t qHash(const cwt::ByteArray &key, uint_t seed);
DLL_API uint_t qHash(const uuid_t &key, uint_t seed);
#endif

#include <QtCore/QHash>

CWT_NS_BEGIN

template <typename Key, typename T>
class HashImpl : public QHash<Key, T> {
public:
	typedef typename QHash<Key,T>::iterator iterator;
	typedef typename QHash<Key,T>::const_iterator const_iterator;

public:
	HashImpl() : QHash<Key, T>() {}
	HashImpl(const HashImpl<Key, T> &other) : QHash<Key, T>(other) {}
//	~HashImpl() {}
};

CWT_NS_END

#endif /* __CWT_HASH_P_HPP__ */
