/*
 * string.c
 *
 *  Created on: 10 янв. 2015 г.
 *      Author: wladt
 */

#include <pfs/string.h>
#include <ctype.h>

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

char * pfs_long_to_string (long_t n, int base, int uppercase, char * buf, int bufsz)
{
	char * p = NULL;

	if (n < 0) {
		p = pfs_ulong_to_string((ulong_t)(n * -1), base, uppercase, buf, bufsz);
		*--p = '-';
	} else {
		p = pfs_ulong_to_string((ulong_t)n, base, uppercase, buf, bufsz);
	}

	return p;
}

char * pfs_ulong_to_string (ulong_t n, int base, int uppercase, char * buf, int bufsz)
{
	char * p = & buf[bufsz - 1];

	PFS_ASSERT(base > 1 && base <= 36);

	buf[bufsz - 1] = '\0';

	if (n) {
		while (n > 0) {
			*--p = digits[n % base];

			if (uppercase) {
				*p = toupper(*p);
			}

			n /= base;
		}
	} else { /* n == 0 */
		*--p = '0';
	}

	return p;
}

char * pfs_real_to_string (real_t n, char f, int prec, char * buf, int bufsz)
{
	char fmt[32];

	PFS_UNUSED(bufsz);

	if (prec)
#ifdef PFS_HAVE_LONG_DOUBLE
		PFS_ASSERT(sprintf(fmt, "%%.%dL%c", prec, f) > 0);
#else
		PFS_ASSERT(sprintf(fmt, "%%.%d%c", prec, f) > 0);
#endif
	else
#ifdef PFS_HAVE_LONG_DOUBLE
		PFS_ASSERT(sprintf(fmt, "%%L%c", f) > 0);
#else
		PFS_ASSERT(sprintf(fmt, "%%%c", f) > 0);
#endif

	PFS_ASSERT(sprintf(buf, fmt, n) > 0);
	return buf;
}


