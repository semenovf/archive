/*
 * list.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_LIST_HPP__
#define __PFS_LIST_HPP__

#include <list>

namespace pfs {

template <typename T, typename Alloc = std::allocator<T> >
class list : public std::list<T, Alloc>
{
	typedef std::list<T, Alloc> base_class;

public:
    typedef typename base_class::value_type             value_type;
    typedef typename base_class::pointer                pointer;
    typedef typename base_class::const_pointer          const_pointer;
    typedef typename base_class::reference              reference;
    typedef typename base_class::const_reference        const_reference;
    typedef typename base_class::iterator               iterator;
    typedef typename base_class::const_iterator         const_iterator;
    typedef typename base_class::const_reverse_iterator const_reverse_iterator;
    typedef typename base_class::reverse_iterator       reverse_iterator;
    typedef typename base_class::size_type              size_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::allocator_type         allocator_type;

public:
    /**
     *  @brief Creates a %list with no elements.
     */
    list ()
		: base_class()
	{}

    /**
     *  @brief  Creates a %list with no elements.
     *  @param  __a  An allocator object.
     */
    explicit list (const allocator_type & alloc)
    	: base_class(alloc)
	{}

//#if __cplusplus >= 201103L
    /**
     *  @brief  Creates a %list with default constructed elements.
     *  @param  n  The number of elements to initially create.
     *
     *  This constructor fills the %list with @a n default
     *  constructed elements.
     */
    explicit list (size_type n)
    	: base_class(n)
    {}

    /**
     *  @brief  Creates a %list with copies of an exemplar element.
     *  @param  __n  The number of elements to initially create.
     *  @param  __value  An element to copy.
     *  @param  __a  An allocator object.
     *
     *  This constructor fills the %list with @a __n copies of @a __value.
     */
    list (size_type n
    		, const value_type & value
    		, const allocator_type & alloc = allocator_type())
    	: base_class(n, value, alloc)
    {}

    /**
     *  @brief  %List copy constructor.
     *  @param  __x  A %list of identical element and allocator types.
     *
     *  The newly-created %list uses a copy of the allocation object used
     *  by @a __x.
     */
    list (const list & x)
    	: base_class(x)
    {}

#if __cplusplus >= 201103L
    /**
     *  @brief  %List move constructor.
     *  @param  x  A %list of identical element and allocator types.
     *
     *  The newly-created %list contains the exact contents of @a __x.
     *  The contents of @a __x are a valid, but unspecified %list.
     */
    list (list && x) noexcept
    	: base_class(x)
	{}

    /**
     *  @brief  Builds a %list from an initializer_list
     *  @param  l  An initializer_list of value_type.
     *  @param  a  An allocator object.
     *
     *  Create a %list consisting of copies of the elements in the
     *  initializer_list @a __l.  This is linear in __l.size().
     */
    list (initializer_list<value_type> init
    		, const allocator_type& alloc = allocator_type())
    	: base_class(init, alloc)
    {}
#endif

//    /**
//     *  @brief  Builds a %list from a range.
//     *  @param  __first  An input iterator.
//     *  @param  __last  An input iterator.
//     *  @param  __a  An allocator object.
//     *
//     *  Create a %list consisting of copies of the elements from
//     *  [@a __first,@a __last).  This is linear in N (where N is
//     *  distance(@a __first,@a __last)).
//     */
//#if __cplusplus >= 201103L
//    template <typename InputIterator,
//	       typename = std::_RequireInputIter<_InputIterator>>
//      list(_InputIterator __first, _InputIterator __last,
//	     const allocator_type& __a = allocator_type())
//	: _Base(_Node_alloc_type(__a))
//      { _M_initialize_dispatch(__first, __last, __false_type()); }
//#else
//    template<typename _InputIterator>
//      list(_InputIterator __first, _InputIterator __last,
//	     const allocator_type& __a = allocator_type())
//	: _Base(_Node_alloc_type(__a))
//      {
//	  // Check whether it's an integral type.  If so, it's not an iterator.
//	  typedef typename std::__is_integer<_InputIterator>::__type _Integral;
//	  _M_initialize_dispatch(__first, __last, _Integral());
//	}
//#endif

    /**
     *  @brief %List assignment operator.
     *  @param x  A %list of identical element and allocator types.
     *
     *  All the elements of @a __x are copied, but unlike the copy
     *  constructor, the allocator object is not copied.
     */
    list & operator = (const list x)
    {
    	base_class::operator = (x);
    	return *this;
    }

#if __cplusplus >= 201103L
    /**
     *  @brief  %List move assignment operator.
     *  @param  x  A %list of identical element and allocator types.
     *
     *  The contents of @a __x are moved into this %list (without copying).
     *  @a __x is a valid, but unspecified %list
     */
    list & operator = (list && x)
    {
    	base_class::operator = (x);
    	return *this;
    }

    /**
     *  @brief  %List initializer list assignment operator.
     *  @param  __l  An initializer_list of value_type.
     *
     *  Replace the contents of the %list with copies of the elements
     *  in the initializer_list @a __l.  This is linear in l.size().
     */
    list & operator = (initializer_list<value_type> init)
    {
    	base_class::operator = (init);
    	return *this;
    }
#endif

};

} // pfs

#endif /* __PFS_LIST_HPP__ */
