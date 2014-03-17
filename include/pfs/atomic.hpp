/**
 * @file   atomic.hpp
 * @author wladt
 * @date   Mar 26, 2013 12:24:30 PM
 *
 * @brief
 */

#ifndef __PFS_ATOMIC_HPP__
#define __PFS_ATOMIC_HPP__

#include <pfs.hpp>

#if defined(PFS_CC_MSVC)
//#	pragma message("Using MSVC implementation for atomic variables")
#	include <pfs/atomic_msvc.hpp>
#	define HAVE_ATOMIC_MSVC 1
#elif defined(PFS_CPP_CXX11)
//#	pragma message("Using C++11 implementation for atomic variables")
#	include <pfs/atomic_cxx11.hpp>
#	define HAVE_ATOMIC_CX11 1
#elif defined(PFS_CC_GNUC)
//#	pragma message("Using GCC implementation for atomic variables")
#	include <pfs/atomic_gcc.hpp>
#	define HAVE_ATOMIC_GCC 1
#elif defined(PFS_TARGET_CPU) && (defined(PFS_TARGET_CPU_X86) || defined(PFS_TARGET_CPU_x86))
//#	pragma message("Using x86 implementation for atomic variables")
#	include <pfs/atomic_x86.hpp>
#	define HAVE_ATOMIC_X86 1
#else
//#	pragma message("Using implementation with locking (worst case) for atomic variables")
#	include <pfs/atomic_mt.hpp>
#	define HAVE_ATOMIC_MT 1
#endif

namespace pfs {

template <typename T>
class atomic_integer
{
	typedef T Type;
public:
    typedef atomic_integer_intrinsics<T> intrinsics;
    typename intrinsics::Type _value;

    atomic_integer () {}
    atomic_integer (Type value) { store(value); }

    // Non-atomic API
    Type load () const { return intrinsics::load(_value); }
    void store (Type newValue) { intrinsics::store((Type &)_value, newValue); }
    bool ref () { return intrinsics::ref((Type &)_value); }
    bool deref () { return intrinsics::deref((Type &)_value); }

    Type fetchAndAddRelaxed (Type valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed(_value, valueToAdd);
    }

    Type compareAndSwap (Type oldValue, Type newValue)
    {
    	return intrinsics::compareAndSwap(& _value, oldValue, newValue);
    }

//    void swap (atomic_integer<Type> & other)
//    {
//    	other.store(compareAndSwap(_value, other.load()));
//    }
};

typedef atomic_integer<int> atomic_int;

template <typename T>
class atomic_pointer
{
	typedef T * Type;
public:
    typedef atomic_integer_intrinsics<Type> intrinsics;
    typename intrinsics::Type _value;

    atomic_pointer () {}
    atomic_pointer (Type value) { store(value); }

    // Non-atomic API
    Type load  () const { return intrinsics::load(_value); }
    void store (Type newValue) { intrinsics::store((Type &)_value, newValue); }
    bool ref   () { return intrinsics::ref((Type &)_value); }
    bool deref () { return intrinsics::deref((Type &)_value); }

    Type fetchAndAddRelaxed(Type valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed(_value, valueToAdd);
    }

    Type compareAndSwap (Type oldValue, Type newValue)
    {
    	return intrinsics::compareAndSwap(& _value, oldValue, newValue);
    }

};

} // pfs

#endif /* __PFS_ATOMIC_HPP__ */
