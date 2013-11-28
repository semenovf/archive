/**
 * @file utf8string_iterator.hpp
 * @author wladt
 * @date Nov 22, 2013
 */

#ifndef __PFS_BITS_UTF8STRING_ITERATOR_HPP__
#define __PFS_BITS_UTF8STRING_ITERATOR_HPP__

#include <iterator>
#include <pfs/bits/utf8string_ptr.hpp>

namespace pfs {

template <typename _P>
class utf8string_iterator
	: public std::iterator<
	  	  std::random_access_iterator_tag
	  	  , typename _P::value_type
	  	  , typename _P::difference_type
	  	  , typename _P::pointer
	  	  , typename _P::reference>
{
protected:
	_P _p;
	typedef std::iterator<
		  	  std::random_access_iterator_tag
		  	  , typename _P::value_type
		  	  , typename _P::difference_type
		  	  , typename _P::pointer
		  	  , typename _P::reference> base_class;
    typedef std::iterator_traits<base_class> traits_type;

public:
    typedef typename traits_type::iterator_category iterator_category;
    typedef typename traits_type::value_type        value_type;
    typedef typename traits_type::difference_type   difference_type;
    typedef typename traits_type::reference         reference;
    typedef typename traits_type::pointer           pointer;

    utf8string_iterator () : _p() {}
    utf8string_iterator (const _P & it) : _p(it) { }
    utf8string_iterator (const utf8string & s);

    difference_type distance (const utf8string_iterator & it) const
    {
    	difference_type d = _p.ptr() - it._p.ptr();
    	return d < 0 ? d * -1 : d;
    }

    // Forward iterator requirements
    value_type operator * () const
    {
    	return *_p;
    }

    struct __ptr {
    	value_type _v;
    	__ptr (value_type v) : _v(v) {}
    	value_type * operator -> () { return & _v; }
    	const value_type * operator -> () const { return & _v; }
    };
    __ptr operator-> () const { return __ptr(*_p); }

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
    reference operator [] (const difference_type & n) const
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

    const _P & base() const
    {
    	return _p;
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

template<typename _I1, typename _I2>
inline typename utf8string_iterator<_I1>::difference_type
operator - (const utf8string_iterator<_I1> & it1, const utf8string_iterator<_I2> & it2)
{
	return it1.base() - it2.base();
}

template<typename _I>
inline typename utf8string_iterator<_I>::difference_type
operator - (const utf8string_iterator<_I> & it1, const utf8string_iterator<_I> & it2)
{
	return it1.base() - it2.base();
}

template<typename _I>
inline utf8string_iterator<_I>
operator + (typename utf8string_iterator<_I>::difference_type n, const utf8string_iterator<_I> & it)
{
	return utf8string_iterator<_I>(it.base() + n);
}

} // pfs

#endif /* __PFS_BITS_UTF8STRING_ITERATOR_HPP__ */
