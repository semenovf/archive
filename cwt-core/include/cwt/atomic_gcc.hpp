/**
 * @file   atomic_gcc.hpp
 * @author wladt
 * @date   Mar 27, 2013 12:24:30 PM
 *
 * @brief
 */

#ifndef __CWT_ATOMIC_GCC_HPP__
#define __CWT_ATOMIC_GCC_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

template<typename T>
struct atomic_integer_intrinsics
{
	typedef int Type;

	static inline T load(const T &value)
	{
		return value;
	}

    static inline void store(T &value, T newValue)
    {
        value = newValue;
    }

    static inline bool ref(T &value)
    {
        return fetchAndAddRelaxed(value, 1) != T(-1);
    }

    static inline bool deref(T &value)
    {
         return fetchAndAddRelaxed(value, -1) != T(1);
    }

    static inline T fetchAndAddRelaxed(T &value, T valueToAdd)
    {
        return __sync_fetch_and_add(&value, valueToAdd);
    }

};

CWT_NS_END

#endif /* __CWT_ATOMIC_GCC_HPP__ */
