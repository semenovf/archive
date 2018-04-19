/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   algorithm.hpp
 * Author: wladt
 *
 * Created on January 18, 2017, 1:18 PM
 */

#ifndef __PFS_CXX98_ALGOBASE_HPP__
#define __PFS_CXX98_ALGOBASE_HPP__

#include <algorithm>
#include <pfs/cxxlang.hpp>

namespace pfs {

template <typename T>
inline pfs_constexpr const T & min (const T & a, const T & b)
{
    return std::min(a, b);
}

template <typename T, typename Compare>
inline pfs_constexpr const T & min (T const & a, T const & b, Compare comp)
{
    return std::min(a, b, comp);
}

template <typename T>
inline pfs_constexpr const T & max (const T & a, const T & b)
{
    return std::max(a, b);
}

template <typename T, typename Compare>
inline pfs_constexpr const T & max (T const & a, T const & b, Compare comp)
{
    return std::max(a, b, comp);
}

}

#endif /* __PFS_CXX98_ALGORITHM_HPP__ */
