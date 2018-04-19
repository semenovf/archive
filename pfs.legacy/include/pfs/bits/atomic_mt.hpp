/**
 * @file   atomic_mt.hpp
 * @author wladt
 * @date   Mar 27, 2013 11:45:49 AM
 *
 * @brief
 */

#ifndef __PFS_BITS_ATOMIC_MT_HPP__
#define __PFS_BITS_ATOMIC_MT_HPP__

#include <pfs/mt.hpp>

namespace pfs {

template<typename T>
struct atomic_integer_intrinsics
{
	typedef int Type;

	static inline T load (const T & value)
	{
		return value;
	}

    static inline void store (T & value, T newValue)
    {
        value = newValue;
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
        // implement fetchAndAdd on top of testAndSet
        for(;;) {
            register T tmp = load(value);
            if (testAndSetRelaxed(value, tmp, T(tmp + valueToAdd)))
                return tmp;
        }
        return T(); // never reached
    }

    static bool testAndSetRelaxed (int & value, int expectedValue, int newValue)
    {
        bool rv = false;
        auto_lock<> locker(& __mutex);
        if (value == expectedValue) {
            value = newValue;
            rv = true;
        }
        return rv;
    }

    static inline T compareAndSwap (T * ptr, T oldValue, T newValue)
    {
    	auto_lock<> locker(& __mutex);
    	 T r = *ptr;
    	 if (*ptr == oldValue)
    		 *ptr = newValue;
    	 return r;
    }

    static PFS_DEFAULT_MT_POLICY __mutex;
};

} // pfs

#endif /* __BITS_PFS_ATOMIC_MT_HPP__ */
