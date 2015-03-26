/*
 * utf8string.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: wladt
 */

#include "pfs/utility.hpp"
#include "pfs/mbcs_string.hpp"
#include "pfs/byte_string.hpp"

template class DLL_API std::allocator<char>;
template class DLL_API std::basic_string<char>;

namespace pfs {

// see http://www.codesynthesis.com/~boris/blog/2010/01/18/dll-export-cxx-templates/
// Forward declaration to avoid
// `specialization of ‘static mbcs_string_impl<char>::const_pointer mbcs_string_impl<char>::decrement ...’
// after instantiation'
template <>
mbcs_string_impl<char>::const_pointer mbcs_string_impl<char>::decrement (
		const_pointer start, difference_type n, size_type * invalidCodeUnits);


/**
 * @brief Increments char pointer by @c n utf8 chars
 * @param start start position
 * @param n     number of utf8 chars
 * @return      incremented position in successful or @c nullptr if out of bounds or invalid char found.
 */
template <>
mbcs_string_impl<char>::const_pointer mbcs_string_impl<char>::increment (
		const_pointer start, difference_type n, size_type * invalidCodeUnits)
{
	size_t invalid = 0;

	if (n < 0)
		return mbcs_string_impl<char>::decrement(start, 0 - n, invalidCodeUnits);

	const char * p = start;

	while (n-- > 0) {

		byte_t ch = byte_t(*p);

		if (ch < 128) {
			++p;
		} else if ((ch & 0xE0) == 0xC0) {
			p += 2;
		} else if ((ch & 0xF0) == 0xE0) {
			p += 3;
		} else if ((ch & 0xF8) == 0xF0) {
			p += 4;
		} else if ((ch & 0xFC) == 0xF8) {
			p += 5;
		} else if ((ch & 0xFE) == 0xFC) {
			p += 6;
		} else {
			++invalid;
			++p;
		}
	}

	if (invalidCodeUnits)
		*invalidCodeUnits = invalid;

	return p;
}


/**
 * @brief Decrements char pointer by @c n utf8 chars
 * @param start start position
 * @param n     number of utf8 chars
 * @return      incremented position in successful or @c nullptr if out of bounds or invalid char found.
 */
template <>
mbcs_string_impl<char>::const_pointer mbcs_string_impl<char>::decrement (
		const_pointer start, difference_type n, size_type * invalidCodeUnits)
{
	size_t invalid = 0;

	if (n < 0)
		return increment(start, 0 - n, invalidCodeUnits);

	const char * p = start;

	while (n-- > 0) {
		if ((*(p - 1) & 0x80) == 0x00) {
			--p;
		} else if ((*(p - 2) & 0xE0) == 0xC0) {
			p -= 2;
		} else if ((*(p - 3) & 0xF0) == 0xE0) {
			p -= 3;
		} else if ((*(p - 4) & 0xF8) == 0xF0) {
			p -= 4;
		} else if ((*(p - 5) & 0xFC) == 0xF8) {
			p -= 5;
		} else if ((*(p - 6) & 0xFE) == 0xFC) {
			p -= 6;
		} else {
			++invalid;
			--p;
		}
	}

	if (invalidCodeUnits)
		*invalidCodeUnits = invalid;

	return p;
}

/**
 * @brief Calculates the length in utf8 chars between @c from and @c to pointers.
 * @param from start pointer
 * @param to   end pointer
 * @return length in utf8 chars between @c from and @c to pointers.
 */
template <>
mbcs_string_impl<char>::size_type mbcs_string_impl<char>::length (const_pointer from, const_pointer to, size_type * invalidCodeUnits)
{
	size_type r = 0;
	size_t invalid = 0;

	if (from > to)
		pfs_swap(from, to);

	const char * p = from;

	while (p < to) {

		byte_t ch = byte_t(*p);

		if (ch < 128) {
			++p;
		} else if ((ch & 0xE0) == 0xC0) {
			p += 2;
		} else if ((ch & 0xF0) == 0xE0) {
			p += 3;
		} else if ((ch & 0xF8) == 0xF0) {
			p += 4;
		} else if ((ch & 0xFC) == 0xF8) {
			p += 5;
		} else if ((ch & 0xFE) == 0xFC) {
			p += 6;
		} else {
			++invalid;
			++p;
		}
		++r;
	}

	if (invalidCodeUnits)
		*invalidCodeUnits = invalid;

	return r;
}

template <>
mbcs_string_impl<char>::size_type mbcs_string_impl<char>::size (const_pointer start, difference_type n, size_t * invalidCodeUnits)
{
	const char * p = increment(start, n, invalidCodeUnits);
	return (start > p)
			? start - p
			: p - start;
}

template <>
mbcs_string_impl<char>::difference_type mbcs_string_impl<char>::difference (const_pointer from, const_pointer to, size_type * invalidCodeUnits)
{
	int sign = 1;

	if (to < from) {
		pfs_swap(from, to);
		sign = -1;
	}

	size_type r = length(from, to, invalidCodeUnits);
	PFS_ASSERT(size_type(pfs::max_type<difference_type>()) >= r);

	return r * sign;
}


/**
 *
 * @param latin1
 * @param length
 * @param state
 * @return
 */
template <>
mbcs_string<char> mbcs_string<char>::fromLatin1 (const char * latin1, size_t n, ConvertState * state)
{
	if (!latin1)
		return mbcs_string<char>();

	mbcs_string<char> r;
	const char * end = latin1 + n;
	size_t invalidChars = 0;
	size_t len = 0;

	char replacement[7];
	size_t replacementSize = state
			? state->replacementChar.encode<char>(replacement, 7)
			: ucchar(ucchar::ReplacementChar).encode<char>(replacement, 7);

	impl_class * d = r.base_class::cast();

	while (latin1 < end) {
		if (*latin1 < 127) {
			d->append(latin1, 1);
			++len;
		} else {
			d->append(replacement, replacementSize);
			++invalidChars;
			++len;
		}
		++latin1;
	}

	if (state)
		state->invalidChars += invalidChars;

	d->_length = len;

	return r;
}

/**
 *
 * @param latin1
 * @param state
 * @return
 */
template <>
mbcs_string<char> mbcs_string<char>::fromLatin1 (const char * latin1, ConvertState * state)
{
	if (!latin1)
		return mbcs_string<char>();

	return fromLatin1(latin1, strlen(latin1), state);
}

// Forward declaration to avoid `specialization of ‘static pfs::mbcs_string<_CodeUnitT> ...’ after instantiation'
template <>
mbcs_string<char> mbcs_string<char>::fromUtf8 (const char * utf8, size_t size, ConvertState * state);


template <>
mbcs_string<char> mbcs_string<char>::fromUtf8 (const char * utf8, ConvertState * state)
{
	if (!utf8)
		return mbcs_string<char>();

	return fromUtf8(utf8, strlen(utf8), state);
}

/**
 * @param utf8 string expected in UTF-8 format
 * @param size size of @c utf8 in code points (bytes)
 * @param state ConvertState pointer to store state of conversion
 * @return result of conversion represented as UTF-8 string instance.
 */
template <>
mbcs_string<char> mbcs_string<char>::fromUtf8 (const char * utf8, size_t size, ConvertState * state)
{
	if (!utf8)
		return mbcs_string<char>();

	mbcs_string<char> r;
	const char * cursor = utf8;
	const char * end = utf8 + size;
	size_t invalidChars = 0;
	size_t nremain = 0;
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition
	size_type len = 0;

	char replacement[7];
	size_t replacementSize = state
			? state->replacementChar.encode<char>(replacement, 7)
			: ucchar(ucchar::ReplacementChar).encode<char>(replacement, 7);

	impl_class * d = r.base_class::cast();

	while (cursor < end) {
		uint32_t uc = 0;
		int n = ucchar::decode<char>(cursor, size_t(end - cursor), uc, min_uc);

		// skip the BOM
		if (n == 0) {
			cursor += 3;
			continue;
		}

		if (n == -1) { // error
			d->append(replacement, replacementSize);
			++invalidChars;
			++cursor;
			++len;
		} else if (n == -2) {
			if (state) {
				nremain = size_t(end - cursor);
			} else {
				for (size_t j = size_t(end - cursor); j > 0; --j) {
					d->append(replacement, replacementSize);
					++invalidChars;
					++len;
				}
			}
			cursor = end;
		} else {
			if (!ucchar::isValid(uc, min_uc)) {
				d->append(replacement, replacementSize);
				++invalidChars;
				++cursor;
				++len;
			} else {
				d->append(cursor, size_type(n));
				cursor += size_t(n);
				++len;
			}
		}
	}

	if (state) {
		state->invalidChars += invalidChars;
		state->nremain = nremain;
	}

	d->_length = len;

	return r;
}

template <>
mbcs_string<char> mbcs_string<char>::fromLatin1 (const pfs::byte_string & latin1, ConvertState * state)
{
	return fromLatin1(reinterpret_cast<const char *>(latin1.constData()), latin1.length(), state);
}

template <>
mbcs_string<char> mbcs_string<char>::fromUtf8 (const byte_string & utf8, ConvertState * state)
{
	return fromUtf8(reinterpret_cast<const char *>(utf8.constData()), utf8.length(), state);
}


} // pfs
