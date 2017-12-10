/**
 * @file utils.hpp
 * @author wladt
 * @date May 6, 2014
 */

#ifndef __PFS_UTILS_HPP__
#define __PFS_UTILS_HPP__

#include <pfs/byte_string.hpp>

namespace pfs {

DLL_API int32_t crc32 (const void * pdata, size_t nbytes, int32_t initial = 0);
DLL_API int32_t crc32 (const byte_string & pdata, int32_t initial = 0);
DLL_API int64_t crc64 (const void * pdata, size_t nbytes, int64_t initial = 0);
DLL_API int64_t crc64 (const byte_string & pdata, int64_t initial = 0);

} // pfs

#endif /* __PFS_UTILS_HPP__ */
