/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#if PFS_STRING_UTF16

#include <pfs/utf16/string.hpp>
#define _u8(s) pfs::utf16::string(s) // FIXME need convert

#elif PFS_STRING_UTF32

#include <pfs/utf32/string.hpp>
#define _u8(s) pfs::utf32::string(s) // FIXME need convert

#else

#include <pfs/utf8/string.hpp>
#define _u8(s) pfs::utf8::string(s)

#endif

namespace pfs {

#if PFS_STRING_UTF16

typedef utf16::string string;

#elif PFS_STRING_UTF32

typedef utf32::string string;

#else

typedef utf8::string string;

#endif

} // pfs

#include <pfs/bits/to_string.hpp>
#include <pfs/bits/strtointegral.hpp>

#endif /* __PFS_STRING_HPP__ */
