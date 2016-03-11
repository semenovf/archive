/*
 * map.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_MAP_HPP__
#define __PFS_MAP_HPP__

#include <map>

namespace pfs {

template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, T> > >
class map : public std::map<Key, T, Compare, Alloc>
{
	typedef std::map<Key, T, Compare, Alloc> base_class;

public:
    typedef typename base_class::key_type       key_type;
    typedef typename base_class::mapped_type    mapped_type;
    typedef typename base_class::value_type     value_type;
    typedef typename base_class::key_compare    key_compare;
    typedef typename base_class::allocator_type allocator_type;

    typedef typename base_class::pointer            pointer;
    typedef typename base_class::const_pointer      const_pointer;
    typedef typename base_class::reference          reference;
    typedef typename base_class::const_reference    const_reference;
    typedef typename base_class::iterator           iterator;
    typedef typename base_class::const_iterator     const_iterator;
    typedef typename base_class::size_type          size_type;
    typedef typename base_class::difference_type    difference_type;
    typedef typename base_class::reverse_iterator   reverse_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;

public:
    explicit map (const key_compare & comp = key_compare(),
                  const allocator_type & alloc = allocator_type())
    	: base_class(comp, alloc)
    {}

    template <typename InputIt>
    map (InputIt first, InputIt last
    		, const key_compare & comp = key_compare()
			, const allocator_type & alloc = allocator_type())
		 : base_class(first, last, comp, alloc)
	{}

    map (const map & other)
    	: base_class(other)
    {}


#if __cplusplus >= 201103L
    explicit map (const allocator_type & alloc)
    	: base_class(alloc)
    {}

    map (const map & other, const allocator_type & alloc)
    	: base_class(other, alloc)
    {}

    map (map && other)
    	: base_class(other)
    {}

    map (map && other, const allocator_type & alloc)
    	: base_class(other, alloc)
    {}

    map (std::initializer_list<value_type> init
    		, const key_compare & comp = key_compare()
			, const allocator_type & alloc = allocator_type())
    	: base_class(init, comp, alloc)
    {}

#endif

#if __cplusplus < 201103L
    const_iterator cbegin () const
    {
    	return base_class::begin();
    }

    const_iterator cend () const
    {
    	return base_class::end();
    }
#endif

    std::pair<iterator, bool> insert2 (const key_type & key, const mapped_type & value)
    {
    	return this->insert(value_type(key, value));
    }
};

} // pfs

#endif /* __PFS_MAP_HPP__ */
