/**
 * @file zlib.cpp
 * @author wladt
 * @date Apr 23, 2014
 */

#include "../include/cwt/zlib.hpp"
#include <pfs/map.hpp>

namespace cwt {

static const struct zlib_rc_map
{
	pfs::map<int, pfs::string> m;
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

	pfs::string operator () (int rc) const { return m.value(rc, _Fr("%d: unknown return code") % rc); }
} __zlib_rc_map;


zlib::zlib ()
	: errorable()
	, _level(DefaultCompression)
	, _chunk_size(DefaultChunkSize)
{}

zlib::zlib (zlib::level l, size_t chunk_size)
	: errorable()
	, _level(l)
	, _chunk_size(chunk_size)
{}

/**
 * @brief Compresses the source buffer into the destination buffer.
 *
 * @param dest Destination buffer.
 * @param src  Source buffer.
 * @return @c true on successful decompression, @c false if an error occurred.
 */
bool zlib::compress (pfs::bytearray & dest, const pfs::bytearray & src)
{
	// Destination buffer size must be at least 0.1% larger than source buffer
	// size plus 12 bytes.
	uLong destLen = src.size() + src.size() * 0.001 + 1 + 12;
	dest.resize(destLen);

	int rc = compress2(reinterpret_cast<byte_t *>(dest.data())
			, & destLen
			, reinterpret_cast<const byte_t *>(src.data())
			, (uInt)src.size()
			, _level);

	if (rc != Z_OK)
		this->addError(__zlib_rc_map(rc));

	dest.resize(destLen);

	return rc == Z_OK;
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
bool zlib::decompress (pfs::bytearray & dest, size_t initialLen, const pfs::bytearray & src)
{
	dest.resize(initialLen);
	uLong destLen = initialLen;
	int rc = uncompress (reinterpret_cast<byte_t *>(dest.data())
			, & destLen
			, reinterpret_cast<const byte_t *>(src.constData())
			, src.size());

	if (rc == Z_OK) {
		PFS_ASSERT(destLen <= PFS_SIZE_MAX);
		dest.resize(size_t(destLen));
	} else {
		this->addError(__zlib_rc_map(rc));
	}

	return rc == Z_OK;
}

} // cwt


