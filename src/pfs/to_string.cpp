/*
 * string.c
 *
 *  Created on: 10 янв. 2015 г.
 *      Author: wladt
 */

#include <pfs.hpp>
#include <ctype.h>

namespace pfs {

static const char digits_lower[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char digits_upper[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char * uintmax_to_string (uintmax_t n, int base, int uppercase, char * buf, int bufsz)
{
	char * p = & buf[bufsz - 1];

	PFS_ASSERT_RANGE(base >= 2 && base <= 36);

	buf[bufsz - 1] = '\0';

	if (n) {
		while (n > 0) {
			if (uppercase)
				*--p = digits_upper[n % base];
			else
				*--p = digits_lower[n % base];

			n /= base;
		}
	} else { /* n == 0 */
		*--p = '0';
	}

	return p;
}

char * intmax_to_string (intmax_t n, int base, int uppercase, char * buf, int bufsz)
{
	char * p = NULL;

	if (n < 0) {
		p = uintmax_to_string(static_cast<uintmax_t>(n * -1), base, uppercase, buf, bufsz);
		*--p = '-';
	} else {
		p = uintmax_to_string(static_cast<uintmax_t>(n), base, uppercase, buf, bufsz);
	}

	return p;
}

/**
 * @brief Convert floating point number to string.
 *
 * @param n Floating point number.
 * @param f Floating point format for output.
 * @param prec Floating point precision.
 * @param buf Pointer to buffer @c buf.
 * @param bufsz Buffer @c buf size.
 * @return Pointer to @c buf if conversion is successful
 *         or NULL if not enough space to store string
 *         representation of floating point number.
 *         In case of failure *bufsz contains final buffer size
 *         or actual number of characters written in success.
 */
char * real_to_string (real_t n, char f, int prec, char * buf, int * bufsz)
{
	PFS_ASSERT(bufsz);

	char fmt[32];

	if (prec) {
#ifdef PFS_HAVE_LONG_DOUBLE
		PFS_ASSERT(sprintf(fmt, "%%.%dL%c", prec, f) > 0);
#else
		PFS_ASSERT(sprintf(fmt, "%%.%d%c", prec, f) > 0);
#endif
	} else {
#ifdef PFS_HAVE_LONG_DOUBLE
		PFS_ASSERT(sprintf(fmt, "%%L%c", f) > 0);
#else
		PFS_ASSERT(sprintf(fmt, "%%%c", f) > 0);
#endif
	}


#if PFS_HAVE_SNPRINTF

	/* The functions snprintf() and vsnprintf() do not write  more  than  size  bytes
	 * (including the terminating null byte ('\0')).  If the output was truncated due
	 * to this limit then the return value is the number of characters (excluding the
	 * terminating  null  byte)  which would have been written to the final string if
	 * enough space had been available. Thus, a return value of size or  more  means
	 * that the output was truncated.
	 *
	 * The glibc implementation of the functions snprintf() and vsnprintf()  conforms
	 * to  the C99 standard, that is, behaves as described above, since glibc version
	 * 2.1.  Until glibc 2.0.6 they would return -1 when the output was truncated.
	 */

	// FIXME Need the recognition of GLIBC version.
	// Supporting modern behavior only now.
	int sz = snprintf(buf, *bufsz, fmt, n);

	// Truncated
	//
	if (sz >= *bufsz) {
		*bufsz = sz;
		return NULL;
	}

	*bufsz = sz;

#else
#	error "Need implementation of snprintf"
#endif

//#ifdef PFS_CC_MSC
//	// FIXME make checks as below code for PFS_CC_GNUC
//	PFS_ASSERT(_snprintf(buf, bufsz - 1, fmt, n) > 0);
//#endif

	return buf;
}

// XXX DEPRECATED, moved to pfs/cast/integral
/**
 * @brief Convert character to integer (according to radix from 2 to 36 including)
 *
 * @param latin1 Character to convert
 * @return Converted digit, or -1
 */
//int pfs_latin1_to_digit (char latin1)
//{
//	if (latin1 >= '0' && latin1 <= '9')
//		return latin1 - '0';
//
//	if (latin1 >= 'A' && latin1 <= 'Z')
//		return latin1 - 'A' + 10;
//
//	if (latin1 >= 'a' && latin1 <= 'z')
//		return latin1 - 'a' + 10;
//
//	return -1;
//}

} // pfs
