/**
 * @file zlib.hpp
 * @author wladt
 * @date Apr 23, 2014
 */

#ifndef __CWT_ZLIB_HPP__
#define __CWT_ZLIB_HPP__

#include "bits/zlib.h"
#include <pfs/bytearray.hpp>
#include <cwt/errorable.hpp>

namespace cwt {

class DLL_API zlib : public errorable
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

	bool compress (pfs::bytearray & dest, const pfs::bytearray & src);
	bool decompress (pfs::bytearray & dest, size_t initialLen, const pfs::bytearray & src);

	// FIXME implement
	//bool decompress (pfs::bytearray & dest, const pfs::bytearray & src);

	static pfs::string strerror (int rc);
};

} // cwt

#endif /* __CWT_ZLIB_HPP__ */
