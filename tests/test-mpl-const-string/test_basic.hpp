/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_basic.hpp
 * Author: wladt
 *
 * Created on December 27, 2016, 2:02 PM
 */

#ifndef __PFS_TEST_BASIC_HPP__
#define __PFS_TEST_BASIC_HPP__

#include <cstring>
#include <cwchar>
#define PFS_INC_STRING_H 1
#define PFS_INC_WCHAR_H 1
#define PFS_INC_STRING 1
#include <pfs/mpl/string.hpp>

template <typename StringImpl>
StringImpl test_basic_string ();

template <>
char const * test_basic_string<char const *> ()
{
    return "ABCDEF";
}

template <>
wchar_t const * test_basic_string<wchar_t const *> ()
{
    return L"ABCDEF";
}

template <>
std::string test_basic_string<std::string> ()
{
    return std::string("ABCDEF");
}

template <typename StringImpl>
void test_basic ()
{
    ADD_TESTS(16);
    
//    char const * cs1 = "String1";
//    char cs2 [] = "String2";
//    
//    typedef pfs::mpl::string_traits<char const *> cstring1_traits;
//    typedef pfs::mpl::string_traits<char *> cstring2_traits;
//    typedef pfs::mpl::string<char const *> cstring1;
//    typedef pfs::mpl::string<char *> cstring2;
//        
//    cstring1_traits st1;
//    cstring2_traits st2;
//    
//    TEST_OK(cstring1_traits::size(cs1) == std::strlen(cs1));
//    TEST_OK(cstring2_traits::size(cs2) == std::strlen(cs2));
//    cstring1 s1(cs1);
//    cstring2 s2(cs2);
//    
    typedef pfs::mpl::string<StringImpl>    string;
    typedef typename string::const_iterator const_iterator;
    typedef typename string::const_reverse_iterator const_reverse_iterator;
    typedef typename string::value_type     value_type;
    
    string str = test_basic_string<StringImpl>();

    const_iterator it = str.begin();
    const_iterator end = str.end();
    
    TEST_OK(it != end);
    
    TEST_OK(*it++ == value_type('A'));
    TEST_OK(*it++ == value_type('B'));
    TEST_OK(*it++ == value_type('C'));
    TEST_OK(*it++ == value_type('D'));
    TEST_OK(*it++ == value_type('E'));
    TEST_OK(*it++ == value_type('F'));
    
    TEST_OK(it == end);

    const_reverse_iterator itr = str.rbegin();
    const_reverse_iterator endr = str.rend();

    TEST_OK(itr != endr);
    
    TEST_OK(*itr++ == value_type('F'));
    TEST_OK(*itr++ == value_type('E'));
    TEST_OK(*itr++ == value_type('D'));
    TEST_OK(*itr++ == value_type('C'));
    TEST_OK(*itr++ == value_type('B'));
    TEST_OK(*itr++ == value_type('A'));
    
    TEST_OK(itr == endr);
}

#endif /* __PFS_TEST_BASIC_HPP__ */

