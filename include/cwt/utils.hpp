/**
 * @file utils.hpp
 * @author wladt
 * @date May 6, 2014
 */

#ifndef __CWT_UTILS_HPP__
#define __CWT_UTILS_HPP__

#include <pfs.hpp>

extern "C" int32_t cwt_crc32 (const void * pdata, size_t nbytes, int32_t initial);
extern "C" int64_t cwt_crc64 (const void * pdata, size_t nbytes, int64_t initial);

namespace cwt {

inline int32_t crc32 (const void * pdata, size_t nbytes, int32_t initial) { return ::cwt_crc32(pdata, nbytes, initial); }
inline int64_t crc64 (const void * pdata, size_t nbytes, int64_t initial) { return ::cwt_crc32(pdata, nbytes, initial); }

} // cwt

#endif /* __CWT_UTILS_HPP__ */
