/*
 * test_p.hpp
 *
 *  Created on: Nov 30, 2013
 *      Author: wladt
 */

#ifndef __PFS_TEST_P_HPP__
#define __PFS_TEST_P_HPP__

#if defined(NDEBUG) || defined(_NDEBUG)
#	define PFS_ASSERT(p) p
#else
#	include <cassert>
#	define PFS_ASSERT(p) assert(p)
#endif

#if __cplusplus < 201103L
#	ifndef NULL
#		define nullptr 0
#	else
#		define nullptr NULL
#	endif
#endif

#endif /* __PFS_TEST_P_HPP__ */
