/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pair.hpp
 * Author: wladt
 *
 * Created on January 17, 2017, 2:13 PM
 */

#ifndef __PFS_CXX98_UTILITY_HPP__
#define __PFS_CXX98_UTILITY_HPP__

#include <algorithm>

namespace pfs {

template <typename T>
inline void swap (T & a, T & b)
{
	std::swap(a, b);
}

template <typename T1, typename T2>
struct pair
{
    typedef T1 first_type;    /// @c first_type is the first bound type
    typedef T2 second_type;   /// @c second_type is the second bound type

    T1 first;                 /// @c first is a copy of the first object
    T2 second;                /// @c second is a copy of the second object

    pair ()
        : first()
        , second() 
    {}

    pair (T1 const & a, T2 const & b)
        : first(a)
        , second(b) 
    {}

    template <typename U1, typename U2>
    pair (pair<U1, U2> const & p)
        : first(p.first)
        , second(p.second) 
    {}
    
    pair & operator = (pair const & p)
    {
        first = p.first;
        second = p.second;
        return *this;
    }

    template <typename U1, typename U2>
	pair & operator = (pair<U1, U2> const & p)
	{
        first = p.first;
        second = p.second;
        return *this;
	}

    void swap (pair & p)
    {
        pfs::swap(first, p.first);
        pfs::swap(second, p.second);
    }
};

} // pfs

#endif /* __PFS_CXX98_UTILITY_HPP__ */
