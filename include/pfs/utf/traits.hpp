/*
 * utf_traits.hpp
 *
 *  Created on: Dec 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF_TRAITS_HPP__
#define __PFS_UTF_TRAITS_HPP__

#include <iterator>
#include <pfs/unicode.hpp>

namespace pfs { namespace utf {

template <typename Rep, typename UtfTag>
struct traits
{
	typedef UtfTag                       utf_tag;
	typedef Rep                          rep_type;
	typedef unicode::char_type           value_type;
	typedef unicode::char_type &         reference;
	typedef const unicode::char_type &   const_reference;
    typedef typename Rep::pointer        pointer;
    typedef typename Rep::const_pointer  const_pointer;
    typedef typename Rep::allocator_type allocator_type;
    typedef typename Rep::size_type	     size_type;
    typedef ptrdiff_t                    difference_type;

    typedef typename Rep::iterator       rep_iterator;
    typedef typename Rep::const_iterator rep_const_iterator;

    template <typename CodeUnitIterator>
    static value_type decode (CodeUnitIterator & p);

    static pointer encode (value_type uc, pointer begin);
    static void    encode (value_type uc, std::back_insert_iterator<Rep> begin);

    template <typename CodeUnitIterator>
    static void advance_forward  (CodeUnitIterator & p, difference_type n);

    template <typename CodeUnitIterator>
    static void advance_backward (CodeUnitIterator & p, difference_type n);

    template <typename CodeUnitIterator>
    class iterator_impl
    {
    public:
    	typedef traits                                    utf_traits_type;
        typedef typename utf_traits_type::value_type      value_type;
        typedef CodeUnitIterator                          pointer;
        typedef typename utf_traits_type::reference       reference; // std::advance() requirement
        typedef typename utf_traits_type::difference_type difference_type;
        typedef std::bidirectional_iterator_tag           iterator_category;

    protected:
    	pointer  _p;

    public:
    	iterator_impl ()
    		: _p(0)
    	{}

        explicit iterator_impl (pointer i)
        	: _p(i)
        {}

        //
    	// iterator traits
    	//

        iterator_impl (const iterator_impl & other)
    		: _p(other._p)
        {}

    	~iterator_impl () {}

    	iterator_impl & operator = (const iterator_impl & other)
    	{
        	_p = other._p;
    		return *this;
    	}

    	iterator_impl & operator ++ ()
    	{
    		utf_traits_type::advance_forward(_p, 1);
        	return *this;
    	}

    	//
    	// input iterator traits
    	//
    	iterator_impl operator ++ (int)
    	{
    		iterator_impl r(*this);
    		utf_traits_type::advance_forward(_p, 1);
        	return r;
    	}

    	value_type operator * () const
    	{
    		pointer p = _p;
    		return utf_traits_type::decode(p);
    	}

    	//	pointer operator -> () const;

    	//
    	// bidirectional iterator traits
        //
    	iterator_impl & operator -- ()
    	{
    		utf_traits_type::advance_backward(_p, 1);
        	return *this;
    	}

    	iterator_impl operator -- (int)
    	{
    		iterator_impl r(*this);
    		utf_traits_type::advance_backward(_p, 1);
        	return r;
    	}

        pointer base () const
        {
        	return _p;
        }

    	friend void swap (iterator_impl & lhs, iterator_impl & rhs)
    	{
    		std::swap(lhs._p, rhs._p);
    	}

    	friend void advance (iterator_impl & i, difference_type n)
    	{
    		if (n > 0)
    			utf_traits_type::advance_forward(i._p, n);
    		else if (n < 0)
    			utf_traits_type::advance_backward(i._p, -n);
    	}

    	friend bool operator == (const iterator_impl & lhs, const iterator_impl & rhs)
    	{
    		return lhs._p == rhs._p;
    	}

        friend bool operator != (const iterator_impl & lhs, const iterator_impl & rhs)
    	{
        	return lhs._p != rhs._p;
    	}

    	friend bool operator < (const iterator_impl & lhs, const iterator_impl & rhs)
    	{
    		return lhs._p < rhs._p;
    	}

    	friend bool operator <= (const iterator_impl & lhs, const iterator_impl & rhs)
    	{
    		return lhs._p <= rhs._p;
    	}

    	friend bool operator > (const iterator_impl & lhs, const iterator_impl & rhs)
    	{
    		return lhs._p > rhs._p;
    	}

    	friend bool operator >= (const iterator_impl & lhs, const iterator_impl & rhs)
    	{
    		return lhs._p >= rhs._p;
    	}
    };

    typedef iterator_impl<pointer>       iterator;
    typedef iterator_impl<const_pointer> const_iterator;

    static rep_iterator to_rep_iterator (iterator it)
    {
    	return rep_iterator(it.base());
    }

    static rep_const_iterator to_rep_iterator (const_iterator it)
    {
    	return rep_const_iterator(it.base());
    }

};

}} // pfs

#endif /* __PFS_UTF_TRAITS_HPP__ */
