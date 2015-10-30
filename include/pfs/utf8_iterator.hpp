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

namespace pfs {

template <typename Iterator>
class utf8_iterator
{
public:
    typedef bidirectional_iterator_tag iterator_category;
    typedef uint32_t                   value_type;
    typedef ptrdiff_t                  difference_type;
    typedef Iterator                   pointer;
    typedef uint32_t &                 reference;

protected:
	pointer  _p;
	uint32_t _replacement_char;

public:
    explicit utf8_iterator (pointer i)
    	: _p(i)
    	, _replacement_char('?')
    {}

    utf8_iterator (pointer i, uint32_t replacement_char)
    	: _p(i)
    	, _replacement_char(replacement_char)
    {}

	// iterator traits
	//
	utf8_iterator (const utf8_iterator &);
	~utf8_iterator () {}

	utf8_iterator & operator = (const utf8_iterator & other)
	{
    	_p = other._p;
    	_replacement_char = other._replacement_char;
		return *this;
	}

	utf8_iterator & operator ++ ()
	{
    	advance_forward_trusted(1);
    	return *this;
	}

	// input iterator traits
	//
	utf8_iterator operator ++ (int)
	{
    	utf8_iterator r(*this);
    	advance_forward_trusted(1);
    	return r;
	}

	value_type operator * () const;
	pointer operator -> () const;

	// bidirectional iterator traits
    //
    utf8_iterator & operator -- ()
	{
    	advance_backward_trusted(1);
    	return *this;
	}

    utf8_iterator operator -- (int)
	{
    	utf8_iterator r(*this);
    	advance_backward_trusted(1);
    	return r;
	}

    pointer base () const
    {
    	return _p;
    }

	friend void swap (utf8_iterator & lhs, utf8_iterator & rhs)
	{
		std::swap(lhs._p, rhs._p);
		std::swap(lhs._replacement_char, rhs._replacement_char);
	}

	friend void advance (utf8_iterator & i, typename utf8_iterator::difference_type n)
	{
		if (n > 0)
			i.advance_forward_trusted(n);
		else if (n < 0)
			i.advance_backward_trusted(-n);
	}

	friend bool operator == (const utf8_iterator & lhs, const utf8_iterator & rhs)
	{
		return lhs._p == rhs._p;
	}

    friend bool operator != (const utf8_iterator & lhs, const utf8_iterator & rhs)
	{
    	return lhs._p != rhs._p;
	}

	friend bool operator < (const utf8_iterator & lhs, const utf8_iterator & rhs)
	{
		return lhs._p < rhs._p;
	}

	friend bool operator <= (const utf8_iterator & lhs, const utf8_iterator & rhs)
	{
		return lhs._p <= rhs._p;
	}

	friend bool operator > (const utf8_iterator & lhs, const utf8_iterator & rhs)
	{
		return lhs._p >= rhs._p;
	}

	friend bool operator >= (const utf8_iterator & lhs, const utf8_iterator & rhs)
	{
		return lhs._p >= rhs._p;
	}

private:
    void advance_forward_trusted (utf8_iterator::difference_type n);
    void advance_backward_trusted (utf8_iterator::difference_type n);
};

template <typename Iterator>
void utf8_iterator<Iterator>::advance_forward_trusted (utf8_iterator::difference_type n)
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

template <typename Iterator>
void utf8_iterator<Iterator>::advance_backward_trusted (utf8_iterator::difference_type n)
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

} // pfs

namespace std {

//template <typename Iterator>
//inline void advance<pfs::utf8_iterator<Iterator>, pfs::utf8_iterator<Iterator>::difference_type>
//	(pfs::utf8_iterator<Iterator> & i, pfs::utf8_iterator<Iterator>::difference_type n)
//{
//	pfs::advance(i, n);
//}

} // std

#endif /* __PFS_UTF8_ITERATOR_HPP__ */
