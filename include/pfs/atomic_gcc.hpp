/**
 * @file   atomic_gcc.hpp
 * @author wladt
 * @date   Mar 27, 2013 12:24:30 PM
 *
 * @brief
 */

#ifndef __PFS_ATOMIC_GCC_HPP__
#define __PFS_ATOMIC_GCC_HPP__

#include <pfs.hpp>

namespace pfs {

template<typename T>
struct atomic_integer_intrinsics
{
	typedef int Type;

	static inline T load (const T &value)
	{
		return value;
	}

    static inline void store(T &value, T newValue)
    {
        value = newValue;
    }

    static inline bool ref (T & value)
    {
        return fetchAndAddRelaxed(value, 1) != T(-1);
    }

    static inline bool deref(T & value)
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
};

} // pfs

#endif /* __PFS_ATOMIC_GCC_HPP__ */
