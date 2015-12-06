/*
 * iterator.hpp
 *
 *  Created on: Dec 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF_ITERATOR_HPP__
#define __PFS_UTF_ITERATOR_HPP__

#include <iterator>
#include <pfs/utf/traits.hpp>

namespace pfs { namespace utf {

template <typename CodeUnitIterator, typename UtfTag>
class iterator
{
public:
	typedef traits<CodeUnitIterator, UtfTag>          utf_traits_type;
    typedef typename utf_traits_type::value_type      value_type;
    typedef typename utf_traits_type::pointer         pointer;
    typedef typename utf_traits_type::reference       reference; // std::advance() requirement
    typedef typename utf_traits_type::difference_type difference_type;
    typedef std::bidirectional_iterator_tag           iterator_category;

protected:
	pointer  _p;
//	uint32_t _replacement_char;

public:
    explicit iterator (pointer i)
    	: _p(i)
//    	, _replacement_char('?')
    {}

//    iterator (pointer i/*, uint32_t replacement_char*/)
//    	: _p(i)
////    	, _replacement_char(replacement_char)
//    {}

    //
	// iterator traits
	//

    iterator (const iterator & other)
	: _p(other._p)
//	, _replacement_char(other._replacement_char)
    {}

	~iterator () {}

	iterator & operator = (const iterator & other)
	{
    	_p = other._p;
//    	_replacement_char = other._replacement_char;
		return *this;
	}

	iterator & operator ++ ()
	{
		utf_traits_type::advance_forward(_p, 1);
    	return *this;
	}

	//
	// input iterator traits
	//
	iterator operator ++ (int)
	{
		iterator r(*this);
		utf_traits_type::advance_forward(_p, 1);
    	return r;
	}

	value_type operator * () const
	{
		value_type result;
		pointer p = _p;
		utf_traits_type::decode(p);
		return result;
	}

	//	pointer operator -> () const;

	//
	// bidirectional iterator traits
    //
	iterator & operator -- ()
	{
		utf_traits_type::advance_backward(_p, 1);
    	return *this;
	}

	iterator operator -- (int)
	{
		iterator r(*this);
		utf_traits_type::advance_backward(_p, 1);
    	return r;
	}

    pointer base () const
    {
    	return _p;
    }

	friend void swap (iterator & lhs, iterator & rhs)
	{
		std::swap(lhs._p, rhs._p);
//		std::swap(lhs._replacement_char, rhs._replacement_char);
	}

	friend void advance (iterator & i, difference_type n)
	{
		if (n > 0)
			utf_traits_type::advance_forward(i._p, n);
		else if (n < 0)
			utf_traits_type::advance_backward(i._p, -n);
	}

	friend bool operator == (const iterator & lhs, const iterator & rhs)
	{
		return lhs._p == rhs._p;
	}

    friend bool operator != (const iterator & lhs, const iterator & rhs)
	{
    	return lhs._p != rhs._p;
	}

	friend bool operator < (const iterator & lhs, const iterator & rhs)
	{
		return lhs._p < rhs._p;
	}

	friend bool operator <= (const iterator & lhs, const iterator & rhs)
	{
		return lhs._p <= rhs._p;
	}

	friend bool operator > (const iterator & lhs, const iterator & rhs)
	{
		return lhs._p > rhs._p;
	}

	friend bool operator >= (const iterator & lhs, const iterator & rhs)
	{
		return lhs._p >= rhs._p;
	}
};

}} // pfs::utf

namespace std {

//#if __cplusplus >= 201103L
//
//template <>
//inline _ForwardIterator
//    next(_ForwardIterator __x, typename
//	 iterator_traits<_ForwardIterator>::difference_type __n = 1)
//    {
//      std::advance(__x, __n);
//      return __x;
//    }
//
//  template<typename _BidirectionalIterator>
//    inline _BidirectionalIterator
//    prev(_BidirectionalIterator __x, typename
//	 iterator_traits<_BidirectionalIterator>::difference_type __n = 1)
//    {
//      std::advance(__x, -__n);
//      return __x;
//    }
//
//#endif // C++11


} // std

#endif /* __PFS_UTF_ITERATOR_HPP__ */
