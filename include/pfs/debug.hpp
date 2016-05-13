/*
 * debug.hpp
 *
 *  Created on: Mar 14, 2016
 *      Author: wladt
 */

#ifndef __PFS_DEBUG_HPP__
#define __PFS_DEBUG_HPP__

#include <iostream>

#ifndef NDEBUG
#	define PFS_DEBUG(x) x
#   define PFS_WARN(x) std::cerr << "WARN: " << x << std::endl
#   define PFS_ERROR(x) std::cerr << "ERROR: " << x << std::endl
#else
#	define PFS_DEBUG(x)
#   define PFS_WARN(x)
#   define PFS_ERROR(x)
#endif

#endif /* __PFS_DEBUG_HPP__ */
