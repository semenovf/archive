/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   May 15, 2013
 *         Oct  6, 2014
 *
 * @brief
 */

#include "pfs/bytearray.hpp"
#include <cstdlib>
#include <cerrno>
#include <sstream>
#include <iomanip>
#include <string>

extern "C" real_t pfs_strtoreal (const char * arg_string, char decimalPoint, char ** arg_endptr);

namespace pfs {

const bytearray::char_type bytearray::TerminatorChar('\0');
const bytearray::char_type bytearray::EndOfLineChar('\n');

static long_t __str_to_long_helper (const char * s, bool * pok, int base, long_t min_val, long_t max_val)
{
	PFS_ASSERT(s);
	bool ok = true;
	char * endptr = nullptr;

	errno = 0;
	long_t r = strtolong(s, & endptr, base);

	if ((errno == ERANGE && (r == PFS_LONG_MAX || r == PFS_LONG_MIN))
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


static ulong_t	__str_to_ulong_helper (const char * s, bool * pok, int base, ulong_t max_val)
{
	PFS_ASSERT(s);
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
	ulong_t r = strtoulong(s, & endptr, base);

	if ((errno == ERANGE && r == PFS_ULONG_MAX)
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

// Decimal point in standard 'strtod' function is locale-dependent.
// Usually it is '.', but may be ',' (comma) or other char symbol

static real_t __str_to_real (const char * s, bool * pok, char decimalPoint)
{
	PFS_ASSERT(s);
	bool ok = true;
	char * endptr = nullptr;

	errno = 0;
	real_t r = pfs_strtoreal(s, decimalPoint, & endptr);

	if (errno == ERANGE
			|| endptr == s
			|| *endptr != '\0' ) {
		r = real_t(0.0f);
		ok = false;
	}

	if (pok)
		*pok = ok;

	return r;
}

real_t bytearray::toReal (bool * ok, char decimalPoint) const
{
	return __str_to_real(isNull() ? "" : c_str(), ok, decimalPoint);
}

long_t bytearray::toLong (bool * ok, int base) const
{
	// FIXME c_str() must return empty string
	const char * s  = c_str();
	return __str_to_long_helper(s ? s : "", ok, base, PFS_LONG_MIN, PFS_LONG_MAX);
}

ulong_t bytearray::toULong (bool * ok, int base) const
{
	const char * s  = c_str();
	return __str_to_ulong_helper(s ? s : "", ok, base, PFS_ULONG_MAX);
}

int_t bytearray::toInt (bool * ok, int base) const
{
	const char * s  = c_str();
	return int_t(__str_to_long_helper(s ? s : "", ok, base, long_t(PFS_INT_MIN), long_t(PFS_INT_MAX)));
}

uint_t bytearray::toUInt (bool * ok, int base) const
{
	const char * s  = c_str();
	return uint_t(__str_to_ulong_helper(s ? s : "", ok, base, ulong_t(PFS_UINT_MAX)));
}

short_t	bytearray::toShort  (bool * ok, int base) const
{
	const char * s  = c_str();
	return short_t(__str_to_long_helper(s ? s : "", ok, base, long_t(PFS_SHORT_MIN), long_t(PFS_SHORT_MAX)));
}

ushort_t bytearray::toUShort (bool * ok, int base) const
{
	const char * s  = c_str();
	return ushort_t(__str_to_ulong_helper(s ? s : "", ok, base, ulong_t(PFS_USHORT_MAX)));
}

sbyte_t bytearray::toSByte (bool * ok, int base) const
{
	const char * s  = c_str();
	return sbyte_t(__str_to_long_helper(s ? s : "", ok, base, long_t(PFS_SBYTE_MIN), long_t(PFS_SBYTE_MAX)));
}

byte_t bytearray::toByte (bool * ok, int base) const
{
	const char * s  = c_str();
	return byte_t(__str_to_ulong_helper(s ? s : "", ok, base, ulong_t(PFS_BYTE_MAX)));
}

//template <>
//bytearray bytearray::toBytes<pfs::string> (const pfs::string & v, endian::type_enum /*order*/)
//{
//	return bytearray(v.constData(), v.sizeInBytes());
//}
//
///*
//DLL_API uint_t hash_func(const bytearray & key, uint_t seed)
//{
//	return hash_bytes(reinterpret_cast<const byte_t *>(key.constData()), key.size(), seed);
//}
//*/

} // pfs
