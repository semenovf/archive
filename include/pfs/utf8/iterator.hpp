/*
 * utf8_iterator.hpp
 *
 *  Created on: Oct 28, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF8_ITERATOR_HPP__
#define __PFS_UTF8_ITERATOR_HPP__

#include <iterator>
#include <pfs/utility.hpp>
#include <pfs/utf8/decode.hpp>

/* UTF-8
 *
 * Bits Last code point/    Byte 1      Byte 2      Byte 3      Byte 4      Byte 5      Byte 6
 *  7   U+007F              0xxxxxxx
 *  11  U+07FF              110xxxxx    10xxxxxx
 *  16  U+FFFF              1110xxxx    10xxxxxx    10xxxxxx
 *  21  U+1FFFFF            11110xxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  26  U+3FFFFFF           111110xx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  31  U+7FFFFFFF          1111110x    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 */

using std::bidirectional_iterator_tag;

namespace pfs { namespace utf8 {

template <typename OctetIterator>
class iterator
{
public:
    typedef bidirectional_iterator_tag iterator_category;
    typedef uint32_t                   value_type;
    typedef ptrdiff_t                  difference_type;
    typedef OctetIterator              pointer;
    typedef uint32_t &                 reference;

protected:
	pointer  _p;
	uint32_t _replacement_char;

public:
    explicit iterator (pointer i)
    	: _p(i)
    	, _replacement_char('?')
    {}

    iterator (pointer i, uint32_t replacement_char)
    	: _p(i)
    	, _replacement_char(replacement_char)
    {}

    //
	// iterator traits
	//

    iterator (const iterator & other)
	: _p(other._p)
	, _replacement_char(other._replacement_char)
    {}

	~iterator () {}

	iterator & operator = (const iterator & other)
	{
    	_p = other._p;
    	_replacement_char = other._replacement_char;
		return *this;
	}

	iterator & operator ++ ()
	{
    	advance_forward(1);
    	return *this;
	}

	//
	// input iterator traits
	//
	iterator operator ++ (int)
	{
    	iterator r(*this);
    	advance_forward(1);
    	return r;
	}

	value_type operator * () const;

//	pointer operator -> () const;

	//
	// bidirectional iterator traits
    //
    iterator & operator -- ()
	{
    	advance_backward(1);
    	return *this;
	}

    iterator operator -- (int)
	{
    	iterator r(*this);
    	advance_backward(1);
    	return r;
	}

    pointer base () const
    {
    	return _p;
    }

	friend void swap (iterator & lhs, iterator & rhs)
	{
		std::swap(lhs._p, rhs._p);
		std::swap(lhs._replacement_char, rhs._replacement_char);
	}

	friend void advance (iterator & i, typename iterator::difference_type n)
	{
		if (n > 0)
			i.advance_forward(n);
		else if (n < 0)
			i.advance_backward(-n);
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

private:
    void advance_forward (iterator::difference_type n);
    void advance_backward (iterator::difference_type n);
};

template <typename OctetIterator>
void iterator<OctetIterator>::advance_forward (iterator::difference_type n)
{
    while (n--) {
        if ((*_p & 0x80) == 0x00) {
            ++_p;
        } else if ((*_p & 0xE0) == 0xC0) {
            _p += 2;
        } else if ((*_p & 0xF0) == 0xE0) {
        	_p += 3;
        } else if ((*_p & 0xF8) == 0xF0) {
        	_p += 4;
        } else if ((*_p & 0xFC) == 0xF8) {
        	_p += 5;
        } else if ((*_p & 0xFE) == 0xFC) {
        	_p += 6;
        } else { // Invalid char
            ++_p;
        }
    }
}

template <typename OctetIterator>
void iterator<OctetIterator>::advance_backward (iterator::difference_type n)
{
    while (n--) {
        if ((*(_p - 1) & 0x80) == 0x00) {
            --_p;
        } else if ((*(_p - 2) & 0xE0) == 0xC0) {
            _p -= 2;
        } else if ((*(_p - 3) & 0xF0) == 0xE0) {
            _p -= 3;
        } else if ((*(_p - 4) & 0xF8) == 0xF0) {
            _p -= 4;
        } else if ((*(_p - 5) & 0xFC) == 0xF8) {
            _p -= 5;
        } else if ((*(_p - 6) & 0xFE) == 0xFC) {
            _p -= 6;
        } else {
            --_p;
        }
    }
}

template <typename OctetIterator>
typename iterator<OctetIterator>::value_type iterator<OctetIterator>::operator * () const
{
	value_type result;
	decode<OctetIterator>(*this, result);
	return result;
}

}} // pfs::utf8

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

#endif /* __PFS_UTF8_ITERATOR_HPP__ */
