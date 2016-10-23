/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ratio.hpp
 * Author: wladt
 *
 * Created on July 11, 2016, 6:55 PM
 */

#ifndef __PFS_CXX11_RATIO_HPP__
#define __PFS_CXX11_RATIO_HPP__

#include <chrono>

namespace pfs {

template <intmax_t Num, intmax_t Denom = 1>
using ratio = std::ratio<Num, Denom>;

} // pfs

#endif /* __PFS_CXX11_RATIO_HPP__ */

