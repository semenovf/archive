/*
 * vector.hpp
 *
 *  Created on: Dec 3, 2015
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <vector>
#include <pfs/shared_ptr.hpp>

namespace pfs {

template <typename T, typename Alloc = std::allocator<T> >
class vector : public std::vector<T, Alloc>
{
	typedef std::vector<T, Alloc> base_class;

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
    typedef typename base_class::size_type				size_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::allocator_type         allocator_type;

    typedef typename pfs::shared_ptr<vector<T, Alloc> > shared_type;
    
public:
	explicit vector (const allocator_type & alloc = allocator_type())
		: base_class(alloc)
	{}

	vector (size_type count, const T & value, const allocator_type & alloc = allocator_type())
		: base_class(count, value, alloc)
	{}

	explicit vector (size_type count)
		: base_class(count, T())
	{}

	template <class InputIt>
	vector (InputIt first, InputIt last, const allocator_type & alloc = allocator_type())
		: base_class(first, last, alloc)
	{}

	vector (const vector & other)
	{}

#if __cplusplus >= 201103L
	vector (const vector & other, const allocator_type & alloc)
		: base_class(other, alloc)
	{}

	vector (vector && other)
		: base_class(other)
	{}

	vector (vector && other, const allocator_type & alloc)
		: base_class(other, alloc)
	{}

	vector (std::initializer_list<T> init,
	        const allocator_type & alloc = allocator_type())
		: base_class(init, alloc)
	{}
#endif

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

#endif /* __PFS_VECTOR_HPP__ */
