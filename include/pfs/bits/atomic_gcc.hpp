/**
 * @file   atomic_gcc.hpp
 * @author wladt
 * @date   Mar 27, 2013 12:24:30 PM
 *
 * @brief
 * 
 * https://gcc.gnu.org/onlinedocs/gcc-4.4.5/gcc/Atomic-Builtins.html
 * 
 * type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
 *      These builtins perform an atomic compare and swap. 
 *      That is, if the current value of *ptr is oldval, then write newval into *ptr. 
 *      Returns the contents of *ptr before the operation. 
 * 
 */

#ifndef __PFS_BITS_ATOMIC_GCC_HPP__
#define __PFS_BITS_ATOMIC_GCC_HPP__

#include <pfs.hpp>

namespace pfs {

template<typename T>
struct atomic_integer_intrinsics
{
	typedef T Type;

	static inline T load (T const & value)
	{
        return __sync_fetch_and_add(const_cast<T *>(& value), 0);
	}

    static inline void store (T & value, T newValue)
    {
        __sync_val_compare_and_swap(& value, value, newValue);
    }

    static inline bool ref (T & value)
    {
        return fetchAndAddRelaxed(value, 1) != T(-1);
    }

    static inline bool deref (T & value)
    {
         return fetchAndAddRelaxed(value, -1) != T(1);
    }

    static inline T fetchAndAddRelaxed (T & value, T valueToAdd)
    {
        return __sync_fetch_and_add(& value, valueToAdd);
    }

    static inline T compareAndSwap (T * ptr, T oldValue, T newValue)
    {
    	return __sync_val_compare_and_swap(ptr, oldValue, newValue);
    }

/* FIXME add support of this
    type __sync_fetch_and_sub (type *ptr, type value, ...)
    type __sync_fetch_and_or (type *ptr, type value, ...)
    type __sync_fetch_and_and (type *ptr, type value, ...)
    type __sync_fetch_and_xor (type *ptr, type value, ...)
*/
};

} // pfs

#endif /* __PFS_BITS_ATOMIC_GCC_HPP__ */
