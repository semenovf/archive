/**
 * @file   atomic_mt.hpp
 * @author wladt
 * @date   Mar 27, 2013 11:45:49 AM
 *
 * @brief
 */

#ifndef __CWT_ATOMIC_MT_HPP__
#define __CWT_ATOMIC_MT_HPP__

#include <cwt/cwt.h>
#include <cwt/mt.hpp>

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
        // implement fetchAndAdd on top of testAndSet
        for(;;) {
            register T tmp = load(value);
            if (testAndSetRelaxed(value, tmp, T(tmp + valueToAdd)))
                return tmp;
        }
    }

    static bool testAndSetRelaxed(int &value, int expectedValue, int newValue)
    {
        bool rv = false;
        AutoLock<> locker(&g_mutex);
        if (value == expectedValue) {
            value = newValue;
            rv = true;
        }
        return rv;
    }

    static CWT_DEFAULT_MT_POLICY g_mutex;
};

CWT_NS_END

#endif /* __CWT_ATOMIC_MT_HPP__ */
