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
#include <pfs/vector.hpp>
#include <pfs/type_traits.hpp>

namespace pfs {

template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<std::pair<const Key, T> > >
class map : public nullable<map_impl<Key, T, Compare, Alloc> >
{
protected:
	typedef map<Key, T, Compare, Alloc>       self_class;
	typedef map_impl<Key, T, Compare, Alloc>  impl_class;
	typedef nullable<impl_class>              base_class;

public:
	typedef T                                    value_type;
	typedef Key                                  key_type;
	typedef typename impl_class::size_type       size_type;
	typedef typename impl_class::difference_type difference_type;
	typedef map_pointer<Key, T, Compare, Alloc, self_class>       pointer;
	typedef map_pointer<Key, T, Compare, Alloc, const self_class> const_pointer;
	typedef pfs::reference<self_class>             reference;
	typedef pfs::reference<const self_class>       const_reference;

	template <typename Holder>
	class base_iterator : public bidirectional_iterator<Holder>
	{
	public:
		typedef typename bidirectional_iterator<Holder>::pointer pointer;

		base_iterator ()
			: bidirectional_iterator<Holder>() {}

		base_iterator (Holder * holder, pointer ptr)
			: bidirectional_iterator<Holder>(holder, ptr) {}

		base_iterator (const base_iterator<Holder> & it)
			: bidirectional_iterator<Holder>(it.holder(), it.base()) {}

		key_type key () const { return this->base().key(); }
		value_type value () const { return this->base().value(); }
	};

	class iterator : public base_iterator<self_class>
	{
	public:
		typedef typename base_iterator<self_class>::pointer pointer;

		iterator () : base_iterator<self_class>() {}
		iterator (self_class * holder, pointer ptr) : base_iterator<self_class>(holder, ptr) {}
		iterator (const iterator & it) : base_iterator<self_class>(it.holder(), it.base())	{}
	};

	class const_iterator : public base_iterator<const self_class>
	{
	public:
		typedef typename base_iterator<const self_class>::pointer pointer;

		const_iterator () : base_iterator<const self_class>() {}
		const_iterator (const self_class * holder, pointer ptr) : base_iterator<const self_class>(holder, ptr) {}
		const_iterator (const const_iterator & it) : base_iterator<const self_class>(it.holder(), it.base()) {}
		const_iterator (const iterator & it) : base_iterator<const self_class>(it.holder(), pointer(it.holder(), it.base().base())) {}
	};

    typedef std::reverse_iterator<iterator>		   reverse_iterator;
    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;

public:
    map () : base_class() {}
    virtual ~map () {}

public:
	bool isEmpty () const { return base_class::isNull() || base_class::cast()->isEmpty(); }
	bool empty () const { return isEmpty(); }
	void clear ()
	{
		if (!isEmpty())
			base_class::cast()->clear();
	}

	size_type size () const { return base_class::isNull() ? 0 : base_class::cast()->size(); }

    iterator begin () { return iterator(this, base_class::cast()->beginPtr(*this)); }
    iterator end   () { return iterator(this, base_class::cast()->endPtr(*this)); }
    const_iterator begin () const { return cbegin(); }
    const_iterator end   () const { return cend(); }
    const_iterator cbegin() const { return const_iterator(this, base_class::cast()->cbeginPtr(*this)); }
    const_iterator cend  () const { return const_iterator(this, base_class::cast()->cendPtr(*this)); }
    reverse_iterator rbegin  ()   { return reverse_iterator(end()); }
    reverse_iterator rend    ()   { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin  () const { return crbegin(); }
    const_reverse_iterator rend    () const { return crend(); }
    const_reverse_iterator crbegin () const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend   () const { return const_reverse_iterator(cbegin()); }

	iterator find (const key_type & key) const
	{
		map * self = const_cast<map *>(this);
		pointer p = self->base_class::cast()->find(*self, key);
		return iterator(self, p);
	}

	bool contains (const key_type & key) const { return find(key) != cend(); }

	/**
	 * @brief Inserts a new item with the key @c key and a value of @c value.
	 *
	 * @details If there is already an item with the key @c key, that item's value is replaced with @c value.
	 *          If there are multiple items with the key @c key, the most recently inserted item's
	 *          value is replaced with @c value.
	 *
	 * @param key Key for insertion value.
	 * @param value Value to insert.
	 * @return iterator Position pointed to insertion value.
	 */
	iterator insert (const key_type & key, const T & value);

	size_type remove (const key_type & key)
	{
		base_class::detach();
		return base_class::cast()->remove(key);
	}

	iterator remove (const_iterator pos)
	{
		base_class::detach();
		pointer p = base_class::cast()->remove(pos);
		return iterator(this, p);
	}


//	bool           remove (const Key & key) { _d.detach(); return _d.cast<impl>()->erase(key) > 0; }
//	void           remove (iterator pos) { _d.detach(); _d.cast<impl>()->erase(pos); }
//	bool	       operator != (const map<Key, T> & other) const { return !(*this == other); }
//	bool	       operator == (const map<Key, T> & other) const;

	value_type valueAt (const key_type & key) const
	{
		reference r = at(key);
		return *(& r);
	}

	value_type valueAt (const key_type & key, const T & defaultValue) const
	{
		iterator it = find(key);
		return it != cend() ? it.value() : defaultValue;
	}

	const T & refAt (const key_type & key) const
	{
		reference r = at(key);
		return *(& r);
	}

	T & refAt (const key_type & key);

	reference at (const key_type & key) const;

	reference operator [] (const key_type & key) const
    {
    	return at(key);
    }

	vector<key_type>   keys() const;

	map & operator << (const std::pair<Key, T> & p) { this->insert(p.first, p.second); return *this; }

	void detach_and_assign (pointer & p, const T & value); // pfs::reference class requirement
};


template <typename Key, typename T, typename Compare, typename Alloc>
T & map<Key, T, Compare, Alloc>::refAt (const key_type & key)
{
	iterator it = find(key);
	if (it == end())
		it = insert(key, T());

	return *it.base();
}

template <typename Key, typename T, typename Compare, typename Alloc>
typename map<Key, T, Compare, Alloc>::reference
map<Key, T, Compare, Alloc>::at (const key_type & key) const
{
	map * self = const_cast<map *>(this);
	iterator it = find(key);
	PFS_ASSERT_RANGE(it != cend());
	return reference(*self, it.base());
}


template <typename Key, typename T, typename Compare, typename Alloc>
void map<Key, T, Compare, Alloc>::detach_and_assign (
		typename map<Key, T, Compare, Alloc>::pointer & p
      , const typename map<Key, T, Compare, Alloc>::value_type & value)
{
	if (!base_class::isUnique()) {
		base_class::detach();
		pointer pp = this->base_class::cast()->find(*this, p.base()->first);
		pp.base()->second = value;
	} else {
		p.base()->second = value;
	}
}


template <typename Key, typename T, typename Compare, typename Alloc>
typename map<Key, T, Compare, Alloc>::iterator map<Key, T, Compare, Alloc>::insert (const Key & key, const T & value)
{
	base_class::detach();
	std::pair<pointer, bool> r = base_class::cast()->insert(*this, std::pair<Key, T>(key, value));
	if (!r.second) { // not inserted
		reference ref = r.first.ref();
		ref = value;
	}
	return iterator(this, r.first);
}

template <typename Key, typename T, typename Compare, typename Alloc>
vector<Key> map<Key, T, Compare, Alloc>::keys () const
{
	vector<Key> r;
	const_iterator it = this->cbegin();
	const_iterator itEnd = cend();
	for (; it != itEnd; ++it) {
		r.append(it.key());
	}
	return r;
}


#ifdef __COMMENT__
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


#endif

//#include <pfs/bits/map_impl_inc.hpp>

} // pfs

#endif /* __PFS_MAP_HPP__ */
