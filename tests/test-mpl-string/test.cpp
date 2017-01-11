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
#include <pfs/mpl/stdc/string.hpp>
#include <pfs/mpl/stdcxx/string.hpp>

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


#include "test_basic.hpp"
#include "test_compare.hpp"

int main (int argc, char *argv[])
{
 	PFS_UNUSED(argc);
	PFS_UNUSED(argv);
    
	BEGIN_TESTS(0);
    
    test_basic<char *>();
    test_basic<char const *>();
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
