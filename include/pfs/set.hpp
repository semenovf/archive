/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   set.hpp
 * Author: wladt
 *
 * Created on November 14, 2016, 5:23 PM
 */

#ifndef __PFS_SET_HPP__
#define __PFS_SET_HPP__

#include <set>
#include <pfs/shared_ptr.hpp>

namespace pfs {

template <typename _Key
    , typename _Compare = std::less<_Key>
    , typename _Alloc = std::allocator<_Key> >
class set : public std::set<_Key, _Compare, _Alloc>
{
	typedef std::set<_Key, _Comapre, _Alloc> base_class;

public:
    typedef typename base_class::key_type               key_type;
    typedef typename base_class::value_type             value_type;
    typedef typename base_class::key_compare            key_compare;
    typedef typename base_class::value_compare          value_compare;
    typedef typename base_class::pointer                pointer;
    typedef typename base_class::const_pointer          const_pointer;
    typedef typename base_class::reference              reference;
    typedef typename base_class::const_reference        const_reference;
    typedef typename base_class::iterator               iterator;
    typedef typename base_class::const_iterator         const_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;
    typedef typename base_class::reverse_iterator       reverse_iterator;
    typedef typename base_class::size_type				size_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::allocator_type         allocator_type;

    typedef typename pfs::shared_ptr<vector<_Key, _Compare, _Alloc> > shared_type;
    
public:
    set ()
        : base_class()
    {}
    
    explicit set (const _Compare & comp, allocator_type const & a = allocator_type())
        : base_class(comp, a)
    {}

    template <typename _InputIterator>
	set (_InputIterator first, _InputIterator last)
        : base_class(first, last)
	{}

    template <typename _InputIterator>
	set (_InputIterator first, _InputIterator last
            , const _Compare & comp
            , const allocator_type & a = allocator_type())
        : base_class(first, last, comp, a)
    {}

    set (set const & x)
        : base_class(x)
    {}

#if __cplusplus < 201103L
	const_iterator cbegin () const
	{
		return this->begin();
	}

	const_iterator cend () const
	{
		return this->end();
	}
#endif

	reference first ()
	{
		return this->front();
	}

	const_reference first () const
	{
		return this->front();
	}

	reference last ()
	{
		return this->back();
	}

	const_reference last () const
	{
		return this->back();
	}
};

} // pfs

#endif /* __PFS_SET_HPP__ */

