/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cmdline.hpp
 * Author: wladt
 *
 * Created on December 20, 2016, 10:05 AM
 */

/*
 * Command line arguments sources:
 * -----------------------------------------------------------------------------
 * 
 * 1. int main (int argc, char ** argv)   // Linux/Windows
 *    argv[0] - program name
 * 
 * 2. int CALLBACK WinMain(               // Windows
 *         _In_ HINSTANCE hInstance,
 *         _In_ HINSTANCE hPrevInstance,
 *         _In_ LPSTR     lpCmdLine,
 *         _In_ int       nCmdShow)
 * 
 *    lpCmdLine - The command line for the application, 
 *                excluding the program name. 
 *                To retrieve the entire command line, 
 *                use the GetCommandLine function.
 * 
 * 3. LPTSTR WINAPI GetCommandLine(void)  // Windows
 *    (GetCommandLineW (Unicode) and GetCommandLineA (ANSI))
 *   
 *    The return value is a pointer to the command-line string for the 
 *    current process (including the program name?).
 *    To convert the command line to an argv style array of strings, 
 *    call function below.
 * 
 * LPWSTR * CommandLineToArgvW (
 *        _In_  LPCWSTR lpCmdLine,
 *        _Out_ int     *pNumArgs);
 * 
 * Returns a pointer to an array of LPWSTR values, similar to argv.
 * 
 * 
 * So, in common case argument list may be obtained as list of code units
 * (Unicode version) or 8-bit characters (ANSI version).
 * 
 * The command line arguments will be called a 'token' to distinguish it 
 * from the argument as an independent element or part of a token in 
 * the composition with the option.
 * 
 * Grammar of the command line:
 * -----------------------------------------------------------------------------
 * 
 * command_line = token *(sp token) [ sp argument-separator *(sp argument) ]
 * sp = 1*<white-space-character>
 * token = option
 *       / argument
 *       / option delimiter argument
 * option              = short-option / long-option
 * argument            = 1*char
 * short-option        = short-option-prefix char
 * long-option         = long-option-prefix 1*char
 * short-option-prefix = "-"  / "/" / char
 * long-option-prefix  = "--" / "/" / 1*char
 * argument-separator  = "--" / 1*char
 * char                = <any-valid-character>
 * 
 */

#ifndef __PFS_CLI_ITERATOR_HPP__
#define __PFS_CLI_ITERATOR_HPP__

#include <pfs/algo/find.hpp>
#include <pfs/cli/traits.hpp>

namespace pfs {
namespace cli {
namespace details {


template <typename Iter>
struct range
{
    typedef Iter iterator;
    
    iterator begin;
    iterator end;
    
    range ()
        : begin(0)
        , end(0)
    {}
    
    range (iterator b, iterator e)
            : begin(b)
            , end(e)
    {}

    range (iterator b, size_t n)
            : begin(b)
            , end(b + n)
    {}
    
    bool empty () const
    {
        return begin == end;
    }
    
    bool operator == (range const & rhs) const
    {
        typename range<Iter>::iterator i1 = begin;
        typename range<Iter>::iterator i2 = rhs.begin;

        while (i1 != end && i2 != rhs.end && *i1++ == *i2++) ;

        return i1 == end && i2 == rhs.end;
    }

    bool operator != (range const & rhs) const
    {
        return ! (*this == rhs);
    }
};

template <typename CharIterator>
struct token
{
    typedef CharIterator char_iterator;
    typedef range<CharIterator> range_type;
    
    range_type prefix;
    range_type option;
    range_type arg;
};

template <typename CharType, typename CharIterator, typename TokenIterator>
class cmdline
{
public:
    typedef TokenIterator       iterator;
    typedef CharType            char_type;
    typedef CharIterator        char_iterator;
    typedef token<CharIterator> token_type;
    typedef range<CharIterator> range_type;

private:
    iterator _begin;
    iterator _end;
    
    char_type  _delimiter_char;
    range_type _longopt_prefix;
    range_type _shortopt_prefix;
    range_type _argument_separator;
    
public:
    cmdline (int n, iterator begin)
        : _begin(begin)
        , _end(begin + n)
    {}

    cmdline (iterator begin, iterator end)
        : _begin(begin)
        , _end(end)
    {}

    void set_delimiter_char (char_type ch)
    {
        _delimiter_char = ch;
    }
    
    void set_shortopt_prefix (typename range_type::iterator begin
        , typename range_type::iterator end)
    {
        _shortopt_prefix = range_type(begin, end);
    }

    void set_shortopt_prefix (typename range_type::iterator begin, size_t n)
    {
        _shortopt_prefix = range_type(begin, n);
    }

    void set_longopt_prefix (typename range_type::iterator begin
        , typename range_type::iterator end)
    {
        _longopt_prefix = range_type(begin, end);
    }

    void set_longopt_prefix (typename range_type::iterator begin, size_t n)
    {
        _longopt_prefix = range_type(begin, n);
    }

    void set_argument_separator (typename range_type::iterator begin
        , typename range_type::iterator end)
    {
        _argument_separator = range_type(begin, end);
    }

    void set_argument_separator (typename range_type::iterator begin, size_t n)
    {
        _argument_separator = range_type(begin, n);
    }
    
    iterator begin () const
    {
        return iterator(_begin + 1);
    }

    iterator end () const
    {
        return iterator(_end);
    }
    
    bool is_argument_separator (token_type const & token) const
    {
        return token.prefix == _argument_separator
                && token.option.empty()
                && token.arg.empty();
    }
    
    token_type split (CharIterator begin, CharIterator end);
};

template <typename CharType, typename CharIterator, typename TokenIterator>
token<CharIterator> cmdline<CharType, CharIterator, TokenIterator>::split (
      CharIterator begin
    , CharIterator end)
{
    token<CharIterator> result;
   
    CharIterator it = begin;
    
    if (pfs::starts_with(it, end, _longopt_prefix.begin, _longopt_prefix.end))
        it += _longopt_prefix.end - _longopt_prefix.begin;
    else if (pfs::starts_with(it, end, _shortopt_prefix.begin, _shortopt_prefix.end))
        it += _shortopt_prefix.end - _shortopt_prefix.begin;
    
    result.prefix = range_type(begin, it);
    begin = it;
    
    if (result.prefix.begin == result.prefix.end) {
        result.option = range_type(begin, begin);
        result.arg = range_type(begin, end);
    } else {
        while (it != end && *it != _delimiter_char)
            ++it;

        result.option = range_type(begin, it);
        
        if (it != end && *it == _delimiter_char)
            ++it;
        
        result.arg = range_type(it, end);
    }
    
    return result;    
}

} // details

}} // pfs::cli::details

#endif /* __PFS_CLI_ITERATOR_HPP__ */

