/*
 * map.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#include <pfs.hpp>
#include <pfs/vector.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/pimpl.hpp>
#include <map>

namespace pfs {

template <typename Key, typename T>
class map
{
	typedef std::map<Key,T> impl;
	pimpl _d;

public:
	typedef typename std::map<Key, T>::iterator iterator;
	typedef typename std::map<Key, T>::const_iterator const_iterator;

public:
    map () : _d(new impl) {}

    // FIXME no need detach(), iterator has to support this feature
	iterator	   find (const Key & key) { _d.detach(); return _d.cast<impl>()->find(key); }
	const_iterator find (const Key & key) const { return _d.cast<impl>()->find(key); }
	void	       clear () { _d.detach(); _d.cast<impl>()->clear(); }
	bool           contains (const Key & key) const { return find(key) != cend(); }
	bool	       isEmpty () const { return _d.cast<impl>()->empty(); }

	iterator       begin () { _d.detach(); return _d.cast<impl>()->begin(); }
    const_iterator begin () const { return _d.cast<impl>()->begin(); }
    const_iterator cbegin () const { return _d.cast<impl>()->begin(); }
    iterator       end () { _d.detach(); return _d.cast<impl>()->end(); }
    const_iterator end () const { return _d.cast<impl>()->end(); }
    const_iterator cend () const { return _d.cast<impl>()->end(); }

	iterator       insert (const Key & key, const T & value);

	const T	       value (const Key & key) const;
	const T	       value (const Key & key, const T & defaultValue) const;

	size_t 	       size () const { return _d.cast<impl>()->size(); }
	bool           remove (const Key & key) { _d.detach(); return _d.cast<impl>()->erase(key) > 0; }
	void           remove (iterator pos) { _d.detach(); _d.cast<impl>()->erase(pos); }
	bool	       operator != (const map<Key, T> & other) const { return !(*this == other); }
	bool	       operator == (const map<Key, T> & other) const;

	// FIXME return reference (or iterator)
	T &	           operator [] (const Key & key) { _d.detach(); return _d.cast<impl>()->operator [] (key); }
	const T        operator [] (const Key & key) const { return _d.cast<impl>()->at(key); }

	map &          operator << (const std::pair<Key, T> & p) { this->insert(p.first, p.second); return *this; }
	vector<Key>    keys() const;
};


template <typename Key, typename T>
inline typename map<Key,T>::iterator map<Key,T>::insert(const Key & key, const T & value)
{
	_d.detach();
	std::pair<iterator,bool> r = _d.cast<impl>()->insert(std::pair<Key,T>(key, value));
	if (!r.second) { // key already exists
		r.first->second = value;
	}
	return r.first;
}

template <class Key, class T>
inline const T map<Key, T>::value (const Key & key) const
{
	return find(key)->second;
}

template <class Key, class T>
const T	map<Key, T>::value (const Key & key, const T & defaultValue) const
{
	const_iterator it = find(key);
	return it == cend() ? defaultValue : it->second;
}


template <class Key, class T>
bool map<Key, T>::operator == (const map<Key, T> & other) const
{
	if (this == & other)
		return true;

	pimpl & p = other._d;
    if (*_d.cast<impl>() == *p.cast<impl>())
        return true;

    if (size() != other.size())
        return false;

    const_iterator it = begin();

    while (it != end()) {
        const Key & key = it->first();
        const_iterator it2 = other.find(key);

        if (it2 == other.end() || !(it2->first() == key))
        	return false;
        ++it;
    }
    return true;
}



/*
template <typename Key, typename T>
MapData<Key, T> * MapData<Key, T>::clone () const
{
	MapData * peer = new MapData<Key,T>;
	const_iterator it = std::map<Key,T>::begin();
	const_iterator itEnd = std::map<Key,T>::end();
	peer->insert(it, itEnd);
	return peer;
}
*/

template <typename Key, typename T>
vector<Key> map<Key, T>::keys () const
{
	vector<Key> r;
	const_iterator it = cbegin();
	const_iterator itEnd = cend();
	for (; it != itEnd; ++it) {
		r.append(it->first);
	}
	return r;
}

} // pfs

#endif /* __PFS_MAP_HPP__ */
