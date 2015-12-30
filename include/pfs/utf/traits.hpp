/*
 * utf_traits.hpp
 *
 *  Created on: Dec 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF_TRAITS_HPP__
#define __PFS_UTF_TRAITS_HPP__

#include <iterator>
#include <pfs/unicode/unicode.hpp>

namespace pfs { namespace utf {

template <typename Container, typename UtfTag>
struct traits
{
	typedef UtfTag                                      utf_tag;
	typedef Container                                   container_type;
	typedef unicode::char_type                          value_type;
    typedef typename Container::pointer                 pointer;
    typedef typename Container::const_pointer           const_pointer;
	typedef unicode::char_type &                        reference;
	typedef const unicode::char_type &                  const_reference;
    typedef typename Container::allocator_type          allocator_type;
    typedef typename Container::size_type	            size_type;
    typedef ptrdiff_t                                   difference_type;

    template <typename CodeUnitIterator>
    static value_type decode (CodeUnitIterator & p);

//    static value_type decode (const_pointer & p);
//    static value_type decode (pointer & p)
//    {
//    	return decode(const_pointer(p));
//    }

    static pointer  encode (value_type uc, pointer begin);
    static void     encode (value_type uc, std::back_insert_iterator<Container> begin);

    template <typename CodeUnitIterator>
    static void advance_forward  (CodeUnitIterator & p, difference_type n);

    template <typename CodeUnitIterator>
    static void advance_backward (CodeUnitIterator & p, difference_type n);

//    static void advance_forward  (const_pointer & p, difference_type n);
//    static void advance_forward  (pointer & p, difference_type n);
//    static void advance_backward (const_pointer & p, difference_type n);
//    static void advance_backward (pointer & p, difference_type n);

    template <typename CodeUnitIterator>
    class iterator
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
    	iterator ()
    		: _p(0)
    	{}

        explicit iterator (pointer i)
        	: _p(i)
        {}

        //
    	// iterator traits
    	//

        iterator (const iterator & other)
    		: _p(other._p)
        {}

    	~iterator () {}

    	iterator & operator = (const iterator & other)
    	{
        	_p = other._p;
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
};

//template <typename CodeUnit>
//struct tag_trait;

}} // pfs

#endif /* __PFS_UTF_TRAITS_HPP__ */
