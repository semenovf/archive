/**
 * @file zlib.cpp
 * @author wladt
 * @date Apr 23, 2014
 */

#include "pfs/zlib.hpp"
#include <pfs/map.hpp>

namespace pfs {

static const struct zlib_rc_map
{
	map<int, string> m;
	zlib_rc_map () {
		m.insert(Z_OK           , _u8(_Tr("no errors")));
		m.insert(Z_STREAM_END   , _u8(_Tr("end of stream")));   // is not an error
		m.insert(Z_NEED_DICT    , _u8(_Tr("need preset dictionary"))); // is not an error
		m.insert(Z_ERRNO        , _u8(_Tr("file operation error (Z_ERRNO), see errno variable")));
		m.insert(Z_STREAM_ERROR , _u8(_Tr("invalid parameter or inconsistent stream state (Z_STREAM_ERROR)")));
		m.insert(Z_DATA_ERROR   , _u8(_Tr("data corrupted or inconsistent (Z_DATA_ERROR)")));
		m.insert(Z_MEM_ERROR    , _u8(_Tr("not enough memory (Z_MEM_ERROR)")));
		m.insert(Z_BUF_ERROR    , _u8(_Tr("buffer error (Z_BUF_ERROR), may be not enough memory, or I/O error")));
		m.insert(Z_VERSION_ERROR, _u8(_Tr("incompatible zlib version (Z_VERSION_ERROR)")));
	}

	string operator () (int rc) const
	{
		string s = string::toString(rc);
		s << _Tr(": unknown return code");
		return m.valueAt(rc, s);
	}
} __zlib_rc_map;


zlib::zlib ()
	: errorable_ext()
	, _level(DefaultCompression)
{}

zlib::zlib (zlib::compression_level l)
	: errorable_ext()
	, _level(l)
{}

/**
 * @brief Compresses the source buffer into the destination buffer.
 *
 * @param dest Destination buffer.
 * @param src  Source buffer.
 * @return @c true on successful decompression, @c false if an error occurred.
 */
byte_string zlib::compress (const pfs::byte_string & src)
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

	this->addError(__zlib_rc_map(rc));
	delete [] dest;

	return byte_string(); // null
}


/**
 * @brief Decompresses the source buffer into the destination buffer.
 *
 * @param dest       Destination buffer.
 * @param initialLen Must be large enough to hold the
 *                   entire uncompressed data. (The size of the uncompressed data must have
 *                   been saved previously by the compressor and transmitted to the decompressor
 *                   by some mechanism outside the scope of this compression library.)
 *
 * @param src Source buffer
 * @return @c true on successful decompression, @c false if an error occurred.
 */
byte_string zlib::decompress (size_t initialLen, const pfs::byte_string & src)
{
	byte_t * dest = new byte_t[initialLen];
	uLong destLen = initialLen;

	int rc = uncompress (dest
			, & destLen
			, reinterpret_cast<const byte_t *>(src.constData())
			, src.size());

	if (rc == Z_OK) {
		PFS_ASSERT(destLen <= PFS_SIZE_MAX);
		// FIXME need optimization
		byte_string r(dest, destLen);
		delete [] dest;
		return r;
	}

	delete [] dest;
	this->addError(__zlib_rc_map(rc));

	return byte_string(); // null
}


pfs::string zlib::strerror (int rc)
{
	return __zlib_rc_map(rc);
}

} // pfs


