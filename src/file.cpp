/**
 * @file file.cpp
 * @author wladt
 * @date Apr 28, 2014
 */

#include "../include/cwt/io/file.hpp"
#include <pfs/scoped_ptr.hpp>
#include <cwt/zlib.hpp>

namespace cwt { namespace io {

bool file::compress (file & dest, cwt::zlib::compression_level level, size_t chunkSize)
{
	if (chunkSize < 32)
		chunkSize = 0x4000;

	int flush;
	z_stream strm;
	pfs::scoped_array_ptr<unsigned char> in(new unsigned char[chunkSize]);
	pfs::scoped_array_ptr<unsigned char> out(new unsigned char[chunkSize]);

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	int rc = deflateInit(& strm, level);

	if (rc != Z_OK) {
		this->addError(cwt::zlib::strerror(rc));
		return false;
	}

	/* compress until end of file */
	do {
		strm.avail_in = this->read(reinterpret_cast<char*>(in.get()), chunkSize);

		if (this->isError()) {
			(void)deflateEnd(& strm);
			return false;
		}

		flush = this->atEnd() ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in.get();

		/* run deflate() on input until output buffer not full, finish
		   compression if all of source has been read in */
        do {
        	strm.avail_out = chunkSize;
        	strm.next_out = out.get();
        	rc = deflate(& strm, flush);    /* no bad return value */
        	PFS_ASSERT(rc != Z_STREAM_ERROR);  /* state not clobbered */
        	size_t have = chunkSize - strm.avail_out;

        	if (dest.write(reinterpret_cast<char*>(out.get()), have) != ssize_t(have) || dest.isError()) {
        		(void)deflateEnd(& strm);
        		return false;
        	}
        } while (strm.avail_out == 0);

        PFS_ASSERT(strm.avail_in == 0);     /* all input will be used */

	  /* done when last data in file processed */
	} while (flush != Z_FINISH);

	PFS_ASSERT(rc == Z_STREAM_END);        /* stream will be complete */

	/* clean up and return */
	deflateEnd(& strm);
	return true;
}

bool file::uncompress (file & dest, size_t chunkSize)
{
    z_stream strm;
	pfs::scoped_array_ptr<unsigned char> in(new unsigned char[chunkSize]);
	pfs::scoped_array_ptr<unsigned char> out(new unsigned char[chunkSize]);

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    int rc = inflateInit(& strm);

	if (rc != Z_OK) {
		this->addError(cwt::zlib::strerror(rc));
		return false;
	}

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = this->read(reinterpret_cast<char*>(in.get()), chunkSize);

        if (this->isError()) {
            (void)inflateEnd(&strm);
            return false;
        }

        if (strm.avail_in == 0)
            break;

        strm.next_in = in.get();

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = chunkSize;
            strm.next_out = out.get();
            rc = inflate(& strm, Z_NO_FLUSH);
            PFS_ASSERT(rc != Z_STREAM_ERROR);  /* state not clobbered */

            switch (rc) {
            case Z_NEED_DICT:
                rc = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return rc;
            }

            size_t have = chunkSize - strm.avail_out;

            if (dest.write(reinterpret_cast<char*>(out.get()), have) != ssize_t(have) || dest.isError()) {
                inflateEnd(& strm);
                return false;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (rc != Z_STREAM_END);

    /* clean up and return */
    inflateEnd(&strm);

    if (rc != Z_STREAM_END) {
    	this->addError(cwt::zlib::strerror(Z_DATA_ERROR));
    	return false;
    }

    return true;
}


}} // cwt::io
