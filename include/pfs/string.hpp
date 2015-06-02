/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#include <pfs/mbcs_string.hpp>

#define _latin1(s) pfs::string::fromLatin1(s)
#define _l1(s)     pfs::string::fromLatin1(s)
#define _u8(s)     pfs::string::fromUtf8(s)
#define _u16(s)    pfs::string::fromUtf16(s)

#ifdef _Tr
#   undef _Tr
#endif

#define _Tr(s) _u8(s).c_str()

namespace pfs {

typedef pfs::mbcs_string<uint8_t>  utf8_string;
typedef pfs::mbcs_string<uint16_t> utf16_string;
typedef pfs::mbcs_string<uint32_t> utf32_string;

typedef utf8_string string;

} // pfs

#endif /* __PFS_STRING_HPP__ */
