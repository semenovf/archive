/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_compare.hpp
 * Author: wladt
 *
 * Created on January 10, 2017, 10:30 AM
 */

#ifndef __PFS_TEST_COMPARE_HPP__
#define __PFS_TEST_COMPARE_HPP__

template <typename StringImpl>
StringImpl test_compare_string (int i);

template <>
char const * test_compare_string<char const *> (int i)
{
    static char const * s[] = {
          "ABCDEF"
        , "ABCDE"
        , "BCDEF"
    };
    
    return s[i];
}

template <>
wchar_t const * test_compare_string<wchar_t const *> (int i)
{
    static wchar_t const * s[] = {
          L"ABCDEF"
        , L"ABCDE"
        , L"BCDEF"
    };
    
    return s[i];
}

template <>
std::string test_compare_string<std::string> (int i)
{
    static std::string s[] = {
          std::string("ABCDEF")
        , std::string("ABCDE")
        , std::string("BCDEF")
    };
    
    return s[i];
}

template <>
std::wstring test_compare_string<std::wstring> (int i)
{
    static std::wstring s[] = {
          std::wstring(L"ABCDEF")
        , std::wstring(L"ABCDE")
        , std::wstring(L"BCDEF")
    };
    
    return s[i];
}

template <typename StringImpl>
void test_compare ()
{
    ADD_TESTS(3);
    
    typedef pfs::mpl::string<StringImpl> string;
    string orig = test_compare_string<StringImpl>(0);
    string same = test_compare_string<StringImpl>(0);
    string head = test_compare_string<StringImpl>(1);
    string tail = test_compare_string<StringImpl>(2);
    
//    orig.compare(0, 0, dynamic_cast<pfs::mpl::details::basic_string<StringImpl> const &>(same), 0, 0) == 0;
//    orig.compare(0, 0, same, 0, 0) == 0;
//    orig.compare(dynamic_cast<pfs::mpl::details::basic_string<StringImpl> const &>(same)) == 0;
//    orig.compare(same) == 0;
//    orig.starts_with(head);
    
    TEST_OK(orig.compare(same) == 0);
    TEST_OK(orig.starts_with(head));
    TEST_OK(orig.ends_with(tail));
}

#endif /* __PFS_TEST_COMPARE_HPP__ */

