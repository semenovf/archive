/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iterator.hpp
 * Author: wladt
 *
 * Created on December 20, 2016, 10:05 AM
 */

#ifndef __PFS_CLI_ITERATOR_HPP__
#define __PFS_CLI_ITERATOR_HPP__

#include <pfs/cli/traits.hpp>

namespace pfs {
namespace cli {

template <typename Traits>
struct tuple
{
    typedef Traits traits_type;
    typedef typename traits_type::string_type string_type;
    
    string_type prefix; // '<empty>', '--' , '-' or '/'
    string_type option;
    string_type arg;
};

template <typename Traits>
class iterator
{
protected:
    typedef Traits                               traits_type;
    typedef tuple<Traits>                        tuple_type;
    typedef typename traits_type::string_type    string_type;
    typedef typename traits_type::char_traits    char_traits;
    typedef typename traits_type::string_traits  string_traits;

    typedef typename string_traits::const_pointer  string_const_pointer;
    typedef typename string_traits::const_iterator string_const_iterator;
    
protected:
    string_const_pointer * _begin;
    string_const_pointer * _p;
    string_const_pointer * _end;
    bool _expected_args;
    
public:
    iterator (string_const_pointer * begin, string_const_pointer * end)
        : _begin(begin)
        , _p(begin)
        , _end(end)
        , _expected_args(false)
    {}
        
    iterator & operator ++ ()
    {
        ++_p;
        return *this;
    }
    
    iterator operator ++ (int)
    {
        iterator it(*this);
        ++_p;
        return it;
    }
    
    string_type operator * () const
    {
        return string_type(*_p);
    }
    
    bool operator == (iterator const & x) const
    {
        return _p == x._p;
    }

    bool operator != (iterator const & x) const
    {
        return !this->operator == (x);
    }
    
    /**
     * @brief Split token into parts.
     * @details Grammar for token:
     *          token = prefix option-name delimiter argument
     *                / prefix option-name
     *                / argument
     * 
     * @return tuple of token parts.
     */
    tuple_type split ();
};

template <typename Traits>
typename iterator<Traits>::tuple_type iterator<Traits>::split ()
{
    tuple_type result;
    
    if (_expected_args) {
        result.arg = string_type(*_p);
        return result;
    }

    string_type const token = string_type(*_p);
    string_const_iterator begin = token.begin();
    string_const_iterator end   = token.end();
    string_const_iterator it    = begin;
    
    while (it != end && char_traits::is_prefix(*it))
        ++it;

    result.prefix = string_type(begin, it);
    begin = it;
    
    if (string_traits::empty(result.prefix)) {
        result.option = string(begin, begin);
        result.arg = string_type(begin, end);
    } else {
        while (it != end && !char_traits::is_delim(*it))
            ++it;

        result.option = string_type(begin, it);
        
        if (it != end && char_traits::is_delim(*it))
            ++it;
        
        begin = it;
        result.arg = string_type(begin, end);
    }
    
    if (result.prefix == string_traits::arg_seperator()
            && string_traits::empty(result.option)
            && string_traits::empty(result.arg)) {
        
        _expected_args = true;
    }
    
    return result;
}

template <typename Traits>
inline iterator<Traits> begin (int argc, char const ** argv)
{
    return iterator<Traits>(argv + 1, argv + argc);
}

template <typename Traits>
inline iterator<Traits> end (int argc, char const ** argv)
{
    return iterator<Traits>(argv + argc, argv + argc);
}

template <typename Traits>
inline iterator<Traits> begin (char const ** b, char const ** e)
{
    return iterator<Traits>(b, e);
}

template <typename Traits>
inline iterator<Traits> end (char const ** b, char const ** e)
{
    return iterator<Traits>(b, e);
}

}} // pfs::cli

#endif /* __PFS_CLI_ITERATOR_HPP__ */

