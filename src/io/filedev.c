/*
 * filedev.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <cwt/stdio.h>
#include <cwt/string.h>
#include <cwt/str.h>
#include <cwt/unistd.h>
#include <cwt/io/filedev.h>
#include <cwt/logger.h>
#ifdef CWT_CC_MSC
#	include <share.h> /* file sharing modes */
#endif

static void    __dev_close(CwtIODevice*);
static size_t  __dev_bytesAvailable(CwtIODevice*);
static ssize_t __dev_read(CwtIODevice*, BYTE*, size_t);
static ssize_t __dev_write(CwtIODevice*, const BYTE*, size_t);

typedef struct CwtFileDevice
{
	CwtIODevice __base;

	int in;  /* input file handler */
	int out; /* output file handler */

	off_t read_pos;
	/*off_t read_pos_saved;*/
} CwtFileDevice;


/**
 *
 * @param path
 * @param mode
 * @return
 */
DLL_API_EXPORT CwtIODevice* cwtFileDeviceOpen(const CWT_CHAR *path, CwtOpenMode mode)
{
	CwtFileDevice *fd;
	CwtUnistdNS *ns = cwtUnistdNS();
	int oflags = 0;
	int fh = -1;


	if( mode & Cwt_FileRead )
		oflags |= O_RDONLY;
	if( mode & Cwt_FileWrite )
		oflags |= O_WRONLY;
	if( mode & Cwt_FileReadWrite )
		oflags |= O_RDWR;

	if( mode & Cwt_FileCreate )
		oflags |= O_CREAT;
	else
		oflags |= O_APPEND;

#ifdef CWT_CC_GNUC
	if( mode & Cwt_FileNonBlocking )
		oflags |= O_NONBLOCK;
#endif

#ifdef CWT_CC_MSC
	oflags |= O_BINARY;
#endif

	if( path ) {
		fh = ns->open(path, oflags, 0);
		if( fh < 0 ) {
			printf_error(_Tr("unable to open input file: %s: %s"), path, cwtStrNS()->strError(errno));
			return (CwtIODevice*)NULL;
		}
	} else {
		print_error(_Tr("unspecified path"));
		return (CwtIODevice*)NULL;
	}

	fd = CWT_MALLOC(CwtFileDevice);
	fd->in  = -1;
	fd->out = -1;

	if( (mode & Cwt_FileReadWrite) || (mode & Cwt_FileRead) )
		fd->in  = fh;

	if( (mode & Cwt_FileReadWrite) || (mode & Cwt_FileWrite) )
		fd->out = fh;

	if( fh > 0 )
		CWT_ASSERT(ns->lseek(fh, 0L, SEEK_SET) >= 0L);


	fd->read_pos = 0L;
	/*fd->read_pos_saved = 0L;*/

	fd->__base.close          = __dev_close;
	fd->__base.bytesAvailable = __dev_bytesAvailable;
	fd->__base.read           = __dev_read;
	fd->__base.write          = __dev_write;

	return (CwtIODevice*)fd;
}

#ifdef CWT_CC_MSC
/**
 * Opens file device consists of two parts: file for read and file for write
 *
 * @param infilename file path for input. May be NULL, so device can't read.
 * @param outfilename file path for output. May be NULL, so device can't write.
 * @param master if @c true files will be created and/or truncated
 * @return
 */
DLL_API_EXPORT CwtIODevice* cwtSharedFileDeviceOpen(const CWT_CHAR* infilename, const CWT_CHAR* outfilename, BOOL master)
{
	CwtFileDevice* fd;
	CwtUnistdNS *ns = cwtUnistdNS();

	int imode = O_RDWR | O_BINARY;
	int omode = O_RDWR | O_BINARY;

	if( master ) {
		imode |= O_CREAT | O_TRUNC;
		omode |= O_CREAT | O_TRUNC;
	}

	fd = CWT_MALLOC(CwtFileDevice);
	fd->in  = -1;
	fd->out = -1;

	if( infilename ) {
			fd->in = ns->sopen(infilename, imode, SH_DENYNO, S_IREAD | S_IWRITE);
			if( fd->in < 0 ) {
					printf_error(_Tr("unable to open input file: %s: %s"), infilename, cwtStrNS()->strError(errno));
					CWT_FREE(fd);
					return (CwtIODevice*)NULL;
			}
	}

	if( outfilename ) {
		fd->out = ns->sopen(outfilename, omode, SH_DENYNO, S_IREAD | S_IWRITE);
		if( fd->out < 0 ) {
			printf_error(_Tr("unable to open output file: %s: %s"), infilename, cwtStrNS()->strError(errno));
			if( fd->in > 0 ) {
					ns->close(fd->in);
			}
			CWT_FREE(fd);
			return (CwtIODevice*)NULL;
		}
	}

	if( fd->in > 0 )
		CWT_ASSERT(ns->lseek(fd->in, 0L, SEEK_SET) >= 0L);

	if( fd->out > 0 )
		CWT_ASSERT(ns->lseek(fd->out, 0L, SEEK_SET) >= 0L);

	fd->read_pos = 0L;

	fd->__base.close          = __dev_close;
	fd->__base.bytesAvailable = __dev_bytesAvailable;
	fd->__base.read           = __dev_read;
	fd->__base.write          = __dev_write;

	return (CwtIODevice*)fd;
}
#endif

void __dev_close(CwtIODevice *dev)
{
	CwtFileDevice *fd;
	CwtUnistdNS *ns = cwtUnistdNS();

	CWT_ASSERT(dev);
	fd = (CwtFileDevice*)dev;

	if( fd->in > 0 )
		ns->close(fd->in);

	if( fd->in != fd->out && fd->out > 0 )
		ns->close(fd->out);

	fd->in = fd->out = -1;
	CWT_FREE(fd);
}

size_t __dev_bytesAvailable(CwtIODevice *dev)
{
	CwtUnistdNS *ns = cwtUnistdNS();
	CwtFileDevice *fd;
	off_t total;

	CWT_ASSERT(dev);
	fd = (CwtFileDevice*)dev;

	if( fd->in < 0 ) {
		print_error(_Tr("file device does not support read operations"));
		return (size_t)0;
	}

	total = ns->lseek(fd->in, 0L, SEEK_END);
	CWT_ASSERT(total >= 0L);

	return (size_t)(total - fd->read_pos);
}

ssize_t __dev_read(CwtIODevice *dev, BYTE* buf, size_t sz)
{
	CwtUnistdNS *ns = cwtUnistdNS();
	CwtFileDevice *fd;
	ssize_t br;

	CWT_ASSERT(dev);
	fd = (CwtFileDevice*)dev;

	if( fd->in < 0 ) {
		print_error(_Tr("file device does not support read operations"));
		return (size_t)-1;
	}

	sz = CWT_MIN(sz, __dev_bytesAvailable(dev));
	CWT_ASSERT(ns->lseek(fd->in, fd->read_pos, SEEK_SET) >= 0L);

	/*FIXME: warning C4267: 'function' : conversion from 'size_t' to 'UINT', possible loss of data */
	br = ns->read(fd->in, buf, /*(UINT)*/sz);

	CWT_ASSERT(br >= 0);
	fd->read_pos += br;

	return br;
}

ssize_t __dev_write(CwtIODevice *dev, const BYTE* buf, size_t sz)
{
	CwtUnistdNS *ns = cwtUnistdNS();
	CwtFileDevice *fd;
	ssize_t bw;

	CWT_ASSERT(dev);
	fd = (CwtFileDevice*)dev;

	if( fd->out < 0 ) {
		print_error(_Tr("file device does not support write operations"));
		return (size_t)-1;
	}

	CWT_ASSERT(ns->lseek(fd->out, 0L, SEEK_END) >= 0L);
	CWT_ASSERT(sz > CWT_INT_MAX);

	bw = ns->write(fd->out, buf, (int)sz);
	CWT_ASSERT(bw >= 0);

	return bw;
}

/*
void __cwtFileReadBegin(CwtIODevice *dev)
{
	CwtFileDevice *fd;
	CWT_ASSERT(dev);

	fd = (CwtFileDevice*)dev;
	fd->read_pos_saved = fd->read_pos;
}

void __cwtFileReadCommit(CwtIODevice *dev)
{
	CwtFileDevice *fd;
	CWT_ASSERT(dev);

	fd = (CwtFileDevice*)dev;
	fd->read_pos_saved = fd->read_pos;
}

void __cwtFileReadRollback(CwtIODevice *dev)
{
	CwtFileDevice *fd;
	CWT_ASSERT(dev);

	fd = (CwtFileDevice*)dev;
	fd->read_pos = fd->read_pos_saved;
}
*/

