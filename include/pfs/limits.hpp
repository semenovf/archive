/*
 * limits.hpp
 *
 *  Created on: Nov 24, 2015
 *      Author: wladt
 */

#ifndef __PFS_LIMITS_HPP__
#define __PFS_LIMITS_HPP__

namespace pfs {

#include <limits>
using namespace std;

} // pfs

#if defined(LLONG_MAX)                   \
	|| defined(__LONG_LONG_MAX__)  // valid for gcc

#	ifdef PFS_HAVE_LONG_LONG
#		undef PFS_HAVE_LONG_LONG
#	endif
#   define PFS_HAVE_LONG_LONG 1
#endif

#ifdef __LDBL_MIN__      // valid for gcc
#	ifdef PFS_HAVE_LONG_DOUBLE
#		undef PFS_HAVE_LONG_DOUBLE
#	endif
#	define PFS_HAVE_LONG_DOUBLE 1
#endif

namespace pfs {

template <typename T> T max_value () { return pfs::numeric_limits<T>::max(); }
template <typename T> T min_value () { return pfs::numeric_limits<T>::min(); }

template <typename T> T max_value (const T &) { return pfs::numeric_limits<T>::max(); }
template <typename T> T min_value (const T &) { return pfs::numeric_limits<T>::min(); }

} // pfs

#endif /* __PFS_LIMITS_HPP__ */
