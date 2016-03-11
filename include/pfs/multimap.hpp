/*
 * multimap.hpp
 *
 *  Created on: Mar 11, 2016
 *      Author: wladt
 */

#ifndef __PFS_MULTIMAP_HPP__
#define __PFS_MULTIMAP_HPP__

#include <map>

namespace pfs {

template <typename Key, typename T,
	    typename Compare = std::less<Key>,
	    typename Alloc = std::allocator<std::pair<const Key, T> > >
class multimap
{
	typedef std::multimap<Key, T, Compare, Alloc> base_class;

public:
    typedef typename base_class::key_type               key_type;
    typedef typename base_class::mapped_type            mapped_type;
    typedef typename base_class::value_type             value_type;
    typedef typename base_class::key_compare            key_compare;
    typedef typename base_class::allocator_type         allocator_type;
    typedef typename base_class::pointer                pointer;
    typedef typename base_class::const_pointer          const_pointer;
    typedef typename base_class::reference              reference;
    typedef typename base_class::const_reference        const_reference;
    typedef typename base_class::iterator               iterator;
    typedef typename base_class::const_iterator         const_iterator;
    typedef typename base_class::size_type              size_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::reverse_iterator       reverse_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;

public:
    multimap ()
    	: base_class()
	{ }

    explicit multimap (const Compare & comp, const allocator_type & a = allocator_type())
    	: base_class(comp, a)
    { }

    multimap (const multimap & other)
    	: base_class(other)
    { }

#if __cplusplus >= 201103L
    multimap (multimap && other)
    	: base_class(other)
    { }

    multimap (initializer_list<value_type> l
    		, const Compare & comp = Compare()
    		, const allocator_type & a = allocator_type())
    	: base_class(l, comp, a)
    { }

    explicit multimap (const allocator_type & a)
    	: base_class(a)
    { }

    multimap (const multimap & m, const allocator_type & a)
    	: base_class(m, a)
    { }

    multimap (multimap && m, const allocator_type & a)
    	: base_class(m, a) { }

    multimap (initializer_list<value_type> l, const allocator_type & a)
    	: base_class(l, a)
    { }

    template <typename InputIterator>
    multimap (InputIterator first, InputIterator last, const allocator_type & a)
		: base_class(first, last, a)
	{ }
#endif

    template <typename InputIterator>
    multimap (InputIterator first, InputIterator last)
		: base_class(first, last)
	{ }

    template <typename InputIterator>
    multimap (InputIterator first, InputIterator last
    		, const Compare & comp
    		, const allocator_type & a = allocator_type())
    	: base_class(first, last, comp, a)
    { }

    multimap & operator = (const multimap & other)
    {
    	base_class::operator = (other);
    	return *this;
    }

#if __cplusplus >= 201103L
    multimap & operator = (multimap && other)
    {
    	base_class::operator = (other);
    	return *this;
    }

    multimap & operator = (initializer_list<value_type> l)
    {
    	base_class::operator = (l);
    	return *this;
    }
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

};

}

#endif /* __PFS_MULTIMAP_HPP__ */
