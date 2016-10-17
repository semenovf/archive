/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   atomic.hpp
 * Author: wladt
 *
 * Created on October 17, 2016, 10:20 AM
 * 
 * https://gcc.gnu.org/onlinedocs/gcc-4.4.5/gcc/Atomic-Builtins.html
 * 
 * type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
 *      These builtins perform an atomic compare and swap. 
 *      That is, if the current value of *ptr is oldval, then write newval into *ptr. 
 *      Returns the contents of *ptr before the operation. 
 * 
 */

#ifndef __PFS_COMPILER_GCC_ATOMIC_INTRINSICS_HPP__
#define __PFS_COMPILER_GCC_ATOMIC_INTRINSICS_HPP__

namespace pfs {

template <typename T>
inline T atomic_add_fetch (T * pval, T n)
{
    return (T) __sync_add_and_fetch(pval, n);
}

template <typename T>
inline T atomic_sub_fetch (T * pval, T n)
{
    return (T) __sync_sub_and_fetch(pval, n);
}

template <typename T>
inline T atomic_and_fetch (T * pval, T n)
{
    return (T) __sync_and_and_fetch(pval, n);
}

template <typename T>
inline T atomic_or_fetch (T * pval, T n)
{
    return (T) __sync_or_and_fetch(pval, n);
}

template <typename T>
inline T atomic_xor_fetch (T * pval, T n)
{
    return (T) __sync_xor_and_fetch(pval, n);
}

template <typename T>
inline T atomic_fetch_add (T * pval, T n)
{
    return (T) __sync_fetch_and_add(pval, n);
}

template <typename T>
inline T atomic_fetch_sub (T * pval, T n)
{
    return (T) __sync_fetch_and_sub(pval, n);
}

template <typename T>
inline T atomic_fetch_and (T * pval, T n)
{
    return (T) __sync_fetch_and_and(pval, n);
}

template <typename T>
inline T atomic_fetch_or (T * pval, T n)
{
    return (T) __sync_fetch_and_or(pval, n);
}

template <typename T>
inline T atomic_fetch_xor (T * pval, T n)
{
    return (T) __sync_fetch_and_xor(pval, n);
}

template <typename T>
inline T atomic_exchange (T * pval, T nv)
{
    T result = *pval;
    while (! __sync_bool_compare_and_swap(pval, result, nv)) {
        result = *pval; 
    }
    return result;
}

template <typename T>
inline T atomic_load (T * pval)
{
    return atomic_add_fetch(pval, 0);
}

template <typename T>
inline void atomic_store (T * pval, T nv)
{
    atomic_exchange(pval, nv);
}

} // pfs

#endif /* __PFS_COMPILER_GCC_ATOMIC_INTRINSICS_HPP__ */

