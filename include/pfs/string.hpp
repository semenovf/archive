/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#include <pfs/utf8/traits.hpp>
#include <pfs/utf/string.hpp>

#define _u8(s) pfs::utf8_string(s)

namespace pfs {

typedef pfs::utf::string<uint8_t>  utf8_string;
typedef pfs::utf::string<uint16_t> utf16_string;
typedef pfs::utf::string<uint32_t> utf32_string;

#if PFS_STRING_UTF16

typedef utf16_string string;

#elif PFS_STRING_UTF32

typedef utf32_string string;

#else

typedef utf8_string string;

#endif

} // pfs

#endif /* __PFS_STRING_HPP__ */
