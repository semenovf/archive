/**
 * @file zlib.hpp
 * @author wladt
 * @date Apr 23, 2014
 */

#ifndef __PFS_ZLIB_HPP__
#define __PFS_ZLIB_HPP__

#include "bits/zlib.h"
#include <pfs/byte_string.hpp>
#include <pfs/errorable_ext.hpp>

namespace pfs {

class DLL_API zlib : public errorable_ext
{
//	static const size_t DefaultChunkSize = 16384;
public:
	enum compression_level {
		  NoCompression      = Z_NO_COMPRESSION
		, BestSpeed          = Z_BEST_SPEED
		, BestCompression    = Z_BEST_COMPRESSION
		, DefaultCompression = Z_DEFAULT_COMPRESSION
	};

private:
	compression_level  _level;
//	size_t _chunk_size;

public:
	zlib ();
	zlib (zlib::compression_level l);

	byte_string compress (const byte_string & src);
	byte_string decompress (size_t initialLen, const byte_string & src);

	// FIXME implement
	//bool decompress (pfs::byte_string & dest, const pfs::byte_string & src);

	static string strerror (int rc);
};

} // pfs

#endif /* __PFS_ZLIB_HPP__ */
