/**
 * @file   atomic_mt.cpp
 * @author wladt
 * @date   Mar 27, 2013 2:32:36 PM
 *
 * @brief
 */

#include "../include/pfs/atomic.hpp"

#if defined(HAVE_ATOMIC_MT)

CWT_NS_BEGIN

template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<char>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<unsigned char>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<short int>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<unsigned short int>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<int>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<unsigned int>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<long int>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<unsigned long int>::g_mutex;

#ifdef HAVE_INT64
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<long long int>::g_mutex;
template <>CWT_DEFAULT_MT_POLICY atomic_integer_intrinsics<unsigned long long int>::g_mutex;
#endif

CWT_NS_END

#endif