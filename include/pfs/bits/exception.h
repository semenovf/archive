/*
 * exception.h
 *
 *  Created on: Sep 23, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_EXCEPTION_H__
#define __PFS_BITS_EXCEPTION_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>

/* FIXME Need to implement thread-safe and platform independent of strerror call */

// XXX Deprecated
/*
 * System error exception
 */
#define PFS_THROW_SYSERR(xerrno)                            \
	if ((xerrno) != 0) {                                    \
		fprintf(stderr, "ERROR: (%s[%d]): %s (errno=%d)\n"  \
				, __TFILE__                                 \
				, __LINE__                                  \
				, strerror(errno)                           \
				, xerrno);                                  \
		::exit(-1);                                         \
	}

#endif /* __PFS_BITS_EXCEPTION_H__ */
