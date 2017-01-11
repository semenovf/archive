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

#ifndef __PFS_CLI_CMDLINE_HPP__
#define __PFS_CLI_CMDLINE_HPP__

#include <pfs/mpl/string.hpp>

namespace pfs {
namespace mpl {
namespace cli {

template <typename StringImpl>
struct token
{
    typedef pfs::mpl::string<StringImpl> string_type;
    
    string_type prefix;
    string_type option;
    string_type arg;
};

template <typename StringImpl>
class token_iterator
{
    typedef pfs::mpl::string<StringImpl> string_type;
    StringImpl * _p;
    
public:
    token_iterator (StringImpl * p)
        : _p(p)
    {}
            
    token_iterator & operator ++ ()
    {
        ++_p;
        return *this;
    }

    token_iterator operator ++ (int)
    {
        token_iterator it(_p);
        ++_p;
        return it;
    }

    token_iterator operator + (int n) const
    {
        return token_iterator(_p + n);
    }
    
    string_type operator * () const
    {
        return string_type(*_p);
    }
    
    bool operator == (token_iterator const & lhs) const
    {
        return _p == lhs._p;
    }

    bool operator != (token_iterator const & lhs) const
    {
        return _p != lhs._p;
    }
};

template <typename StringImpl>
class cmdline
{
public:
    typedef string<StringImpl>                   string_type;
    typedef typename string_type::value_type     char_type;
    typedef typename string_type::const_iterator char_iterator;
    typedef token<StringImpl>                    token_type;
    typedef token_iterator<StringImpl>           iterator;

private:
    iterator _begin;
    iterator _end;
    
    char_type   _delimiter_char;
    string_type _longopt_prefix;
    string_type _shortopt_prefix;
    string_type _argument_separator;
    
public:
    cmdline (int n, StringImpl * begin)
        : _begin(begin)
        , _end(begin + n)
    {}

    cmdline (StringImpl * begin, StringImpl * end)
        : _begin(begin)
        , _end(end)
    {}

    void set_delimiter_char (char_type ch)
    {
        _delimiter_char = ch;
    }
    
    void set_shortopt_prefix (string_type const & s)
    {
        _shortopt_prefix = s;
    }

    void set_longopt_prefix (string_type const & s)
    {
        _longopt_prefix = s;
    }

    void set_argument_separator (string_type const & s)
    {
        _argument_separator = s;
    }
    
    iterator begin () const
    {
        return iterator(_begin + 1);
    }

    iterator end () const
    {
        return iterator(_end);
    }
    
    token_type split (string_type const & t);
};

template <typename StringImpl>
token<StringImpl> cmdline<StringImpl>::split (string_type const & t)
{
    token_type result;
    char_iterator begin = t.begin();
    char_iterator end   = t.end();
    char_iterator it = begin;
    string_type s(it, end);
    
    if (s.starts_with(_longopt_prefix))
        it += _longopt_prefix.size();
    else if (s.starts_with(_shortopt_prefix))
        it += _shortopt_prefix.size();
    
    result.prefix = string_type(begin, it);
    begin = it;
    
    if (result.prefix.begin() == result.prefix.end()) {
        result.option = string_type(begin, begin);
        result.arg = string_type(begin, end);
    } else {
        while (it != end && *it != _delimiter_char)
            ++it;

        result.option = string_type(begin, it);
        
        if (it != end && *it == _delimiter_char)
            ++it;
        
        result.arg = string_type(it, end);
    }
    
    return result;    
}

}}} // pfs::mpl::cli

#endif /* __PFS_CLI_CMDLINE_HPP__ */

