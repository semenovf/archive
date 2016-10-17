/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   atomic.hpp
 * Author: wladt
 *
 * Created on October 17, 2016, 9:13 AM
 */

#ifndef __PFS_CXX11_ATOMIC_HPP__
#define __PFS_CXX11_ATOMIC_HPP__

#include <atomic>

namespace pfs {

template <typename T> 
using atomic = std::atomic<T>;

}

#endif /* __PFS_CXX11_ATOMIC_HPP__ */

