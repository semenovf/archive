/*
 * byte_string_impl.hpp
 *
 *  Created on: 23 Dec 2014
 *      Author: wladt
 */

#ifndef __PFS_BYTE_STRING_IMPL_HPP__
#define __PFS_BYTE_STRING_IMPL_HPP__

#include <string>
#include <pfs/bits/reference.hpp>

namespace pfs {

class byte_string_impl : protected std::basic_string<byte_t>
{
public:
	typedef std::basic_string<byte_t> base_class;
	typedef byte_string_impl          self_class;
	typedef size_t                    size_type;
	typedef ptrdiff_t                 difference_type;
	typedef base_class::pointer       pointer;
	typedef base_class::const_pointer const_pointer;

public:
	byte_string_impl () : base_class() {}
	virtual ~byte_string_impl () {}

	bool isEmpty ()            const { return base_class::empty(); }
	const_pointer constData () const { return base_class::data(); }
	size_type size ()          const { return base_class::size(); }
	size_type length ()        const { return base_class::size(); }

	void erase (size_type index, size_type count)
	{
		base_class::erase(index, count);
	}

	void resize (size_type count)
	{
		base_class::resize(count);
	}

	void reserve (size_type count)
	{
		base_class::reserve(count);
	}

	size_type capacity() const
	{
		return base_class::capacity();
	}

    size_type max_size() const
    {
    	return base_class::max_size();
    }

	void append (const_pointer s , size_type n)
	{
		base_class::append(s, n);
	}

	void insert (size_type index, const_pointer s, size_type n)
	{
		base_class::insert(index, s, n);
	}

	void replace (const_pointer ptr1, size_type count, const_pointer ptr2, size_type count2)
	{
		base_class::replace(ptr1 - constData(), count, ptr2, count2);
	}

	int compare (const_pointer p1, size_type count1, const_pointer p2, size_type count2) const
	{
		return base_class::compare(p1 - constData(), count1, p2, count2);
	}

	bool find (const_pointer s, size_type pos, size_type count, size_type & result) const
	{
		result = base_class::find(s, pos, count);
		return result == base_class::npos ? false : true;
	}

	void clear ()
	{
		base_class::clear();
	}
};

} // pfs

#endif // __PFS_BYTE_STRING_IMPL_HPP__
