/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   condition_variable.hpp
 * Author: wladt
 *
 * Created on August 16, 2016, 10:15 AM
 */

#ifndef __PFS_BITS_CONDITION_VARIABLE_HPP__
#define __PFS_BITS_CONDITION_VARIABLE_HPP__

#include <pfs.hpp>

#if PFS_OS_POSIX
#   include <pthread.h>
#elif PFS_CC_MSVC
#   error "Need implementation for condition_variable"
#else
#   error "Need implementation for condition_variable"
#endif

namespace pfs {
namespace details {

#if PFS_OS_POSIX

struct condition_variable_data
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int waiters;
    int wakeups;
};

#elif PFS_CC_MSVC

#endif

}}

#endif /* __PFS_BITS_CONDITION_VARIABLE_HPP__ */

