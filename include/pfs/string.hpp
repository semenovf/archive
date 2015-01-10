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
#include <cstdio>

namespace pfs {

typedef pfs::mbcs_string<char> utf8string;
// TODO need to select type of string by OS (Linux/Mac: ut8string; Windows: utf16string)
typedef utf8string string;

#define _latin1(s) pfs::utf8string::fromLatin1(s)
#define _l1(s) pfs::utf8string::fromLatin1(s)
#define _u8(s) pfs::utf8string::fromUtf8(s)
#define _u16(s) pfs::utf8string::fromUtf16(s)

} // pfs

#endif /* __PFS_STRING_HPP__ */
