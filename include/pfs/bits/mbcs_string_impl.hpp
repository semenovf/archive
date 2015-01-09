/*
 * mbcs_string_impl.hpp
 *
 *  Created on: 04 Nov 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_MBCS_STRING_IMPL_HPP__
#define __PFS_BITS_MBCS_STRING_IMPL_HPP__

#include <string>
#include <pfs/ucchar.hpp>
#include <pfs/bits/reference.hpp>

namespace pfs {

/* Multibyte charcter set */

template <typename _CodeUnitT>
class mbcs_string_impl : protected std::basic_string<_CodeUnitT>
{
public:
	typedef std::basic_string<_CodeUnitT>   base_class;
	typedef mbcs_string_impl          self_class;
	typedef size_t                    size_type;
	typedef ptrdiff_t                 difference_type;
	typedef typename base_class::pointer       pointer;
	typedef typename base_class::const_pointer const_pointer;

public:
	size_type _length; // length in Unicode chars

public:
	mbcs_string_impl () : base_class(), _length(0) {}
	virtual ~mbcs_string_impl () {}

	const_pointer constData () const { return base_class::data(); }
	size_type size ()          const { return base_class::size(); }
	size_type length ()        const { return _length; }

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

public: // static
	static DLL_API const_pointer   increment  (const_pointer start, difference_type n, size_type * invalidCodeUnits = nullptr);
	static DLL_API const_pointer   decrement  (const_pointer start, difference_type n, size_t * invalidCodeUnits = nullptr);
	static DLL_API size_type       length     (const_pointer from, const_pointer to, size_type * invalidCodeUnits = nullptr);
	static DLL_API size_type       size       (const_pointer start, difference_type nchars, size_type * invalidCodeUnits = nullptr);
	static DLL_API difference_type difference (const_pointer from, const_pointer to, size_type * invalidCodeUnits = nullptr);
};

template <typename _CodeUnitT, typename Holder>
class mbcs_string_ptr
{
	typedef typename constness<Holder>::pointer pointer;

public:
	typedef typename mbcs_string_impl<_CodeUnitT>::difference_type difference_type;
	typedef typename mbcs_string_impl<_CodeUnitT>::size_type       size_type;
	typedef ucchar    value_type;

protected:
	Holder *  _holder;
	size_type _off; // offset in code units

public:
	mbcs_string_ptr (Holder & holder, size_type off) : _holder(& holder), _off(off) {}

	size_type index () { return _off; }

	reference<Holder> ref () const;

	//reference<Holder> operator * () const;
	value_type operator * () const;

	mbcs_string_ptr & operator ++ ()
	{
    	return this->operator += (1);
	}

	mbcs_string_ptr operator ++ (int)
    {
		mbcs_string_ptr r(*this);
        this->operator ++ ();
        return r;
    }

	mbcs_string_ptr & operator -- ()
	{
    	return this->operator -= (1);
	}

	mbcs_string_ptr operator -- (int)
    {
		mbcs_string_ptr r(*this);
        this->operator -- ();
        return r;
    }

	mbcs_string_ptr operator + (difference_type n) const
    {
		mbcs_string_ptr r(*this);
    	r += n;
    	return r;
    }

	mbcs_string_ptr operator - (difference_type n) const
    {
		mbcs_string_ptr r(*this);
    	r -= n;
    	return r;
    }

	mbcs_string_ptr & operator += (difference_type n);
	mbcs_string_ptr & operator -= (difference_type n);

    // Random access iterator requirements
    //reference<Holder> operator [] (difference_type n) const;
    value_type operator [] (difference_type n) const;

//    difference_type distance (const ucchar_ptr_basic & p) const
//    {
//       	difference_type d = _ref.ptr() - p._ref.ptr();
//        return d < 0 ? d * -1 : d;
//    }

    bool operator == (const mbcs_string_ptr & o) const { return _holder->constData() + _off == o._holder->constData() + o._off; }
    bool operator != (const mbcs_string_ptr & o) const { return ! this->operator == (o); }
	bool operator  > (const mbcs_string_ptr & o) const { return _holder->constData() + _off  > o._holder->constData() + o._off; }
	bool operator >= (const mbcs_string_ptr & o) const { return _holder->constData() + _off >= o._holder->constData() + o._off; }
	bool operator  < (const mbcs_string_ptr & o) const { return _holder->constData() + _off  < o._holder->constData() + o._off; }
	bool operator <= (const mbcs_string_ptr & o) const { return _holder->constData() + _off <= o._holder->constData() + o._off; }

	const _CodeUnitT * base () const { return _holder->constData() + _off; }
};

template <typename _CodeUnitT, typename Holder>
inline typename mbcs_string_ptr<_CodeUnitT, Holder>::difference_type operator
- (const mbcs_string_ptr<_CodeUnitT, Holder> & p1, const mbcs_string_ptr<_CodeUnitT, Holder> & p2)
{
	size_t invalidBytes = 0;
	typename mbcs_string_ptr<_CodeUnitT, Holder>::difference_type r
		= mbcs_string_impl<_CodeUnitT>::difference(p2.base(), p1.base(), & invalidBytes);

	PFS_ASSERT(invalidBytes == 0);
	return r;
}

} // pfs

#endif // __PFS_BITS_MBCS_STRING_IMPL_HPP__
