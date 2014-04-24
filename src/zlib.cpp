/**
 * @file zlib.cpp
 * @author wladt
 * @date Apr 23, 2014
 */

#include "../include/cwt/zlib.hpp"

namespace cwt {

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

/* ===========================================================================
     Compresses the source buffer into the destination buffer. The level
   parameter has the same meaning as in deflateInit.  sourceLen is the byte
   length of the source buffer. Upon entry, destLen is the total size of the
   destination buffer, which must be at least 0.1% larger than sourceLen plus
   12 bytes. Upon exit, destLen is the actual size of the compressed buffer.

     compress2 returns Z_OK if success, Z_MEM_ERROR if there was not enough
   memory, Z_BUF_ERROR if there was not enough room in the output buffer,
   Z_STREAM_ERROR if the level parameter is invalid.
*/

bool zlib::compress (const pfs::bytearray & src, pfs::bytearray & dest)
{
//	int ZEXPORT compress2 (dest, destLen, source, sourceLen, level)
//	    Bytef *dest;
//	    uLongf *destLen;
//	    const Bytef *source;
//	    uLong sourceLen;
//	    int level;
//	{
	z_stream stream;
	int err;

	// Destination buffer size must be at least 0.1% larger than source buffer
	// size plus 12 bytes.
	dest.resize(src.size() + src.size() * 0.001 + 1 + 12);

	stream.next_in = (z_const Bytef *)src.data();
	stream.avail_in = (uInt)src.size();

	#ifdef MAXSEG_64K
	/* Check for source > 64K on 16-bit machine: */
	if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;
	#endif
	stream.next_out = dest;
	stream.avail_out = (uInt)*destLen;
	if ((uLong)stream.avail_out != *destLen)
		return Z_BUF_ERROR;

	stream.zalloc = (alloc_func)0;
	stream.zfree = (free_func)0;
	stream.opaque = (voidpf)0;

	err = deflateInit(& stream, _level);
	if (err != Z_OK)
		return err;

	err = deflate(& stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		deflateEnd(& stream);
		return err == Z_OK ? Z_BUF_ERROR : err;
	}

	*destLen = stream.total_out;

	err = deflateEnd(& stream);
	return err;
}

} // cwt


