/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   exception.hpp
 * Author: wladt
 *
 * Created on October 20, 2016, 1:33 PM
 */

#ifndef __PFS_CXX98_EXCEPTION_HPP__
#define __PFS_CXX98_EXCEPTION_HPP__

#include <stdexcept>

namespace pfs {

typedef std::logic_error      logic_error;
typedef std::domain_error     domain_error;
typedef std::invalid_argument invalid_argument;
typedef std::length_error     length_error;
typedef std::out_of_range     out_of_range;
typedef std::runtime_error    runtime_error;
typedef std::range_error      range_error;
typedef std::overflow_error   overflow_error;
typedef std::underflow_error  underflow_error;

} // pfs

#endif /* __PFS_CXX98_EXCEPTION_HPP__ */

