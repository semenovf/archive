/**
 * @file   atomic.hpp
 * @author wladt
 * @date   Mar 26, 2013 12:24:30 PM
 *
 * @brief
 */

#ifndef __CWT_ATOMIC_HPP__
#define __CWT_ATOMIC_HPP__

#include <cwt/cwt.hpp>

#if defined(CWT_CC_MSVC)
//#	pragma message("Using MSVC implementation for atomic variables")
#	include <cwt/atomic_msvc.hpp>
#	define CWT_HAVE_ATOMIC_MSVC 1
#elif defined(CWT_CPP_CXX11)
//#	pragma message("Using C++11 implementation for atomic variables")
#	include <cwt/atomic_cxx11.hpp>
#	define CWT_HAVE_ATOMIC_CX11 1
#elif defined(CWT_CC_GNUC)
//#	pragma message("Using GCC implementation for atomic variables")
#	include <cwt/atomic_gcc.hpp>
#	define CWT_HAVE_ATOMIC_GCC 1
#elif defined(CWT_TARGET_CPU) && (defined(CWT_TARGET_CPU_X86) || defined(CWT_TARGET_CPU_x86))
//#	pragma message("Using x86 implementation for atomic variables")
#	include <cwt/atomic_x86.hpp>
#	define CWT_HAVE_ATOMIC_X86 1
#else
//#	pragma message("Using implementation with locking (worst case) for atomic variables")
#	include <cwt/atomic_mt.hpp>
#	define CWT_HAVE_ATOMIC_MT 1
#endif

CWT_NS_BEGIN

template <typename T>
class atomic_integer
{
	typedef T Type;
public:
    typedef atomic_integer_intrinsics<T> intrinsics;
    typename intrinsics::Type m_value;

    // Non-atomic API
    Type load() const { return intrinsics::load(m_value); }
    void store(Type newValue) { intrinsics::store((Type &)m_value, newValue); }
    bool ref() { return intrinsics::ref((Type &)m_value); }
    bool deref() { return intrinsics::deref((Type &)m_value); }

    Type fetchAndAddRelaxed(Type valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed(m_value, valueToAdd);
    }
};

typedef atomic_integer<int> atomic_int;

template <typename T>
class atomic_pointer
{
	typedef T * Type;
public:
    typedef atomic_integer_intrinsics<Type> intrinsics;
    typename intrinsics::Type m_value;

    // Non-atomic API
    Type load() const { return intrinsics::load(m_value); }
    void store(Type newValue) { intrinsics::store((Type &)m_value, newValue); }
    bool ref() { return intrinsics::ref((Type &)m_value); }
    bool deref() { return intrinsics::deref((Type &)m_value); }

    Type fetchAndAddRelaxed(Type valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed(m_value, valueToAdd);
    }
};

CWT_NS_END

#endif /* __CWT_ATOMIC_HPP__ */
