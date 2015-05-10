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

typedef pfs::mbcs_string<uint8_t> utf8_string;
typedef utf8_string string;

#define _latin1(s) pfs::string::fromLatin1(s)
#define _l1(s)     pfs::string::fromLatin1(s)
#define _u8(s)     pfs::string::fromUtf8(s)
#define _u16(s)    pfs::string::fromUtf16(s)

} // pfs

#endif /* __PFS_STRING_HPP__ */
