/**
 * @file   hash.hpp
 * @author wladt
 * @date   Feb 6, 2013 10:20:03 AM
 *
 * @brief
 */

#ifndef __CWT_HASH_HPP__
#define __CWT_HASH_HPP__

#include <cwt/hash_p.hpp>

CWT_NS_BEGIN

template <typename Key, typename T>
class DLL_API Hash {
public:
	typedef typename HashImpl<Key,T>::iterator iterator;
	typedef typename HashImpl<Key,T>::const_iterator const_iterator;

public:
	Hash() : impl() {}
	Hash(const Hash<Key, T> &other) : impl(other.impl) {}
	~Hash() {}

	T&	           at(const Key & key) { return impl.operator [](key); }
	const T        at(const Key & key) const { return impl.operator [](key); }
	iterator       begin() { return impl.begin(); }
	const_iterator begin() const { return impl.begin(); }
	iterator       end() { return impl.end(); }
	const_iterator end() const { return impl.end(); }
	void	       clear() { impl.clear(); }
	iterator	   find(const Key & key) { return impl.find(key); }
	const_iterator find(const Key & key) const { return impl.find(key); }
	bool	       isEmpty() const { return impl.isEmpty(); }
	iterator       insert(const Key & key, const T & value) { return impl.insert(key, value); }
	const Key	   key ( const T & value ) const { return impl->key(value); }
	const Key	   key ( const T & value, const Key & defaultKey ) const { return impl.key(value, defaultKey); }
	int	           remove(const Key & key) { return impl.remove(key); }
	int  	       size() const { return impl.size(); }
	const T	       value(const Key & key) const { return impl.value(key); }
	const T	       value(const Key & key, const T &defaultValue) const { return impl.value(key); }

	bool	       operator!=(const QHash<Key, T> & other) const { return impl.operator !=(other.__impl); }
	Hash<Key, T>&  operator=(const QHash<Key, T> & other) { impl.operator =(other.__impl); return *this; }
	bool	       operator==(const QHash<Key, T> &other) const { return impl.operator ==(other.__impl); }
	T&	           operator[](const Key & key) { return impl.operator [](key); }
	const T        operator[](const Key & key) const { return impl.operator [](key); }

public:
	HashImpl<Key,T> impl;
};

CWT_NS_END

#endif /* __CWT_HASH_HPP__ */
