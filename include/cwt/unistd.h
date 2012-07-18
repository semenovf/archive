/*
 * io.h
 *
 *  Created on: 26.06.2012
 *      Author: user
 */

#ifndef __CWT_IO_H__
#define __CWT_IO_H__

#include <cwt/types.h>

#ifdef CWT_CC_GNUC
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <fcntl.h>
#else
#	include <io.h>
#	if defined(CWT_OS_DOS)
#		include <dos.h>
#	endif
#endif


typedef struct CwtUnistdNS {
	INT  (*open)     (const CWT_CHAR *filename, INT oflag, ...);
	INT  (*close)    (int fd);
	INT  (*read)     (int fd, void *buffer, UINT count );
	INT  (*write)    (int fd, const void *buffer, UINT count );
	LONG (*lseek)    (int fd, LONG offset, INT origin);
#ifdef __SUPPORT_SHARING__
	INT  (*sopen)    (const CWT_CHAR *filename, INT oflag, INT shflag, ...);
#endif

	void (*sleep)(UINT sec);
	void (*msleep)(UINT msec);
	void (*usleep)(UINT sec);

} CwtUnistdNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtUnistdNS* cwtUnistdNS(void);


EXTERN_C_END

#endif /* __CWT_UNISTD_H__ */
