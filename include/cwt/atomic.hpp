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

    // static check that this is a valid integer
    //typedef char PermittedIntegerType[QAtomicIntegerTraits<T>::IsInteger ? 1 : -1];

    // Non-atomic API
    T load() const { return intrinsics::load(m_value); }
    void store(T newValue) { intrinsics::store(m_value, newValue); }
    bool ref() { return intrinsics::ref(m_value); }
    bool deref() { return intrinsics::deref(m_value); }

    T fetchAndAddRelaxed(T valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed(m_value, valueToAdd);
    }

    // Atomic API, implemented in qatomic_XXX.h
//    T loadAcquire() const Q_DECL_NOTHROW { return Ops::loadAcquire(_q_value); }
//    void storeRelease(T newValue) Q_DECL_NOTHROW { Ops::storeRelease(_q_value, newValue); }

//    static Q_DECL_CONSTEXPR bool isReferenceCountingNative() Q_DECL_NOTHROW { return Ops::isReferenceCountingNative(); }
//    static Q_DECL_CONSTEXPR bool isReferenceCountingWaitFree() Q_DECL_NOTHROW { return Ops::isReferenceCountingWaitFree(); }

//    static Q_DECL_CONSTEXPR bool isTestAndSetNative() Q_DECL_NOTHROW { return Ops::isTestAndSetNative(); }
//    static Q_DECL_CONSTEXPR bool isTestAndSetWaitFree() Q_DECL_NOTHROW { return Ops::isTestAndSetWaitFree(); }

//    bool testAndSetRelaxed(T expectedValue, T newValue) Q_DECL_NOTHROW
//    { return Ops::testAndSetRelaxed(_q_value, expectedValue, newValue); }
//    bool testAndSetAcquire(T expectedValue, T newValue) Q_DECL_NOTHROW
//    { return Ops::testAndSetAcquire(_q_value, expectedValue, newValue); }
//    bool testAndSetRelease(T expectedValue, T newValue) Q_DECL_NOTHROW
//    { return Ops::testAndSetRelease(_q_value, expectedValue, newValue); }
//    bool testAndSetOrdered(T expectedValue, T newValue) Q_DECL_NOTHROW
//    { return Ops::testAndSetOrdered(_q_value, expectedValue, newValue); }

//    static Q_DECL_CONSTEXPR bool isFetchAndStoreNative() Q_DECL_NOTHROW { return Ops::isFetchAndStoreNative(); }
//    static Q_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() Q_DECL_NOTHROW { return Ops::isFetchAndStoreWaitFree(); }

//    T fetchAndStoreRelaxed(T newValue) Q_DECL_NOTHROW
//    { return Ops::fetchAndStoreRelaxed(_q_value, newValue); }
//    T fetchAndStoreAcquire(T newValue) Q_DECL_NOTHROW
//    { return Ops::fetchAndStoreAcquire(_q_value, newValue); }
//    T fetchAndStoreRelease(T newValue) Q_DECL_NOTHROW
//    { return Ops::fetchAndStoreRelease(_q_value, newValue); }
//    T fetchAndStoreOrdered(T newValue) Q_DECL_NOTHROW
//    { return Ops::fetchAndStoreOrdered(_q_value, newValue); }

//    static Q_DECL_CONSTEXPR bool isFetchAndAddNative() Q_DECL_NOTHROW { return Ops::isFetchAndAddNative(); }
//    static Q_DECL_CONSTEXPR bool isFetchAndAddWaitFree() Q_DECL_NOTHROW { return Ops::isFetchAndAddWaitFree(); }

//    T fetchAndAddAcquire(T valueToAdd) Q_DECL_NOTHROW
//    { return Ops::fetchAndAddAcquire(_q_value, valueToAdd); }
//    T fetchAndAddRelease(T valueToAdd) Q_DECL_NOTHROW
//    { return Ops::fetchAndAddRelease(_q_value, valueToAdd); }
//    T fetchAndAddOrdered(T valueToAdd) Q_DECL_NOTHROW
//    { return Ops::fetchAndAddOrdered(_q_value, valueToAdd); }
};

typedef atomic_integer<int> atomic_int;

CWT_NS_END

#endif /* __CWT_ATOMIC_HPP__ */
