/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief Testing pfs::mpl::string.hpp.
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

#include <cstring>
#include <cwchar>
#include <iostream>
#include <sstream>
#include <pfs/mpl/impl/stdc/string.hpp>
#include <pfs/mpl/impl/stdcxx/string.hpp>

// Enabled by `qt_enable`
#ifdef QT_CORE_LIB
#   include <pfs/mpl/impl/qt/string.hpp>
#endif

template <typename StringImpl>
char const * stringify_string_impl ();

template <>
inline char const * stringify_string_impl<char const *> ()
{ return "char const *"; }

template <>
inline char const * stringify_string_impl<char *> ()
{ return "char *"; }

template <>
inline char const * stringify_string_impl<wchar_t const *> ()
{ return "wchar_t const *"; }

template <>
inline char const * stringify_string_impl<wchar_t *> ()
{ return "wchar_t *"; }

template <>
inline char const * stringify_string_impl<std::string> ()
{ return "std::string"; }

template <>
inline char const * stringify_string_impl<std::wstring> ()
{ return "std::wstring"; }


#ifdef QT_CORE_LIB

template <>
inline char const * stringify_string_impl<QString> ()
{ return "QString"; }

#endif

template <typename StringImpl>
void test_description (char const * title)
{
    
    std::cout << "=== "
            << title << ": string implemented using <" 
            << stringify_string_impl<StringImpl>() << "> ==="
            << std::endl;
}

#define STR_ORIG    0
#define STR_SAME    1
#define STR_LESS    2
#define STR_GREATER 3
#define STR_HEAD    2
#define STR_TAIL    3
#define STR_MID     4
#define STR_DIFF    4

template <typename StringImpl>
StringImpl string_samples (int i);

template <>
char const * string_samples<char const *> (int i)
{
    static char const * s[] = {
          "ABCDEF"
        , "ABCDEF"
        , "ABCDE"
        , "BCDEF"
        , "BCDE"
    };
    
    return s[i];
}

template <>
char * string_samples<char *> (int i)
{
    static char s[][10] = {
          "ABCDEF"
        , "ABCDEF"
        , "ABCDE"
        , "BCDEF"
        , "BCDE"
    };
    
    return & s[i][0];
}

template <>
wchar_t const * string_samples<wchar_t const *> (int i)
{
    static wchar_t const * s[] = {
          L"ABCDEF"
        , L"ABCDEF"
        , L"ABCDE"
        , L"BCDEF"
        , L"BCDE"
    };
    
    return s[i];
}

template <>
wchar_t * string_samples<wchar_t *> (int i)
{
    static wchar_t s[][10] = {
          L"ABCDEF"
        , L"ABCDEF"
        , L"ABCDE"
        , L"BCDEF"
        , L"BCDE"
    };
    
    return & s[i][0];
}

template <>
std::string string_samples<std::string> (int i)
{
    static std::string s[] = {
          std::string("ABCDEF")
        , std::string("ABCDEF")
        , std::string("ABCDE")
        , std::string("BCDEF")
        , std::string("BCDE")
    };
    
    return s[i];
}

template <>
std::wstring string_samples<std::wstring> (int i)
{
    static std::wstring s[] = {
          std::wstring(L"ABCDEF")
        , std::wstring(L"ABCDEF")
        , std::wstring(L"ABCDE")
        , std::wstring(L"BCDEF")
        , std::wstring(L"BCDE")
    };
    
    return s[i];
}

#ifdef QT_CORE_LIB
template <>
QString string_samples<QString> (int i)
{
    static QString s[] = {
          QString("ABCDEF")
        , QString("ABCDEF")
        , QString("ABCDE")
        , QString("BCDEF")
        , QString("BCDE")
    };
    
    return s[i];
}
#endif

#include "test_basic.hpp"
#include "test_compare.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_basic<char const *>();
    test_basic<char *>();
    test_basic<wchar_t const *>();
    test_basic<wchar_t *>();
    test_basic<std::string>();
    test_basic<std::wstring>();
    
#ifdef QT_CORE_LIB
    test_basic<QString>();
#endif    

    test_compare<char const *>();
    test_compare<char *>();
    test_compare<wchar_t const *>();
    test_compare<wchar_t *>();
    test_compare<std::string>();
    test_compare<std::wstring>();

#ifdef QT_CORE_LIB
    test_compare<QString>();
#endif

	return END_TESTS;
}
