/**
 * @file utils.hpp
 * @author wladt
 * @date May 6, 2014
 */

#ifndef __CWT_UTILS_HPP__
#define __CWT_UTILS_HPP__

#include <pfs/bytearray.hpp>

extern "C" int32_t cwt_crc32 (const void * pdata, size_t nbytes, int32_t initial);
extern "C" int64_t cwt_crc64_iso (const void * pdata, size_t nbytes, int64_t initial);

namespace cwt {

inline int32_t crc32 (const void * pdata, size_t nbytes, int32_t initial = 0)
	{ return ::cwt_crc32(pdata, nbytes, initial); }
inline int64_t crc64 (const void * pdata, size_t nbytes, int64_t initial = 0)
	{ return ::cwt_crc64_iso(pdata, nbytes, initial); }
inline int32_t crc32 (const pfs::bytearray & pdata, int32_t initial = 0)
	{ return ::cwt_crc32(pdata.data(), pdata.size(), initial); }
inline int64_t crc64 (const pfs::bytearray & pdata, int64_t initial = 0)
	{ return ::cwt_crc64_iso(pdata.data(), pdata.size(), initial); }

} // cwt

#endif /* __CWT_UTILS_HPP__ */
