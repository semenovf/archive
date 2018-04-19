/**
 * @file zlib.cpp
 * @author wladt
 * @date Apr 23, 2014
 */

#include "pfs/zlib.hpp"
#include <pfs/map.hpp>

namespace pfs {

static error_code convert_zlib_errno (int rc)
{
	switch (rc) {
	// file operation error, see errno variable;
	//
	case Z_ERRNO:
		return error_code(errno);

	// invalid parameter or inconsistent stream state
	//
	case Z_STREAM_ERROR:
		return error_code(ZlibStreamError);

	// data corrupted or inconsistent
	//
	case Z_DATA_ERROR:
		return error_code(ZlibDataError);

	// not enough memory
	//
	case Z_MEM_ERROR:
		return error_code(ZlibMemError);

	// buffer error, may be not enough memory, or I/O error
	//
	case Z_BUF_ERROR:
		return error_code(ZlibBufError);

	// incompatible zlib version
	//
	case Z_VERSION_ERROR:
		return error_code(ZlibVersionError);

	// no errors
	//
	case Z_OK:

	// end of stream, is not an error
	//
	case Z_STREAM_END:

	// need preset dictionary, is not an error
	//
	case Z_NEED_DICT:
	default:
		break;
	}

	return error_code(0);
}

zlib::zlib ()
	: _level(DefaultCompression)
{}

zlib::zlib (zlib::compression_level l)
	: _level(l)
{}

byte_string zlib::compress (const pfs::byte_string & src, error_code * ex)
{
	// Destination buffer size must be at least 0.1% larger than source buffer
	// size plus 12 bytes.
	uLong destLen = src.size() + uLong(src.size() * 0.001) + 1 + 12;
	byte_t * dest = new byte_t[destLen];

	int rc = compress2(dest
			, & destLen
			, reinterpret_cast<const byte_t *>(src.data())
			, (uInt)src.size()
			, _level);

	if (rc == Z_OK) {
		// FIXME need optimization
		byte_string r(dest, destLen);
		delete [] dest;
		return r;
	}

	if (ex)
		*ex = convert_zlib_errno(rc);

	delete [] dest;

	return byte_string(); // null
}


byte_string zlib::decompress (size_t initial_len
		, const pfs::byte_string & src
		, error_code * ex)
{
	byte_t * dest = new byte_t[initial_len];
	uLong dest_len = initial_len;

	int rc = uncompress(dest
			, & dest_len
			, reinterpret_cast<const byte_t *>(src.data())
			, src.size());

	if (rc == Z_OK) {
		PFS_ASSERT(dest_len <= max_value<uLong>());
		// FIXME need optimization
		byte_string r(dest, dest_len);
		delete [] dest;
		return r;
	}

	if (ex)
		*ex = convert_zlib_errno(rc);

	delete [] dest;

	return byte_string(); // null
}

} // pfs


