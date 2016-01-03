/**
 * @file zlib.hpp
 * @author wladt
 * @date Apr 23, 2014
 */

#ifndef __PFS_ZLIB_HPP__
#define __PFS_ZLIB_HPP__

#include "bits/zlib.h"
#include <pfs/byte_string.hpp>
#include <pfs/error_code.hpp>

namespace pfs {

class DLL_API zlib
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

	/**
	 * @brief Compresses the source buffer into the destination buffer.
	 *
	 * @param src  Source data.
	 * @param ex Pointer to store error if occurred while compressing.
	 *
	 * @return Compressed data.
	 */
	byte_string compress (const byte_string & src
			, error_code * ex = 0);

	/**
	 * @brief Decompresses the source buffer.
	 *
	 * @param initial_len Must be large enough to hold the
	 *        entire uncompressed data. (The size of the uncompressed data must have
	 *        been saved previously by the compressor and transmitted to the decompressor
	 *        by some mechanism outside the scope of this compression library.)
	 * @param src Source (compressed data).
	 * @param ex Pointer to store error if occurred while decompressing.
	 *
	 * @return Decompressed data.
	 */
	byte_string decompress (size_t initial_len
			, const byte_string & src
			, error_code * ex);

	// FIXME implement
	//bool decompress (pfs::byte_string & dest, const pfs::byte_string & src);
};

} // pfs

#endif /* __PFS_ZLIB_HPP__ */
