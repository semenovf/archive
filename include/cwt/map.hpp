/*
 * map.hpp
 *
 *  Created on: Oct 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_MAP_HPP__
#define __CWT_MAP_HPP__

#include <map>
#include <utility> // for std::pair
#include <cwt/cwt.h>
#include <cwt/shared_ptr.hpp>


CWT_NS_BEGIN

template <typename Key, typename T>
class MapData : public std::map<Key, T>
{
public:
	typedef typename std::map<Key,T>::iterator iterator;
	typedef typename std::map<Key,T>::const_iterator const_iterator;
public:
	MapData() : std::map<Key, T>() {}
	MapData * clone () const;
};


template <typename Key, typename T>
class DLL_API Map
{
public:
	typedef typename MapData<Key,T>::iterator iterator;
	typedef typename MapData<Key,T>::const_iterator const_iterator;

public:
    Map ();
	Map (const Map<Key, T> & other);
	Map<Key, T> & operator = (const Map<Key, T> & other);

	T &	           at (const Key & key) { value(key); }
	const T        at (const Key & key) const { return value(key); }

	iterator	   find (const Key & key) { detach(); return m_d->find(key); }
	const_iterator find (const Key & key) const { return m_d->find(key); }
	void	       clear () { *this = Map<Key,T>(); }
	bool           contains (const Key & key) const { return find(key) != cend(); }
	bool	       isEmpty () const { return m_d->size() == 0; }

	iterator       begin () { detach(); return iterator(m_d->begin()); }
    const_iterator begin () const { return const_iterator(m_d->begin()); }
    const_iterator cbegin () const { return const_iterator(m_d->begin()); }
    iterator       end () { detach(); return iterator(m_d->end()); }
    const_iterator end () const { return const_iterator(m_d->end()); }
    const_iterator cend () const { return const_iterator(m_d->end()); }
	iterator       insert (const Key & key, const T & value);
	T &	           value (const Key & key) { return find(key)->second; }
	const T	       value (const Key & key) const { return find(key)->second; }
	const T	       value (const Key & key, const T & defaultValue) const
	{
		const_iterator it = find(key);
		return it == cend() ? defaultValue : it->second;
	}

	size_t 	       size() const { return m_d->size(); }
	bool           remove(const Key & key) { detach(); return m_d->erase(key) > 0; }
	bool	       operator != (const Map<Key, T> & other) const { return !(*this == other); }
	bool	       operator == (const Map<Key, T> & other) const;
	T &	           operator [] (const Key & key) { return value(key); }
	const T        operator [] (const Key & key) const { return value(key); }

	Map &          operator << (const std::pair<Key, T> & p) { this->insert(p.first, p.second); return *this; }

protected:
	void           detach();

private:
	shared_ptr<MapData<Key,T> > m_d;
};

template <typename Key, typename T>
inline Map<Key,T>::Map() : m_d(new MapData<Key,T>)
{
}

template <typename Key, typename T>
inline Map<Key,T>::Map(const Map<Key, T> &other)
	: m_d(other.m_d)
{
}

template <typename Key, typename T>
inline Map<Key, T>&  Map<Key,T>::operator = (const Map<Key, T> & other)
{
	m_d = other.m_d;
	return *this;
}

template <typename Key, typename T>
inline void Map<Key,T>::detach()
{
	if (m_d.use_count() > 1) {
		shared_ptr<MapData<Key,T> > d(m_d->clone());
		m_d.swap(d);
	}
}

template <typename Key, typename T>
inline typename Map<Key,T>::iterator Map<Key,T>::insert(const Key & key, const T & value)
{
	detach();
	std::pair<iterator,bool> r = m_d->insert(std::pair<Key,T>(key, value));
	if (!r.second) { // key already exists
		r.first->second = value;
	}
	return r.first;
}


template <class Key, class T>
bool Map<Key, T>::operator == (const Map<Key, T> & other) const
{
    if (size() != other.size())
        return false;
    if (m_d == other.m_d)
        return true;

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

template <typename Key, typename T>
MapData<Key, T> * MapData<Key, T>::clone () const
{
	MapData * peer = new MapData<Key,T>;
	const_iterator it = std::map<Key,T>::begin();
	const_iterator itEnd = std::map<Key,T>::end();
	peer->insert(it, itEnd);
	return peer;
}
CWT_NS_END

#endif /* __CWT_MAP_HPP__ */
