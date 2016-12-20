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

//
// Command line grammar
//
// command-line    = program-path [ argument-list ] [ positional-separator argument-list ]
// program-path    = <operation system valid file path>
// argument-list   = 1*argument
// argument        = option / non-option
// option          = prefix option-spec
// option-spec     = option-name option-argument-delimiter option-argument
// separator       = "--"
// option-argument = <any valid string (including quotations)>
// non-option      = <any valid string (including quotations)>
// option-argument-delimiter = <'=', ':' and/or any other valid delimiter>
//

//
// Command line grammar for iterator
//
// command-line = *token
// token = ( prefix option-name delimiter argument )
//       / ( prefix option-name )
//       / ( argument )
//

namespace pfs {
namespace cli {

// Parse flags
enum {
    // Flags applicable to iterator
              
    // Flags applicable to route          
//    , shortopt_stacked             = 0x0002 //!< Combine short options (stacked options) using form -abc (-a -b -c).
//    , combine_short_options        = shortopt_stacked //!< Synonym for shortopt_stacked.
//    , longopt_single_dash          = 0x0004 //!< Allow single dash prefix for long options in form -option.
//    , longopt_double_dash          = 0x0008 //!< Allow double dash prefix for long options in form --option.
//    , longopt_slash                = 0x0010 //!< Allow slash prefix for long options in form /option.
//    , shortopt_slash               = 0x0020 //!< Allow slash prefix for short options in form /o.
//    , double_dash_positional_args  = 0x0040 //!< Allow double dash to separate options and positional arguments.
//    , longopt_arg_eq_separator     = 0x0080 //!< Allow long option with argument in form {-- | /}option=ARG.
//    , longopt_arg_colon_separator  = 0x0100 //!< Allow long option with argument in form {-- | /}option:ARG.
//    , longopt_arg_space_separator  = 0x0200 //!< Allow long option with argument in form {-- | /}option ARG.
//    , shortopt_arg_eq_separator    = 0x0400 //!< Allow short option with argument in form {- | /}o=ARG.
//    , shortopt_arg_colon_separator = 0x0800 //!< Allow short option with argument in form {- | /}o:ARG.
//    , shortopt_arg_space_separator = 0x1000 //!< Allow short option with argument in form {- | /}o ARG.
//    , shortopt_arg_no_separator    = 0x2000 //!< Allow short option with argument in form {- | /}oARG.
};

//template <typename Traits>
//struct token
//{
//    typedef Traits traits_type;
//    typedef typename traits_type::string_type string_type;
//    
//    string_type value;  // full value for item
//    string_type prefix; // '<empty>', '--' , '-' or '/'
//    string_type option;
//    string_type arg;
//};

template <typename Traits>
class iterator
{
protected:
    typedef Traits                               traits_type;
    typedef typename traits_type::string_type    string_type;
    typedef typename string_type::const_pointer  const_pointer;
    typedef typename string_type::const_iterator const_iterator;
    
protected:
    const_pointer * _begin;
    const_pointer * _p;
    const_pointer * _end;
    
protected:
    static error_code parse_prefix (const_iterator begin
        , const_iterator end
        , error_code * ex);

    static error_code parse_option (const_iterator begin
        , const_iterator end
        , error_code * ex);

    static error_code parse_arg (const_iterator begin
        , const_iterator end
        , error_code * ex);
    
    static error_code parse_token (string_type const & token
        , string_type & prefix
        , string_type & option
        , string_type & arg);
    
public:
    iterator (const_pointer * begin, const_pointer * end)
        : _begin(begin)
        , _p(begin)
        , _end(end)
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
    
    string_type operator * ()
    {
        return string_type(*_p);
    }
    
    bool operator == (iterator const & x)
    {
        return _p == x._p;
    }

    bool operator != (iterator const & x)
    {
        return !this->operator == (x);
    }
};

template <typename Traits>
iterator<Traits>::const_iterator iterator<Traits>::parse_prefix (const_iterator begin
        , const_iterator end
        , error_code * ex)
{
    const_iterator it = begin;
    
    while (it != end && traits_type::is_prefix_char(*it))
        ++it;

    return it;
}

template <typename Traits>
iterator<Traits>::const_iterator iterator<Traits>::parse_option (const_iterator begin
        , const_iterator end
        , error_code * ex)
{
    const_iterator it = begin;
    return it;
}

template <typename Traits>
iterator<Traits>::const_iterator iterator<Traits>::parse_arg (const_iterator begin
        , const_iterator end
        , error_code * ex)
{
    const_iterator it = begin;
    return it;
}


template <typename Traits>
error_code iterator<Traits>::parse_token (string_type const & token
        , string_type & prefix
        , string_type & option
        , string_type & arg)
{
    const_iterator begin = token.begin();
    const_iterator end   = token.end();
    const_iterator it    = begin;
    error_code ex        = no_error;
    
    it = parse_prefix(begin, end, & ex);
    
    if (ex != no_error)
        return ex;
    
    prefix = string_type(begin, it);
    begin = it;
    
    if (traits_type::empty(prefix)) {
        it = parse_arg(begin, end, & ex);
    } else {
        it = parse_option(begin, end, & ex);
        it = parse_arg(begin, end, & ex);
    }
        // Parse option
        //
//        while (it != end && !traits_type::is_delim_char(*it)) {
//            if (traits_type::is_quote_char(*it)) {
//                traits_type::char_type q = *it;
//                        
//                while (it != end && *it != q) {
//                    ++it;
//                }
//                
//                // Unterminated quoted string
//                //
//                if (it == end) {
//                    return quote_error;
//                }
//            }
//        }
//    } else {
//        // Parse argument
//        //
//    }
    
    return ex;
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

