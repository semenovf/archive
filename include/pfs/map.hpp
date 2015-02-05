/*
 * map.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/bits/map_impl.hpp>
#include <pfs/bits/algorithm.hpp>

//#include <pfs.hpp>
//#include <pfs/vector.hpp>
//#include <pfs/shared_ptr.hpp>
//#include <pfs/pimpl.hpp>
//#include <map>

namespace pfs {

template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T> > >
class map : public nullable<map_impl<Key, T, Compare, Alloc> >
{
protected:
	typedef map<Key, T, Compare, Alloc>       self_class;
	typedef map_impl<Key, T, Compare, Alloc>  impl_class;
	typedef nullable<impl_class>              base_class;

public:
	typedef typename impl_class::value_type             value_type;
	typedef typename impl_class::size_type              size_type;
	typedef typename impl_class::difference_type        difference_type;

	typedef map_pointer<Key, T, Compare, Alloc, self_class>       pointer;
	typedef map_pointer<Key, T, Compare, Alloc, const self_class> const_pointer;
	typedef pfs::reference<self_class>                  reference;
	typedef pfs::reference<const self_class>            const_reference;
	typedef pfs::bidirectional_iterator<self_class>     iterator;
	typedef pfs::bidirectional_iterator<const self_class> const_iterator;
    typedef std::reverse_iterator<iterator>		        reverse_iterator;
    typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;

public:
    map () : base_class() {}
    virtual ~map () {}

public:
	bool isEmpty () const { return base_class::isNull() || base_class::cast()->empty(); }
	bool empty () const { return isEmpty(); }

	size_type size () const { return base_class::isNull() ? 0 : base_class::cast()->size(); }

//    iterator begin () { return iterator(this, impl_class::begin()); }
//    iterator end   () { return iterator(this, impl_class::end()); }
//    const_iterator begin () const { return cbegin(); }
//    const_iterator end   () const { return cend(); }
//    const_iterator cbegin() const { return const_iterator(this, impl_class::cbegin()); }
//    const_iterator cend  () const { return const_iterator(this, impl_class::cend()); }
//    reverse_iterator rbegin  ()   { return reverse_iterator(end()); }
//    reverse_iterator rend    ()   { return reverse_iterator(begin()); }
//    const_reverse_iterator rbegin  () const { return crbegin(); }
//    const_reverse_iterator rend    () const { return crend(); }
//    const_reverse_iterator crbegin () const { return const_reverse_iterator(cend()); }
//    const_reverse_iterator crend   () const { return const_reverse_iterator(cbegin()); }


//    // FIXME no need detach(), iterator has to support this feature
//	iterator	   find (const Key & key) { _d.detach(); return _d.cast<impl>()->find(key); }
//	const_iterator find (const Key & key) const { return _d.cast<impl>()->find(key); }
//	bool           contains (const Key & key) const { return find(key) != cend(); }
//
//	iterator       begin () { _d.detach(); return _d.cast<impl>()->begin(); }
//    const_iterator begin () const { return _d.cast<impl>()->begin(); }
//    const_iterator cbegin () const { return _d.cast<impl>()->begin(); }
//    iterator       end () { _d.detach(); return _d.cast<impl>()->end(); }
//    const_iterator end () const { return _d.cast<impl>()->end(); }
//    const_iterator cend () const { return _d.cast<impl>()->end(); }

	std::pair<iterator, bool> insert (const value_type & value)
	{
		base_class::detach();
		std::pair<pointer, bool> r = base_class::cast()->insert(*this, value);
		return std::pair<iterator, bool>(iterator(this, r.first), r.second);
	}

	std::pair<iterator, bool> insert (const Key & key, const T & value)
	{
		return insert(std::pair<Key, T>(key, value));
	}

//	const T	       value (const Key & key) const;
//	const T	       value (const Key & key, const T & defaultValue) const;
//
//	bool           remove (const Key & key) { _d.detach(); return _d.cast<impl>()->erase(key) > 0; }
//	void           remove (iterator pos) { _d.detach(); _d.cast<impl>()->erase(pos); }
//	bool	       operator != (const map<Key, T> & other) const { return !(*this == other); }
//	bool	       operator == (const map<Key, T> & other) const;
//

	value_type valueAt (const Key & key) const
	{
		return value_type(at(key));
	}

	reference at (const Key & key) const
	{
		map * self = const_cast<map *>(this);
		pointer p = self->base_class::cast()->find(*self, key);
		return reference(*self, p);
	}

	reference operator [] (const Key & key) const
    {
    	return at(key);
    }

//	map &          operator << (const std::pair<Key, T> & p) { this->insert(p.first, p.second); return *this; }
//	vector<Key>    keys() const;
};

#ifdef __COMMENT__
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

#endif

//#include <pfs/bits/map_impl_inc.hpp>

} // pfs

#endif /* __PFS_MAP_HPP__ */
