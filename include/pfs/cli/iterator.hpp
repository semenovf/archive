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
namespace details {

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
    typedef typename traits_type::char_type      char_type;
    
protected:
    typename string_type::const_pointer * _begin;
    typename string_type::const_pointer * _p;
    typename string_type::const_pointer * _end;
    bool        _expected_args;
    char_type   _delim_char;
    string_type _arg_seperator;
    string_type _shortopt_prefix;
    string_type _longopt_prefix;
    
public:
    iterator (typename string_type::const_pointer * begin
            , typename string_type::const_pointer * end)
        : _begin(begin)
        , _p(begin)
        , _end(end)
        , _expected_args(false)
        , _delim_char('=')
        , _arg_seperator("--")
        , _shortopt_prefix("-")
        , _longopt_prefix("--")
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

    string_type const token(*_p);
    
    typename string_type::const_iterator begin = token.begin();
    typename string_type::const_iterator end   = token.end();
    typename string_type::const_iterator it    = begin;
    
    if (token.starts_with(_longopt_prefix))
        it += _longopt_prefix.size();
    else if (token.starts_with(_shortopt_prefix))
        it += _shortopt_prefix.size();
    
    result.prefix = string_type(begin, it);
    begin = it;
    
    if (result.prefix.empty()) {
        result.option = string_type(begin, begin);
        result.arg = string_type(begin, end);
    } else {
        while (it != end && *it != _delim_char)
            ++it;

        result.option = string_type(begin, it);
        
        if (it != end && *it == _delim_char)
            ++it;
        
        begin = it;
        result.arg = string_type(begin, end);
    }
    
    if (result.prefix == _arg_seperator
            && result.option.epmty()
            && result.arg.empty()) {
        
        _expected_args = true;
    }
    
    return result;
}

} // details

template <typename Traits>
inline details::iterator<Traits> begin (int argc, char const ** argv)
{
    return details::iterator<Traits>(argv + 1, argv + argc);
}

template <typename Traits>
inline details::iterator<Traits> end (int argc, char const ** argv)
{
    return details::iterator<Traits>(argv + argc, argv + argc);
}

template <typename Traits>
inline details::iterator<Traits> begin (char const ** b, char const ** e)
{
    return details::iterator<Traits>(b, e);
}

template <typename Traits>
inline details::iterator<Traits> end (char const ** b, char const ** e)
{
    return details::iterator<Traits>(b, e);
}

}} // pfs::cli::details

#endif /* __PFS_CLI_ITERATOR_HPP__ */

