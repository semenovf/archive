/**
 * @file utf8string_iterator.hpp
 * @author wladt
 * @date Nov 22, 2013
 */

#ifndef __PFS_BITS_UTF8STRING_ITERATOR_HPP__
#define __PFS_BITS_UTF8STRING_ITERATOR_HPP__

#include <iterator>
#include <pfs/bits/uccharptr.hpp>

namespace pfs {

/*
template <typename _Ptr2>
friend class utf8string_iterator;
*/

template <typename _Ptr>
class utf8string_iterator
	: public std::iterator<
	  	  std::random_access_iterator_tag
	  	  , typename _Ptr::value_type
	  	  , typename _Ptr::difference_type
	  	  , typename _Ptr::pointer
	  	  , typename _Ptr::reference>
{

	template <typename _Ptr2>
	friend class utf8string_iterator;

protected:
	_Ptr _p;
	typedef std::iterator<
		  	  std::random_access_iterator_tag
		  	  , typename _Ptr::value_type
		  	  , typename _Ptr::difference_type
		  	  , typename _Ptr::pointer
		  	  , typename _Ptr::reference> base_class;
    typedef std::iterator_traits<base_class> traits_type;

public:
    typedef typename traits_type::iterator_category iterator_category;
    typedef typename traits_type::value_type        value_type;
    typedef typename traits_type::difference_type   difference_type;
    typedef typename traits_type::reference         reference;
    typedef typename traits_type::pointer           pointer;
    typedef typename _Ptr::owner_type               owner_type;

    utf8string_iterator () : _p() {}
    utf8string_iterator (owner_type & s) : _p(s) {}

    // Allow ucchar_ptr to ucchar_const_ptr conversion
    template<typename _Ptr2>
    utf8string_iterator (const _Ptr2 & p) : _p(p) { }

    // Allow iterator to const_iterator conversion
    template<typename _Ptr2>
    utf8string_iterator(const utf8string_iterator<_Ptr2> & p)
    	: _p(p._p) { }

    // Forward iterator requirements
    const reference operator * () const
    {
    	return *_p;
    }

    reference operator * ()
    {
    	return *_p;
    }

    const pointer operator -> () const { return _p; }
    pointer operator -> () { return _p; }

    utf8string_iterator & operator ++ ()
    {
    	++_p;
    	return *this;
    }

    utf8string_iterator operator ++ (int)
    {
    	return utf8string_iterator(_p++);
    }

    // Bidirectional iterator requirements
    utf8string_iterator & operator -- ()
    {
    	--_p;
    	return *this;
    }

    utf8string_iterator operator -- (int)
    {
    	return utf8string_iterator(_p--);
    }

    // Random access iterator requirements
    const reference operator [] (const difference_type & n) const
    {
    	return _p[n];
    }

    reference operator [] (const difference_type & n)
    {
    	return _p[n];
    }

    utf8string_iterator & operator += (const difference_type & n)
	{
    	_p += n;
    	return *this;
	}

    utf8string_iterator operator + (const difference_type & n) const
    {
    	return utf8string_iterator(_p + n);
    }

    utf8string_iterator & operator -= (const difference_type & n)
    {
    	_p -= n;
    	return *this;
    }

    utf8string_iterator operator - (const difference_type & n) const
    {
    	return utf8string_iterator(_p - n);
    }

    _Ptr base() const
    {
    	return _p;
    }

    difference_type distance (const utf8string_iterator & it) const
    {
    	return _p.distance(it._p);
    }

};

// Forward iterator requirements
template<typename _I1, typename _I2>
inline bool operator == (const utf8string_iterator<_I1> & it1, const utf8string_iterator<_I2> & it2)
{
	return it1.base() == it2.base();
}

template<typename _I>
inline bool operator == (const utf8string_iterator<_I> & it1, const utf8string_iterator<_I> & it2)
{
	return it1.base() == it2.base();
}

template<typename _I1, typename _I2>
inline bool operator != (const utf8string_iterator<_I1> & it1, const utf8string_iterator<_I2> & it2)
{
	return it1.base() != it2.base();
}

template<typename _I>
inline bool operator != (const utf8string_iterator<_I> & it1, const utf8string_iterator<_I> & it2)
{
	return it1.base() != it2.base();
}

// Random access iterator requirements
template<typename _I1, typename _I2>
inline bool operator < (const utf8string_iterator<_I1> & it1, const utf8string_iterator<_I2> & it2)
{
	return it1.base() < it2.base();
}

template<typename _I>
inline bool operator < (const utf8string_iterator<_I> & it1, const utf8string_iterator<_I> & it2)
{
	return it1.base() < it2.base();
}

template<typename _I1, typename _I2>
inline bool operator > (const utf8string_iterator<_I1> & it1 , const utf8string_iterator<_I2> & it2)
{
	return it1.base() > it2.base();
}

template<typename _I>
inline bool operator > (const utf8string_iterator<_I> & it1, const utf8string_iterator<_I> & it2)
{
	return it1.base() > it2.base();
}

template<typename _I1, typename _I2>
inline bool operator <= (const utf8string_iterator<_I1> & it1, const utf8string_iterator<_I2> & it2)
{
	return it1.base() <= it2.base();
}

template<typename _I>
inline bool operator <= (const utf8string_iterator<_I> & it1, const utf8string_iterator<_I> & it2)
{
	return it1.base() <= it2.base();
}

template<typename _I1, typename _I2>
inline bool operator >= (const utf8string_iterator<_I1> & it1, const utf8string_iterator<_I2> & it2)
{
	return it1.base() >= it2.base();
}

template<typename _I>
inline bool operator >= (const utf8string_iterator<_I> & it1, const utf8string_iterator<_I> & it2)
{
	return it1.base() >= it2.base();
}


// TODO May be need to implement operator- (iter1, iter2) ?
template<typename _I1, typename _I2>
inline typename utf8string_iterator<_I1>::difference_type
operator - (const utf8string_iterator<_I1> & /*it1*/, const utf8string_iterator<_I2> & /*it2*/)
{
	// deny use of this operator for forward and reverse iterators
	PFS_ASSERT(0);
	//return it1.base() - it2.base();
}

template<typename _I>
inline typename utf8string_iterator<_I>::difference_type
operator - (const utf8string_iterator<_I> & /*it1*/, const utf8string_iterator<_I> & /*it2*/)
{
	// deny use of this operator for forward and reverse iterators
	PFS_ASSERT(0);
	//return it1.base() - it2.base();
}

// TODO May be need to implement operator+(n, iter) ?
template<typename _I>
inline utf8string_iterator<_I>
operator + (typename utf8string_iterator<_I>::difference_type n, const utf8string_iterator<_I> & /*it*/)
{
	// deny use of this operator
	PFS_ASSERT(0);
//	return utf8string_iterator<_I>(it.base() + n);
}

} // pfs

#endif /* __PFS_BITS_UTF8STRING_ITERATOR_HPP__ */
