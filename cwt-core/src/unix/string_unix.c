/**
 * @file   system.c
 * @author wladt
 * @date   Apr 11, 2013 11:37:39 AM
 *
 * @brief
 */

#include <string.h>
#include "../../include/cwt/string.h"
#include "../../include/cwt/mt.h"

DLL_API char* cwt_strerror(int errn, char *buf, size_t buflen)
{
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
	strerror_r(errn, buf, buflen);
#else
	CWT_LOCK;
	char *s = strerror(errn);
	size_t len = strlen(len);
	strncpy(buf, s, CWT_MIN(buflen-1, len));
	CWT_UNLOCK;
#endif

	return buf;
}


