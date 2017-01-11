/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 10:49 AM
 * 
 * @brief Immutable string interface.
 */

#ifndef __PFS_MPL_STRING_HPP__
#define __PFS_MPL_STRING_HPP__

#include <pfs/mpl/algo/find.hpp>
#include <pfs/type_traits.hpp>

namespace pfs {
namespace mpl {

template <typename T>
struct string_traits
{
    typedef T const &                          const_impl_reference;
    typedef typename T::size_type              size_type;
    typedef typename T::value_type             value_type;
    typedef typename T::const_pointer          const_pointer;
    typedef typename T::const_iterator         const_iterator;
    typedef typename T::const_reverse_iterator const_reverse_iterator;
    
    typedef T data_type;
};

template <typename T>
class basic_string;

namespace details {
    
template <typename T>
class basic_string 
{
public:
    typedef string_traits<T>                        traits;
    typedef typename traits::const_impl_reference   const_impl_reference;
    typedef typename traits::size_type              size_type;
    typedef typename traits::value_type             value_type;
    typedef typename traits::const_pointer          const_pointer;
    typedef typename traits::const_iterator         const_iterator;
    typedef typename traits::const_reverse_iterator const_reverse_iterator;
    typedef typename traits::data_type              data_type;

public:
    data_type _d;

protected:
    virtual int xcompare (size_type pos1, size_type count1
        , basic_string const & rhs, size_type pos2, size_type count2) const = 0;

public:
    basic_string ()
    {}

    basic_string (data_type const & d)
        : _d(d)
    {}
    
    virtual size_type size () const = 0;
    
    size_type length () const
    {
        return this->size();
    }
    
    bool empty () const
    {
    	return this->size() == 0;
    }
    
    virtual const_iterator begin () const = 0;

    virtual const_iterator end () const = 0;

    const_iterator cbegin () const
    {
        return this->begin();
    }

    const_iterator cend () const
    {
        return this->end();
    }

    virtual const_reverse_iterator rbegin () const = 0;

    virtual const_reverse_iterator rend () const = 0;

    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }

    const_reverse_iterator crend () const
    {
        return this->rend();
    }

    int compare (size_type pos1, size_type count1
        , basic_string const & rhs, size_type pos2, size_type count2) const
    {
        return this->xcompare(pos1, count1, rhs, pos2, count2) ;
    }
    
    int compare (size_type pos1, size_type count1, basic_string const & rhs) const
    {
        return this->xcompare(pos1, count1, rhs, 0, rhs.size());
    }
    
    int compare (basic_string const & rhs) const
    {
        return this->xcompare(0, this->size(), rhs, 0, rhs.size());
    }
    
    bool starts_with (basic_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return this->compare(0, rhs.size(), rhs) == 0;
    }
    
    /**
     * 
     * @param rhs
     * @return 
     */
  	bool ends_with (basic_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return this->compare(this->size() - rhs.size(), rhs.size(), rhs) == 0;
    }
};

} // details

template <typename T>
class string : public basic_string<T>
{
    typedef basic_string<T> basic_type;
    
public:    
    typedef typename basic_type::traits                 traits;
    typedef typename basic_type::const_impl_reference   const_impl_reference;
    typedef typename basic_type::size_type              size_type;
    typedef typename basic_type::value_type             value_type;
    typedef typename basic_type::const_pointer          const_pointer;
    typedef typename basic_type::const_iterator         const_iterator;
    typedef typename basic_type::const_reverse_iterator const_reverse_iterator;
    
public:
    string ()
        : basic_type()
    {}
    
    string (const_impl_reference s)
        : basic_type(s)
    {}
    
    string (const_iterator begin, const_iterator end)
        : basic_type(begin, end)
    {}

    string (string const & rhs)
        : basic_type(rhs)
    {}

    string & operator = (string const & rhs)
    {
        basic_type::operator = (rhs);
        return *this;
    }

    string & operator = (const_impl_reference rhs)
    {
        basic_type::operator = (rhs);
        return *this;
    }

//	string substr (const_iterator begin, const_iterator end) const
//    {
////        TODO:        
////        PFS_ASSERT_INVALID_ARGUMENT(begin <= end);
////        PFS_ASSERT_INVALID_ARGUMENT(begin >= _begin);
////        PFS_ASSERT_INVALID_ARGUMENT(end < _end);
//
//        return string(begin, end);
//    }
//    
//    string substr (const_iterator begin) const
//    {
//        return string(begin, this->end());
//    }
//    
//    string substr (size_type pos, size_type count) const
//    {
//        return this->substr(begin + pos, begin + pos + count);
//    }
//    
//	string substr (size_t pos) const
//    {
//        return this->substr(begin + pos, this->end());
//    }
//
//	string left (size_t count) const
//	{
//		return substr(0, count);
//	}
//
//	string right (size_t count) const
//	{
//		return substr(length() - count, count);
//	}
};

template <typename T>
inline bool operator == (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename T>
inline bool operator != (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) != 0;
}

template <typename T>
inline bool operator < (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename T>
inline bool operator <= (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

template <typename T>
inline bool operator > (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename T>
inline bool operator >= (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

}} // pfs::mpl

#endif /* __PFS_MPL_STRING_HPP__ */

