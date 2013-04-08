/**
 * @file   atomic.hpp
 * @author wladt
 * @date   Mar 26, 2013 12:24:30 PM
 *
 * @brief
 */

#ifndef __CWT_ATOMIC_HPP__
#define __CWT_ATOMIC_HPP__

#include <cwt/cwt.h>

#if defined(CWT_CC_MSVC)
#	pragma message("Using MSVC implementation for atomic variables")
#	include <cwt/atomic_msvc.hpp>
#elif defined(CWT_CPP_CXX11)
#	pragma message("Using C++11 implementation for atomic variables")
#	include <cwt/atomic_cxx11.hpp>
#elif defined(CWT_CC_GNUC)
#	pragma message("Using GCC implementation for atomic variables")
#	include <cwt/atomic_gcc.hpp>
#elif defined(CWT_TARGET_CPU) && (defined(CWT_TARGET_CPU_X86) || defined(CWT_TARGET_CPU_x86))
#	pragma message("Using x86 implementation for atomic variables")
#	include <cwt/atomic_x86.hpp>
#else
#	pragma message("Using implementation with locking (worst case) for atomic variables")
#	include <cwt/atomic_mt.hpp>
#endif


CWT_NS_BEGIN

template <typename T>
class atomic_integer
{
public:
    typedef atomic_integer_intrinsics<T> intrinsics;
    typename intrinsics::Type m_value;

    // Non-atomic API
    T load() const { return intrinsics::load(m_value); }
    void store(T newValue) { intrinsics::store(m_value, newValue); }
    bool ref() { return intrinsics::ref(m_value); }
    bool deref() { return intrinsics::deref(m_value); }

    T fetchAndAddRelaxed(T valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed(m_value, valueToAdd);
    }
};

typedef atomic_integer<int> atomic_int;

CWT_NS_END

#endif /* __CWT_ATOMIC_HPP__ */
