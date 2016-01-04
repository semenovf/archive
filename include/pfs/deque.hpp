/*
 * deque.hpp
 *
 *  Created on: Jan 4, 2016
 *      Author: wladt
 */

#ifndef __PFS_DEQUE_HPP__
#define __PFS_DEQUE_HPP__

#include <deque>

namespace pfs {

template <typename T, typename Alloc = std::allocator<T> >
class deque : public std::deque<T, Alloc>
{
	typedef std::deque<T, Alloc> base_class;

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
     *  @brief Default constructor creates no elements.
     */
    deque()
    	: base_class()
	{}

    explicit deque (const allocator_type & alloc)
    	: base_class(alloc)
    {}

    deque (size_type n
    		, const value_type & value
    		, const allocator_type & alloc = allocator_type())
    	: base_class(n, value, alloc)
    {}

    explicit deque (size_type n)
    	: base_class(n, value_type(), allocator_type())
    {}

    deque (const deque & other)
    	: base_class(other)
    {}

    template <typename InputIterator>
    deque(InputIterator first
    		, InputIterator last
    		, const allocator_type & alloc = allocator_type())
    	: base_class(first, last, alloc)
    {}

#if __cplusplus >= 201103L
    deque (deque && other)
    	: base_class(other)
    {}

    deque (initializer_list<value_type> init
    		, const allocator_type & alloc = allocator_type())
    	: base_class(init, alloc)
    {}
#endif
};

} // pfs

#endif /* __PFS_DEQUE_HPP__ */
