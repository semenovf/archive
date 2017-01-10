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

template <>
std::wstring test_basic_string<std::wstring> ()
{
    return std::wstring(L"ABCDEF");
}

template <typename StringImpl>
void test_basic ()
{
    ADD_TESTS(16);
    
    typedef pfs::mpl::string<StringImpl>    string;
    typedef typename string::const_iterator const_iterator;
    typedef typename string::const_reverse_iterator const_reverse_iterator;
    typedef typename string::value_type     value_type;
    
    string str = test_basic_string<StringImpl>();

    const_iterator it = str.cbegin();
    const_iterator end = str.cend();
    
    TEST_OK(it != end);
    
    TEST_OK(*it++ == value_type('A'));
    TEST_OK(*it++ == value_type('B'));
    TEST_OK(*it++ == value_type('C'));
    TEST_OK(*it++ == value_type('D'));
    TEST_OK(*it++ == value_type('E'));
    TEST_OK(*it++ == value_type('F'));
    
    TEST_OK(it == end);

    const_reverse_iterator itr = str.crbegin();
    const_reverse_iterator endr = str.crend();

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

