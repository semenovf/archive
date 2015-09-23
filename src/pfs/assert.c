/*
 * assert.c
 *
 *  Created on: Jun 18, 2015
 *      Author: wladt
 */

#include "pfs.h"
#include <errno.h>

/* TODO DEPRECATED */

bool pfs_verify (bool predicate, const char * prefix, const char * file, int line, const char * text)
{
	if (!predicate)
		fprintf(stderr, "%s(%s[%d]): %s\n", prefix, file, line, text);
	return predicate;
}

bool pfs_verify_errno (bool predicate, const char * prefix, const char * file, int line, const char * text)
{
	if (!predicate)
		fprintf(stderr, "%s(errno=%d, %s[%d]): %s\n", prefix, errno, file, line, text);
	return predicate;
}



