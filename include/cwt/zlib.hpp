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
	static const size_t DefaultChunkSize = 16384;

	enum level {
		  NoCompression      = Z_NO_COMPRESSION
		, BestSpeed          = Z_BEST_SPEED
		, BestCompression    = Z_BEST_COMPRESSION
		, DefaultCompression = Z_DEFAULT_COMPRESSION
	};

private:
	level  _level;
	size_t _chunk_size;

public:
	zlib ();
	zlib (zlib::level l, size_t chunk_size = DefaultChunkSize);

	void setChunkSize (size_t sz) { _chunk_size = sz > 32 ? sz : DefaultChunkSize; }
	size_t chunkSize () const { return _chunk_size; }

	bool compress (pfs::bytearray & dest, const pfs::bytearray & src);
	bool decompress (pfs::bytearray & dest, size_t initialLen, const pfs::bytearray & src);

	// FIXME implement
	//bool decompress (pfs::bytearray & dest, const pfs::bytearray & src);
};

} // cwt

#endif /* __CWT_ZLIB_HPP__ */
