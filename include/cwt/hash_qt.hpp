/**
 * @file   hash_qt.hpp
 * @author wladt
 * @date   Feb 6, 2013 10:20:03 AM
 *
 * @brief Qt implementation of Hash class.
 */

#ifndef __CWT_HASH_QT_HPP__
#define __CWT_HASH_QT_HPP__

#include <cwt/cwt.h>
#include <QtCore/QHash>

#if QT_VERSION < 0x050000
DLL_API uint_t qHash(const cwt::String &key);
DLL_API uint_t qHash(const cwt::ByteArray &key);
DLL_API uint_t qHash(const uuid_t &key);
#else
DLL_API uint_t qHash(const cwt::String &key, uint_t seed);
DLL_API uint_t qHash(const cwt::ByteArray &key, uint_t seed);
DLL_API uint_t qHash(const uuid_t &key, uint_t seed);
#endif

CWT_NS_BEGIN

template <typename Key, typename T>
class Hash {
public:
	typedef typename QHash<Key,T>::iterator iterator;
	typedef typename QHash<Key,T>::const_iterator const_iterator;

public:
	Hash() : __impl() {}
	Hash(const Hash<Key, T> &other) : __impl(other.__impl) {}
	Hash<Key, T>&  operator=(const QHash<Key, T> & other) { __impl.operator =(other.__impl); return *this; }
	~Hash() {}

	T&	           at(const Key & key) { return __impl.operator [](key); }
	const T        at(const Key & key) const { return __impl.operator [](key); }
	iterator       begin() { return __impl.begin(); }
	const_iterator begin() const { return __impl.begin(); }
	const_iterator cbegin() const { return __impl.cbegin(); }
	iterator       end() { return __impl.end(); }
	const_iterator end() const { return __impl.end(); }
	const_iterator cend() const { return __impl.cend(); }
	void	       clear() { __impl.clear(); }
	iterator	   find(const Key & key) { return __impl.find(key); }
	const_iterator find(const Key & key) const { return __impl.find(key); }
	bool	       isEmpty() const { return __impl.isEmpty(); }
	iterator       insert(const Key & key, const T & value) { return __impl.insert(key, value); }
	const Key	   key ( const T & value ) const { return __impl->key(value); }
	const Key	   key ( const T & value, const Key & defaultKey ) const { return __impl.key(value, defaultKey); }
	int	           remove(const Key & key) { return __impl.remove(key); }
	int  	       size() const { return __impl.size(); }
	const T	       value(const Key & key) const { return __impl.value(key); }
	const T	       value(const Key & key, const T &defaultValue) const { return __impl.value(key); }

	bool	       operator!=(const QHash<Key, T> & other) const { return __impl.operator !=(other.__impl); }

	bool	       operator==(const QHash<Key, T> &other) const { return __impl.operator ==(other.__impl); }
	T&	           operator[](const Key & key) { return __impl.operator [](key); }
	const T        operator[](const Key & key) const { return __impl.operator [](key); }

public:
	QHash<Key,T> __impl;
};

CWT_NS_END

#endif /* __CWT_HASH_HPP__ */
