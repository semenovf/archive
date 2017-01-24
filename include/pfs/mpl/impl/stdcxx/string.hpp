/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 3:21 PM
 */

#ifndef __PFS_MPL_STDCXX_STRING_HPP__
#define __PFS_MPL_STDCXX_STRING_HPP__

#include <string>
#include <pfs/mpl/string.hpp>

namespace pfs {
namespace mpl {
namespace stdcxx {

template <typename T>
class basic_string : public pfs::mpl::details::basic_string<std::basic_string<T> >
{
    typedef pfs::mpl::details::basic_string<std::basic_string<T> > base_type;

public:    
    typedef typename base_type::traits_type            traits_type;
    typedef typename base_type::const_native_reference const_native_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::data_type              data_type;

protected:
    virtual size_type xsize () const
    {
        return this->_d.size();
    }

    virtual const_iterator xbegin () const
    {
        return this->_d.begin();
    }

    virtual const_iterator xend () const
    {
        return this->_d.end();
    }

    virtual const_reverse_iterator xrbegin () const
    {
        return this->_d.rbegin();
    }

    virtual const_reverse_iterator xrend () const
    {
        return this->_d.rend();
    }
    
    virtual value_type xat (size_type pos) const
    {
        return this->_d.at(pos);
    }

    virtual int xcompare (size_type pos1, size_type count1
        , base_type const & rhs, size_type pos2, size_type count2) const
    {
        return this->_d.compare(pos1, count1, rhs._d, pos2, count2);
    }
    
    virtual size_type xfind (const_native_reference rhs, size_type pos) const
    {
        return this->_d.find(rhs, pos);
    }

    virtual size_type xfind (value_type c, size_type pos) const
    {
        return this->_d.find(c, pos);
    }

    virtual size_type xrfind (const_native_reference rhs, size_type pos) const
    {
        return this->_d.rfind(rhs, pos);
    }

    virtual size_type xrfind (value_type c, size_type pos) const
    {
        return this->_d.rfind(c, pos);
    }
    
public:
    basic_string ()
        : base_type()
    {}

    basic_string (const_iterator begin, const_iterator end)
    {
        this->_d = data_type(begin, end);
    }

    basic_string (const_native_reference s)
    {
        this->_d = s;
    }

    basic_string (basic_string const & rhs)
    {
        this->_d = rhs._d;
    }
    
    basic_string & operator = (basic_string const & rhs)
    {
        if (this != & rhs)
            this->_d = rhs._d;
        return *this;
    }

    virtual const_native_reference native () const
    {
        return this->_d;
    }
};

} // stdcxx

template <>
class basic_string<std::string> : public stdcxx::basic_string<char>
{
public:
    typedef stdcxx::basic_string<char>                 base_type;
    typedef typename base_type::traits_type            traits;
    typedef typename base_type::const_native_reference const_native_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::data_type              data_type;
    
public:
    basic_string ()
        : base_type()
    {}

    basic_string (const_native_reference s)
        : base_type(s)
    {}
    
    basic_string (const_iterator begin, const_iterator end)
        : base_type(begin, end)
    {}

    basic_string (basic_string const & rhs)
        : base_type(rhs)
    {}
    
    explicit basic_string (char const * str, size_type n = size_type(-1))
    {
        this->_d = data_type(str, n);
    }
    
#ifdef _WCHAR_H
    // TODO Implement conversion from
    // UTF-16 (sizeof(wchar_t) == 2) or UTF-32 (sizeof(wchar_t) == 4)
    // to UTF-8
    explicit basic_string (wchar_t const * str, size_type n = size_type(-1))
    {
        throw logic_error("Implement");
    }
#endif
};

template <>
class basic_string<std::wstring> : public stdcxx::basic_string<wchar_t>
{
public:
    typedef stdcxx::basic_string<wchar_t>              base_type;
    typedef typename base_type::traits_type            traits;
    typedef typename base_type::const_native_reference const_native_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;
    typedef typename base_type::data_type              data_type;
    
public:
    basic_string ()
        : base_type()
    {}

    basic_string (const_native_reference s)
        : base_type(s)
    {}
    
    basic_string (const_iterator begin, const_iterator end)
        : base_type(begin, end)
    {}

    basic_string (basic_string const & rhs)
        : base_type(rhs)
    {}

    // TODO Implement conversion from UTF-8 to 
    // UTF-16 (sizeof(wchar_t) == 2) or UTF-32 (sizeof(wchar_t) == 4)
    explicit basic_string (char const * str, size_type n = size_type(-1))
    {
        throw logic_error("Implement");
    }
    
    explicit basic_string (wchar_t const * str, size_type n = size_type(-1))
    {
        this->_d = data_type(str, n);
    }
};

}} // pfs::mpl

namespace std {

template <>
inline ostream & operator<< <std::string> (ostream & out, pfs::mpl::string<std::string> const & s)
{
    out << s.native();
    return out;
}

} // std

#endif /* __PFS_MPL_STDCXX_STRING_HPP__ */