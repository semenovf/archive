/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#include <pfs/utf8string.hpp>

namespace pfs {

typedef utf8string string;

#define _latin1(s) pfs::utf8string::fromLatin1(s)
#define _l1(s) pfs::utf8string::fromLatin1(s)
#define _u8(s) pfs::utf8string::fromUtf8(s)

} // pfs

#endif /* __PFS_STRING_HPP__ */
