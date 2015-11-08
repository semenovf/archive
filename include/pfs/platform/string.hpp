/*
 * string.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: wladt
 */

#ifndef __PFS_PLATFORM_STRING_HPP__
#define __PFS_PLATFORM_STRING_HPP__

#include <pfs.hpp>
#include <string>

namespace pfs { namespace platform {

#if PFS_CC_MSC
	typedef wchar_t char_type;
#else
	typedef char char_type;
#endif

typedef std::basic_string<char_type> string_type;

}}

#endif /* __PFS_PLATFORM_STRING_HPP__ */
