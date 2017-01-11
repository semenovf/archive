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

#include <iostream>
#include <sstream>

template <typename StringImpl>
StringImpl test_compare_string (int i);

template <>
char const * test_compare_string<char const *> (int i)
{
    static char const * s[] = {
          "ABCDEF"
        , "ABCDEF"
        , "ABCDE"
        , "BCDEF"
    };
    
    return s[i];
}

template <>
char * test_compare_string<char *> (int i)
{
    static char s[][10] = {
          "ABCDEF"
        , "ABCDEF"
        , "ABCDE"
        , "BCDEF"
    };
    
    return & s[i][0];
}

template <>
wchar_t const * test_compare_string<wchar_t const *> (int i)
{
    static wchar_t const * s[] = {
          L"ABCDEF"
        , L"ABCDEF"
        , L"ABCDE"
        , L"BCDEF"
    };
    
    return s[i];
}

template <>
wchar_t * test_compare_string<wchar_t *> (int i)
{
    static wchar_t s[][10] = {
          L"ABCDEF"
        , L"ABCDEF"
        , L"ABCDE"
        , L"BCDEF"
    };
    
    return & s[i][0];
}

template <>
std::string test_compare_string<std::string> (int i)
{
    static std::string s[] = {
          std::string("ABCDEF")
        , std::string("ABCDEF")
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
        , std::wstring(L"ABCDEF")
        , std::wstring(L"ABCDE")
        , std::wstring(L"BCDEF")
    };
    
    return s[i];
}

#ifdef QT_CORE_LIB
template <>
QString test_compare_string<QString> (int i)
{
    static QString s[] = {
          QString("ABCDEF")
        , QString("ABCDEF")
        , QString("ABCDE")
        , QString("BCDEF")
    };
    
    return s[i];
}
#endif

template <typename StringImpl>
void test_compare_basic ()
{
    ADD_TESTS(3);
    
    typedef pfs::mpl::string<StringImpl> string;
    string orig = test_compare_string<StringImpl>(0);
    string same = test_compare_string<StringImpl>(1);
    string head = test_compare_string<StringImpl>(2);
    string tail = test_compare_string<StringImpl>(3);
    
    TEST_OK(orig.compare(same) == 0);
    TEST_OK(orig.starts_with(head));
    TEST_OK(orig.ends_with(tail));
}

struct test_compare_empty_data
{
    size_t pos1;
    size_t count1;
    size_t pos2;
    size_t count2;
    int    result;
};

template <typename StringImpl>
void test_compare_empty ()
{
    ADD_TESTS(1);

    typedef pfs::mpl::string<StringImpl> string;
    string s1 = test_compare_string<StringImpl>(0);
    string s2 = test_compare_string<StringImpl>(1);
    
    typename string::size_type n1 = s1.size();
    typename string::size_type n2 = s2.size();
    
    test_compare_empty_data t[] = {
          {  0, 0,  0,  0, 0 }
        , {  0, 0, n2,  0, 0 }
        , {  0, 0, n2,  1, 0 }
        , { n1, 0,  0,  0, 0 }
        , { n1, 1,  0,  0, 0 }
        , { n1, 0, n2,  0, 0 }
        , { n1, 0, n2,  1, 0 }
        , { n1, 1, n2,  0, 0 }
        , { n1, 1, n2,  1, 0 }
    };
    
    size_t count = sizeof(t)/sizeof(t[0]);
    size_t i = 0;
    
    for (; i < count; ++i) {
        if (s1.compare(t[i].pos1, t[i].count1, s2, t[i].pos2, t[i].count2) != t[i].result) {
            std::cerr << "Compare strings implemented using <" 
                    << stringify_string_impl<StringImpl>()
                    << ">: failed with index: " << i << std::endl;
            break;
        }
    }

    std::ostringstream ss;
    ss << "Compare strings implemented using <" 
            << stringify_string_impl<StringImpl>() 
            << ">";
    
    TEST_OK2(i == count, ss.str().c_str());
}

template <typename StringImpl>
void test_compare ()
{
    test_compare_basic<StringImpl>();
    test_compare_empty<StringImpl>();
}

#endif /* __PFS_TEST_COMPARE_HPP__ */

