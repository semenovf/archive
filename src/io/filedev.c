/*
 * filedev.c
 *
 *  Created on: 18.11.2011
 *      Author: user
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <share.h> /* file sharing modes */
#include <fcntl.h>
#include <errno.h>
#include <cwt/stdio.h>
#include <cwt/string.h>
#include <cwt/str.h>
#include <cwt/unistd.h>
#include <cwt/io/filedev.h>
#include <cwt/logger.h>

static void    __cwtFileClose(CwtIODevicePtr);
static size_t  __cwtFileBytesAvailable(CwtIODevicePtr);
static ssize_t __cwtFileRead(CwtIODevicePtr, BYTE*, size_t);
static ssize_t __cwtFileWrite(CwtIODevicePtr, const BYTE*, size_t);
static void    __cwtFileReadBegin(CwtIODevicePtr);
static void    __cwtFileReadCommit(CwtIODevicePtr);
static void    __cwtFileReadRollback(CwtIODevicePtr);

typedef struct CwtFileDevice
{
	CwtIODevice base;

	int in;  /* input file handler */
	int out; /* output file handler */
	off_t read_pos;
	off_t read_pos_saved;
} CwtFileDevice, *CwtFileDevicePtr;


DLL_API_EXPORT CwtIODevicePtr cwtFileDeviceOpen(const CWT_CHAR *path, int mode)
{
	CwtFileDevicePtr fd;
	CwtUnistdNS *ns = cwtUnistdNS();
	int fh;

	if( path ) {
		fh = ns->open(path, mode, 0);
		if( fh < 0 ) {
			printf_error(_Tr("unable to open input file: %s: %s"), path, strerror(errno));
			return (CwtIODevicePtr)NULL;
		}
	}

	fd = CWT_MALLOC(CwtFileDevice);
	fd->in  = -1;
	fd->out = -1;

	if( (mode & O_RDWR) || (mode & O_RDONLY) )
		fd->in  = fh;

	if( (mode & O_RDWR) || (mode & O_WRONLY) )
		fd->out = fh;

	if( fh > 0 )
		CWT_ASSERT(ns->lseek(fh, 0L, SEEK_SET) >= 0L);

	fd->read_pos = 0L;
	fd->read_pos_saved = 0L;

	fd->base.close          = __cwtFileClose;
	fd->base.bytesAvailable = __cwtFileBytesAvailable;
	fd->base.read           = __cwtFileRead;
	fd->base.write          = __cwtFileWrite;

	fd->base.readBegin      = __cwtFileReadBegin;
	fd->base.readCommit     = __cwtFileReadCommit;
	fd->base.readRollback   = __cwtFileReadRollback;

	return (CwtIODevicePtr)fd;
}

/**
 * Opens file device consists of two parts: file for read and file for write
 *
 * @param infilename file path for input. May be NULL, so device can't read.
 * @param outfilename file path for output. May be NULL, so device can't write.
 * @param master if @c true files will be created and/or truncated
 * @return
 */
DLL_API_EXPORT CwtIODevicePtr cwtSharedFileDeviceOpen(const CWT_CHAR* infilename, const CWT_CHAR* outfilename, BOOL master)
{
	CwtFileDevicePtr fd;
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
					printf_error(_Tr("unable to open input file: %s: %s"), infilename, cwtStrNS()->strerror(errno));
					CWT_FREE(fd);
					return (CwtIODevicePtr)NULL;
			}
	}

	if( outfilename ) {
		fd->out = ns->sopen(outfilename, omode, SH_DENYNO, S_IREAD | S_IWRITE);
		if( fd->out < 0 ) {
			printf_error(_Tr("unable to open output file: %s: %s"), infilename, cwtStrNS()->strerror(errno));
			if( fd->in > 0 ) {
					ns->close(fd->in);
			}
			CWT_FREE(fd);
			return (CwtIODevicePtr)NULL;
		}
	}

	if( fd->in > 0 )
		CWT_ASSERT(ns->lseek(fd->in, 0L, SEEK_SET) >= 0L);

	if( fd->out > 0 )
		CWT_ASSERT(ns->lseek(fd->out, 0L, SEEK_SET) >= 0L);

	fd->read_pos = 0L;
	fd->read_pos_saved = 0L;

	fd->base.close          = __cwtFileClose;
	fd->base.bytesAvailable = __cwtFileBytesAvailable;
	fd->base.read           = __cwtFileRead;
	fd->base.write          = __cwtFileWrite;

	fd->base.readBegin      = __cwtFileReadBegin;
	fd->base.readCommit     = __cwtFileReadCommit;
	fd->base.readRollback   = __cwtFileReadRollback;

	return (CwtIODevicePtr)fd;
}


void __cwtFileClose(CwtIODevicePtr dev)
{
	CwtFileDevicePtr fd;
	CwtUnistdNS *ns = cwtUnistdNS();

	CWT_ASSERT(dev);
	fd = (CwtFileDevicePtr)dev;

	if( fd->in > 0 )
		ns->close(fd->in);

	if( fd->in != fd->out && fd->out > 0 )
		ns->close(fd->out);

	fd->in = fd->out = -1;
	CWT_FREE(fd);
}

size_t __cwtFileBytesAvailable(CwtIODevicePtr dev)
{
	CwtUnistdNS *ns = cwtUnistdNS();
	CwtFileDevicePtr fd;
	off_t total;

	CWT_ASSERT(dev);
	fd = (CwtFileDevicePtr)dev;

	if( fd->in < 0 ) {
		print_error(_Tr("file device does not support read operations"));
		return (size_t)0;
	}

	total = ns->lseek(fd->in, 0L, SEEK_END);
	CWT_ASSERT(total >= 0L);

	return (size_t)(total - fd->read_pos);
}

ssize_t __cwtFileRead(CwtIODevicePtr dev, BYTE* buf, size_t sz)
{
	CwtUnistdNS *ns = cwtUnistdNS();
	CwtFileDevicePtr fd;
	ssize_t br;

	CWT_ASSERT(dev);
	fd = (CwtFileDevicePtr)dev;

	if( fd->in < 0 ) {
		print_error(_Tr("file device does not support read operations"));
		return (size_t)-1;
	}

	sz = CWT_MIN(sz, __cwtFileBytesAvailable(dev));
	CWT_ASSERT(ns->lseek(fd->in, fd->read_pos, SEEK_SET) >= 0L);

	/*FIXME: warning C4267: 'function' : conversion from 'size_t' to 'UINT', possible loss of data */
	br = ns->read(fd->in, buf, (UINT)sz);

	CWT_ASSERT(br >= 0);
	fd->read_pos += br;

	return br;
}

ssize_t __cwtFileWrite(CwtIODevicePtr dev, const BYTE* buf, size_t sz)
{
	CwtUnistdNS *ns = cwtUnistdNS();
	CwtFileDevicePtr fd;
	ssize_t bw;

	CWT_ASSERT(dev);
	fd = (CwtFileDevicePtr)dev;

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

void __cwtFileReadBegin(CwtIODevicePtr dev)
{
	CwtFileDevicePtr fd;
	CWT_ASSERT(dev);

	fd = (CwtFileDevicePtr)dev;
	fd->read_pos_saved = fd->read_pos;
}

void __cwtFileReadCommit(CwtIODevicePtr dev)
{
	CwtFileDevicePtr fd;
	CWT_ASSERT(dev);

	fd = (CwtFileDevicePtr)dev;
	fd->read_pos_saved = fd->read_pos;
}

void __cwtFileReadRollback(CwtIODevicePtr dev)
{
	CwtFileDevicePtr fd;
	CWT_ASSERT(dev);

	fd = (CwtFileDevicePtr)dev;
	fd->read_pos = fd->read_pos_saved;
}

