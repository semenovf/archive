/*
 * byte_string_impl.hpp
 *
 *  Created on: 23 Dec 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_BYTE_STRING_IMPL_HPP__
#define __PFS_BITS_BYTE_STRING_IMPL_HPP__

#include <string>
#include <pfs/bits/reference.hpp>

namespace pfs {

class byte_string_impl : protected std::basic_string<byte_t>
{
public:
	typedef std::basic_string<byte_t> base_class;
	typedef byte_string_impl          self_class;
	typedef byte_t					  value_type;
	typedef size_t                    size_type;
	typedef ptrdiff_t                 difference_type;
	typedef base_class::pointer       pointer;
	typedef base_class::const_pointer const_pointer;

public:
	byte_string_impl () : base_class() {}
	virtual ~byte_string_impl () {}

	const_pointer constData () const { return base_class::data(); }
	size_type size ()          const { return base_class::size(); }
	size_type length ()        const { return base_class::size(); }

	void erase (size_type index, size_type count)
	{
		base_class::erase(index, count);
	}

	void resize (size_type count, const value_type & v)
	{
		base_class::resize(count, v);
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

	void replace (size_type index, const value_type & val)
	{
		operator [] (index) = val;
	}

	int compare (const_pointer p1, size_type count1, const_pointer p2, size_type count2) const
	{
		return base_class::compare(p1 - constData(), count1, p2, count2);
	}

	/**
	 * @brief Finds the first substring equal to the first @c count characters
	 * of the character string pointed to by @c s. @c s can include null characters.
	 *
	 * @param index  position at which to start the search
	 * @param s      string to search for
	 * @param count  length of substring to search for
	 * @param result position of the first character of the found substring or npos if no such substring is found
	 * @return @c true if substring found, false if otherwise
	 */
	bool find (size_type index, const_pointer s, size_type count, size_type & result) const
	{
		result = base_class::find(s, index, count);
		return result == base_class::npos ? false : true;
	}
};


template <typename Holder>
class byte_string_ptr
{
	typedef typename constness<Holder>::pointer pointer;

public:
	typedef typename byte_string_impl::difference_type difference_type;
	typedef typename byte_string_impl::size_type       size_type;
	typedef byte_t   value_type;

protected:
	Holder *  _holder;
	size_type _off; // offset in code units

public:
	byte_string_ptr (Holder * holder, size_type off) : _holder(holder), _off(off) {}

	size_type index () { return _off; }

	reference<Holder> ref () const;

	value_type operator * () const;

	byte_string_ptr & operator ++ ()
	{
    	return this->operator += (1);
	}

	byte_string_ptr operator ++ (int)
    {
		byte_string_ptr r(*this);
        this->operator ++ ();
        return r;
    }

	byte_string_ptr & operator -- ()
	{
    	return this->operator -= (1);
	}

	byte_string_ptr operator -- (int)
    {
		byte_string_ptr r(*this);
        this->operator -- ();
        return r;
    }

	byte_string_ptr operator + (difference_type n) const
    {
		byte_string_ptr r(*this);
    	r += n;
    	return r;
    }

	byte_string_ptr operator - (difference_type n) const
    {
		byte_string_ptr r(*this);
    	r -= n;
    	return r;
    }

	byte_string_ptr & operator += (difference_type n)
	{
		_off += n;
		PFS_ASSERT(_off <= _holder->size());
		return *this;
	}

	byte_string_ptr & operator -= (difference_type n)
	{
		PFS_ASSERT(_off >= n);
		_off -= n;
		return *this;
	}

    // Random access iterator requirements
    value_type operator [] (difference_type n) const;

    bool operator == (const byte_string_ptr & o) const { return _holder->constData() + _off == o._holder->constData() + o._off; }
    bool operator != (const byte_string_ptr & o) const { return ! this->operator == (o); }
	bool operator  > (const byte_string_ptr & o) const { return _holder->constData() + _off  > o._holder->constData() + o._off; }
	bool operator >= (const byte_string_ptr & o) const { return _holder->constData() + _off >= o._holder->constData() + o._off; }
	bool operator  < (const byte_string_ptr & o) const { return _holder->constData() + _off  < o._holder->constData() + o._off; }
	bool operator <= (const byte_string_ptr & o) const { return _holder->constData() + _off <= o._holder->constData() + o._off; }

	const byte_t * base () const { return _holder->constData() + _off; }
};

template <typename Holder>
inline typename byte_string_ptr<Holder>::difference_type operator
- (const byte_string_ptr<Holder> & p1, const byte_string_ptr<Holder> & p2)
{
	typename byte_string_ptr<Holder>::difference_type r = p1.base() - p2.base();
	return r;
}

} // pfs

#endif // __PFS_BITS_BYTE_STRING_IMPL_HPP__
