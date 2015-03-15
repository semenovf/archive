/**
 * @file utils.hpp
 * @author wladt
 * @date May 6, 2014
 */

#ifndef __PFS_UTILS_HPP__
#define __PFS_UTILS_HPP__

#include <pfs/byte_string.hpp>

extern "C" int32_t pfs_crc32 (const void * pdata, size_t nbytes, int32_t initial);
extern "C" int64_t pfs_crc64_iso (const void * pdata, size_t nbytes, int64_t initial);

namespace pfs {

inline int32_t crc32 (const void * pdata, size_t nbytes, int32_t initial = 0)
	{ return ::pfs_crc32(pdata, nbytes, initial); }
inline int64_t crc64 (const void * pdata, size_t nbytes, int64_t initial = 0)
	{ return ::pfs_crc64_iso(pdata, nbytes, initial); }
inline int32_t crc32 (const byte_string & pdata, int32_t initial = 0)
	{ return ::pfs_crc32(pdata.data(), pdata.size(), initial); }
inline int64_t crc64 (const byte_string & pdata, int64_t initial = 0)
	{ return ::pfs_crc64_iso(pdata.data(), pdata.size(), initial); }

} // pfs

#endif /* __PFS_UTILS_HPP__ */
