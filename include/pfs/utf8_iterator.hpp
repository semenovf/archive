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

using std::bidirectional_iterator_tag;

namespace pfs {

template <typename Iterator>
class __utf8_iterator
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
    explicit __utf8_iterator (pointer & i)
    	: _p(i)
    	, _replacement_char('?')
    {}

    __utf8_iterator (pointer i, uint32_t replacement_char)
    	: _p(i)
    	, _replacement_char(replacement_char)
    {}

    // Allow iterator to const_iterator conversion
//    template<typename Iter>
//    __utf8_iterator (const __utf8_iterator<Iter,
//			  typename enable_if<
//    	       (std::__are_same<Iter, Iterator>::__value),
//		      _Container>::__type>& __i)
//      : _M_current(__i.base()) { }


	// iterator traits
	//
	__utf8_iterator (const __utf8_iterator &);
	~__utf8_iterator () {}

	uint32_t replacement_char () const
	{
		return _replacement_char;
	}

	__utf8_iterator & operator = (const __utf8_iterator &);
	__utf8_iterator & operator ++ ();

	// input iterator traits
	//
	__utf8_iterator operator ++ (int);
	value_type operator * () const;
	pointer operator -> () const;
	friend bool operator == (const __utf8_iterator &, const __utf8_iterator &);
    friend bool operator != (const __utf8_iterator &, const __utf8_iterator &);

	// bidirectional iterator traits
    //
    __utf8_iterator & operator -- ();
    __utf8_iterator operator -- (int);

    pointer base () const { return _p; }

	friend void swap (__utf8_iterator & lhs, __utf8_iterator & rhs);
	friend void advance (__utf8_iterator & i, __utf8_iterator::difference_type n);
};

template <typename Iterator>
inline void swap (__utf8_iterator<Iterator> & lhs
		, __utf8_iterator<Iterator> & rhs)
{
	pfs::swap(lhs._p, rhs._p);
	pfs::swap(lhs._replacement_char, rhs._replacement_char);
}

void __advance (const char *  & p
		, __utf8_iterator<const char *>::difference_type n);

inline void __advance (char *  & p
		, __utf8_iterator<char *>::difference_type n)
{
	__advance(reinterpret_cast<const char * &>(p), n);
}

template <typename Iterator>
inline void advance (__utf8_iterator<Iterator> & i
		, __utf8_iterator<Iterator>::difference_type n)
{
    __advance(i._p, n);
}

typedef __utf8_iterator<char *> utf8_iterator;
typedef __utf8_iterator<const char *> utf8_const_iterator;

} // pfs

namespace std {

//template <>
//inline void advance<pfs::utf8_iterator, pfs::utf8_iterator::difference_type>
//	(pfs::utf8_iterator & i, pfs::utf8_iterator::difference_type n)
//{
//	pfs::advance(i, n);
//}

} // std

#endif /* __PFS_UTF8_ITERATOR_HPP__ */
