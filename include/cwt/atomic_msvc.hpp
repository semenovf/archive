/**
 * @file   atomic_msvc.hpp
 * @author wladt
 * @date   Mar 27, 2013 4:07:05 PM
 *
 * @brief
 */

#ifndef __CWT_ATOMIC_MSVC_HPP__
#define __CWT_ATOMIC_MSVC_HPP__

#include <cwt/cwt.h>

extern "C" {
    long _InterlockedIncrement(long volatile *);
    long _InterlockedDecrement(long volatile *);
    long _InterlockedExchangeAdd(long volatile *, long);

#ifdef CWT_HAS_INT64
    __int64 _InterlockedExchangeAdd64(__int64 volatile *, __int64);
#endif
}

CWT_NS_BEGIN

template <typename T>
struct atomic_integer_intrinsics
{
	typedef long Type;

	static inline T load(const T &value)
	{
		return value;
	}

    static inline void store(T &value, T newValue)
    {
        value = (T)newValue;
    }

    static inline bool ref(T &value)
    {
    	return _InterlockedIncrement((long volatile *)&value) != 0;
    }

    static inline bool deref(T &value)
    {
    	return _InterlockedDecrement((long volatile *)&value) != 0;
    }

    static inline T fetchAndAddRelaxed(T &value, T valueToAdd)
    {
    	return _InterlockedExchangeAdd(&value, valueToAdd);
    }
};

CWT_NS_END

#endif /* __CWT_ATOMIC_MSVC_HPP__ */
