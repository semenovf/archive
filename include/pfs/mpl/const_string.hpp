/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   const_string.hpp
 * Author: wladt
 *
 * Created on December 26, 2016, 4:27 PM
 * 
 * @brief Immutable string interface.
 */

#ifndef __PFS_MPL_CONST_STRING_HPP__
#define __PFS_MPL_CONST_STRING_HPP__

#include <pfs/mpl/algo/find.hpp>
#include <pfs/type_traits.hpp>

namespace pfs {
namespace mpl {

template <typename ConstStringImpl>
struct const_string_traits
{
    typedef ConstStringImpl const &                  const_impl_reference;
    typedef typename ConstStringImpl::size_type      size_type;
    typedef typename ConstStringImpl::value_type     value_type;
    typedef typename ConstStringImpl::const_pointer  const_pointer;
    typedef typename ConstStringImpl::const_iterator const_iterator;
    typedef typename ConstStringImpl::const_reverse_iterator const_reverse_iterator;
    ConstStringImpl data_type;
};

template <typename ConstStringImpl>
class const_string
{
    typedef const_string_traits<ConstStringImpl> traits;

public:
    typedef typename traits::const_impl_reference   const_impl_reference;
    typedef typename traits::size_type              size_type;
    typedef typename traits::value_type             value_type;
    typedef typename traits::const_pointer          const_pointer;
    typedef typename traits::const_iterator         const_iterator;
    typedef typename traits::const_reverse_iterator const_reverse_iterator;

public:
    typename traits::data_type _d;
    
public:
    const_string ();
    
    const_string (const_impl_reference s);
    
    const_string (const_iterator begin, const_iterator end);
    
    const_string (const_string const & rhs)
        : _d(rhs._d)
    {}
        
    const_string & operator = (const_string const & rhs)
    {
        if (this != & rhs)
            _d = rhs._d;
        return *this;
    }
    
    const_string & operator = (const_pointer rhs);
    
    const_string & operator = (ConstStringImpl const & p);

    const_iterator begin () const;

    const_iterator end () const;

    const_iterator cbegin () const
    {
        return this->begin();
    }

    const_iterator cend () const
    {
        return this->end();
    }

    const_reverse_iterator rbegin () const;

    const_reverse_iterator rend () const;

    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }

    const_reverse_iterator crend () const
    {
        return this->rend();
    }
    
	const_pointer data () const;

	size_type size () const;

    size_type length () const;

    bool empty () const
    {
    	return size() == 0;
    }

	int compare (const_string const & rhs) const;
    
    bool starts_with (const_string const & rhs) const
    {
        return pfs::mpl::starts_with(this->begin(), this->end()
                , rhs.begin(), rhs.end());
    }

	bool ends_with (const_string const & rhs) const
    {
        return pfs::mpl::ends_with(this->begin(), this->end()
                , rhs.begin(), rhs.end());
    }

	const_string substr (const_iterator begin, const_iterator end) const
    {
//        TODO:        
//        PFS_ASSERT_INVALID_ARGUMENT(begin <= end);
//        PFS_ASSERT_INVALID_ARGUMENT(begin >= _begin);
//        PFS_ASSERT_INVALID_ARGUMENT(end < _end);

        return const_string(begin, end);
    }
    
    const_string substr (const_iterator begin) const
    {
        return const_string(begin, this->end());
    }
    
    const_string substr (size_type pos, size_type count) const
    {
        return this->substr(begin + pos, begin + pos + count);
    }
    
	const_string substr (size_t pos) const
    {
        return this->substr(begin + pos, this->end());
    }

	const_string left (size_t count) const
	{
		return substr(0, count);
	}

	const_string right (size_t count) const
	{
		return substr(length() - count, count);
	}
};

template <typename ConstStringImpl>
inline bool operator == (const_string<ConstStringImpl> const & lhs
    , const_string<ConstStringImpl> const & rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename ConstStringImpl>
inline bool operator != (const_string<ConstStringImpl> const & lhs
    , const_string<ConstStringImpl> const & rhs)
{
    return lhs.compare(rhs) != 0;
}

template <typename ConstStringImpl>
inline bool operator < (const_string<ConstStringImpl> const & lhs
    , const_string<ConstStringImpl> const & rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename ConstStringImpl>
inline bool operator <= (const_string<ConstStringImpl> const & lhs
    , const_string<ConstStringImpl> const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

template <typename ConstStringImpl>
inline bool operator > (const_string<ConstStringImpl> const & lhs
    , const_string<ConstStringImpl> const & rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename ConstStringImpl>
inline bool operator >= (const_string<ConstStringImpl> const & lhs
    , const_string<ConstStringImpl> const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

}} // pfs::mpl

#endif /* __PFS_MPL_CONST_STRING_HPP__ */
