/*
 * string.c
 *
 *  Created on: Jul 27, 2013
 *      Author: wladt
 */

#include "../include/cwt/string.h"
#include <stdlib.h>
#include <errno.h>

CWT_NS_BEGIN

static long_t __str_to_long_helper(const char *s, bool *pok, int base, long_t min_val, long_t max_val)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
#ifdef CWT_HAVE_INT64
	long_t r = strtoll(s, & endptr, base);
#else
	long_t r = strtol(s, & endptr, base);
#endif

	if ((errno == ERANGE && (r == CWT_LONG_MAX || r == CWT_LONG_MIN))
			|| (errno != 0 && r == (long_t)0)
			|| endptr == s
			|| *endptr != '\0'
			|| r < min_val || r > max_val) {
		r = (long_t)0;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}


static ulong_t	__str_to_ulong_helper(const char *s, bool *pok, int base, ulong_t max_val)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
#ifdef CWT_HAVE_INT64
	ulong_t r = strtoull(s, & endptr, base);
#else
	ulong_t r = strtoul(s, & endptr, base);
#endif

	if ((errno == ERANGE && r == CWT_ULONG_MAX)
			|| (errno != 0 && r == (ulong_t)0)
			|| endptr == s
			|| *endptr != '\0'
			|| r > max_val ) {
		r = (ulong_t)0;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}


DLL_API double cwt_str_to_double (const char * s, bool *pok)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
	double r = strtod(s, & endptr);

	if (errno == ERANGE
			|| endptr == s
			|| *endptr != '\0' ) {
		r = (double)0.0f;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}

DLL_API float cwt_str_to_float (const char * s, bool *pok)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
	float r = strtof(s, & endptr);

	if (errno == ERANGE
			|| endptr == s
			|| *endptr != '\0' ) {
		r = (float)0.0f;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}

DLL_API long_t cwt_str_to_long (const char * s, bool *ok, int base)
{
	return __str_to_long_helper(s, ok, base, CWT_LONG_MIN, CWT_LONG_MAX);
}

DLL_API ulong_t	cwt_str_to_ulong (const char * s, bool *ok, int base)
{
	return __str_to_ulong_helper(s, ok, base, CWT_ULONG_MAX);
}

DLL_API int_t cwt_str_to_int (const char * s, bool *ok, int base)
{
	return (int_t)__str_to_long_helper(s, ok, base, (long_t)CWT_INT_MIN, (long_t)CWT_INT_MAX);
}

DLL_API uint_t cwt_str_to_uint (const char * s, bool *ok, int base)
{
	return (uint_t)__str_to_ulong_helper(s, ok, base, (ulong_t)CWT_UINT_MAX);
}

DLL_API short_t	 cwt_str_to_short  (const char * s, bool *ok, int base)
{
	return (short_t)__str_to_long_helper(s, ok, base, (long_t)CWT_SHORT_MIN, (long_t)CWT_SHORT_MAX);
}

DLL_API ushort_t cwt_str_to_ushort (const char * s, bool *ok, int base)
{
	return (ushort_t)__str_to_ulong_helper(s, ok, base, (ulong_t)CWT_USHORT_MAX);
}

DLL_API sbyte_t  cwt_str_to_sbyte (const char * s, bool *ok, int base)
{
	return (sbyte_t)__str_to_long_helper(s, ok, base, (long_t)CWT_SBYTE_MIN, (long_t)CWT_SBYTE_MAX);
}

DLL_API byte_t  cwt_str_to_byte (const char * s, bool *ok, int base)
{
	return (byte_t)__str_to_ulong_helper(s, ok, base, (ulong_t)CWT_BYTE_MAX);
}

CWT_NS_END
