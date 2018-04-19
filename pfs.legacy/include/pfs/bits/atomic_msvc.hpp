/**
 * @file   atomic_msvc.hpp
 * @author wladt
 * @date   Mar 27, 2013 4:07:05 PM
 *
 * @brief
 * 
 * https://msdn.microsoft.com/ru-ru/library/2ddez55b.aspx
 */

#ifndef __PFS_BITS_ATOMIC_MSVC_HPP__
#define __PFS_BITS_ATOMIC_MSVC_HPP__

#include <pfs.hpp>

extern "C" {
    long _InterlockedIncrement (long volatile *);
    long _InterlockedDecrement (long volatile *);
    long _InterlockedExchangeAdd (long volatile *, long);
    long _InterlockedCompareExchange (long volatile * ptr, long Exchange, long Comparand);

#ifdef HAVE_INT64
    __int64 _InterlockedExchangeAdd64 (__int64 volatile *, __int64);
#endif
}

namespace pfs {

template <typename T>
struct atomic_integer_intrinsics
{
	typedef long Type;

	static inline T load (const T &value)
	{
		return value;
	}

    static inline void store (T & value, T newValue)
    {
        value = (T)newValue;
    }

    static inline bool ref (T & value)
    {
    	return _InterlockedIncrement((long volatile *)& value) != 0;
    }

    static inline bool deref (T & value)
    {
    	return _InterlockedDecrement((long volatile *)& value) != 0;
    }

    static inline T fetchAndAddRelaxed (T & value, T valueToAdd)
    {
    	return _InterlockedExchangeAdd(& value, valueToAdd);
    }

    static inline T compareAndSwap (T * ptr, T oldValue, T newValue)
    {
    	return _InterlockedCompareExchange ((long volatile *)ptr, newValue, oldValue);
    }
};

} // pfs

#endif /* __PFS_BITS_ATOMIC_MSVC_HPP__ */
