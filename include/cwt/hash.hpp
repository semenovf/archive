/**
 * @file   hash.hpp
 * @author wladt
 * @date   Feb 6, 2013 10:20:03 AM
 *
 * @brief
 */

#ifndef __CWT_HASH_HPP__
#define __CWT_HASH_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/bytearray.hpp>
#include <QtCore/QHash>

CWT_NS_BEGIN

#if QT_VERSION < 0x050000
inline uint qHash(const String &key) { return qHash(key.__impl); }
inline uint qHash(const ByteArray &key) { return qHash(key.__impl); }
#else
inline uint qHash(const String &key, uint seed = 0) { return qHash(key.__impl, seed); }
inline uint qHash(const ByteArray &key, uint seed = 0) { return qHash(key.__impl, seed); }
#endif

template <typename Key, typename T>
class DLL_API Hash {
public:
	typedef typename QHash<Key,T>::iterator iterator;
	typedef typename QHash<Key,T>::const_iterator const_iterator;

public:
	Hash() : __impl() {}
	Hash(const Hash<Key, T> &other) : __impl(other.__impl) {}
	~Hash() {}

	iterator       begin() { return __impl.begin(); }
	const_iterator begin() const { return __impl.begin(); }
	iterator       end() { return __impl.end(); }
	const_iterator end() const { return __impl.end(); }
	void	       clear() { __impl.clear(); }
	iterator	   find(const Key & key) { return __impl.find(key); }
	const_iterator find(const Key & key) const { return __impl.find(key); }
	bool	       isEmpty() const { return __impl.isEmpty(); }
	iterator       insert(const Key & key, const T & value) { return __impl.insert(key, value); }
	int	           remove(const Key & key) { return __impl.remove(key); }
	int  	       size() const { return __impl.size(); }
	const T	       value(const Key & key) const { return __impl.value(key); }
	const T	       value(const Key & key, const T &defaultValue) const { return __impl.value(key); }

	bool	       operator!=(const QHash<Key, T> & other) const { return __impl.operator !=(other.__impl); }
	Hash<Key, T>&  operator=(const QHash<Key, T> & other) { __impl.operator =(other.__impl); return *this; }
	bool	       operator==(const QHash<Key, T> &other) const { return __impl.operator ==(other.__impl); }
	T&	           operator[](const Key & key) { return __impl.operator [](key); }
	const T        operator[](const Key & key) const { return __impl.operator [](key); }

public:
	QHash<Key,T> __impl;
};

CWT_NS_END

#endif /* __CWT_HASH_HPP__ */
