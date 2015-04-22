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

extern const uint8_t * advance_utf8_char (
          const uint8_t * begin
        , const uint8_t * end
        , uint32_t & min_uc
        , size_t * pnremain
        , size_t * pinvalid
        , ucchar * pch);

extern const uint8_t * advance_utf8_char (
          const uint8_t * begin
        , ucchar * pch);

extern const uint8_t * backward_utf8_char (
          const uint8_t * rbegin
        , ucchar * pch);



// see http://www.codesynthesis.com/~boris/blog/2010/01/18/dll-export-cxx-templates/
// Forward declaration to avoid
// `specialization of ‘static mbcs_string_impl<uint8_t>::const_pointer mbcs_string_impl<uint8_t>::decrement ...’
// after instantiation'
template <>
mbcs_string_impl<uint8_t>::const_pointer mbcs_string_impl<uint8_t>::decrement (
		  const_pointer start
		, difference_type n);


/**
 * @brief Increments char pointer by @c n utf8-encoded chars
 * @param start start position
 * @param n     number of utf8-encoded chars
 * @return      incremented position in successful or @c nullptr if out of bounds or invalid char found.
 */
template <>
mbcs_string_impl<uint8_t>::const_pointer mbcs_string_impl<uint8_t>::increment (
		  const_pointer start
		, difference_type n)
{
    if (n < 0)
        return mbcs_string_impl<uint8_t>::decrement(start, 0 - n);

    const_pointer p = start;

    while (n-- > 0) {
        p = advance_utf8_char(p, nullptr);
    }

    return p;
}


/**
 * @brief Decrements char pointer by @c n utf8-encoded chars
 * @param start start position
 * @param n     number of utf8-encoded chars
 * @return      incremented position in successful or @c nullptr if out of bounds or invalid char found.
 */
template <>
mbcs_string_impl<uint8_t>::const_pointer mbcs_string_impl<uint8_t>::decrement (
		  const_pointer start
		, difference_type n)
{
    if (n < 0)
        return increment(start, 0 - n);

    const_pointer p = start;

    while (n-- > 0) {
        p = backward_utf8_char(p, nullptr);
    }

    return p;
}

/**
 * @brief Calculates the length in utf8 chars between @c from and @c to pointers.
 * @param from start pointer
 * @param to   end pointer
 * @return length in utf8 chars between @c from and @c to pointers.
 */
template <>
mbcs_string_impl<uint8_t>::size_type mbcs_string_impl<uint8_t>::length (
          const_pointer from
        , const_pointer to)
{
    size_type r = 0;

    if (from > to)
        pfs_swap(from, to);

    const_pointer p = from;

    while (p < to) {
        p = advance_utf8_char(p, nullptr);
        ++r;
    }

    return r;
}

/**
 *
 * @param latin1
 * @param length
 * @param state
 * @return
 */
template <>
mbcs_string<uint8_t> mbcs_string<uint8_t>::fromLatin1 (
          const char * latin1
        , size_t n
        , ConvertState * state)
{
	if (!latin1)
		return mbcs_string<uint8_t>();

	mbcs_string<uint8_t> r;
	const char * end = latin1 + n;
	size_t invalidChars = 0;
	size_t len = 0;

	uint8_t replacement[7];
	size_t replacementSize = state
			? state->replacementChar.encode<uint8_t>(replacement, 7)
			: ucchar(ucchar::ReplacementChar).encode<uint8_t>(replacement, 7);

	impl_class * d = r.base_class::cast();

	while (latin1 < end) {
		if (*latin1 < 127) {
			d->append(reinterpret_cast<const_data_pointer>(latin1), 1);
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
mbcs_string<uint8_t> mbcs_string<uint8_t>::fromLatin1 (
          const char * latin1
        , ConvertState * state)
{
	if (!latin1)
		return mbcs_string<uint8_t>();

	return fromLatin1(latin1, strlen(latin1), state);
}


template <>
mbcs_string<uint8_t> mbcs_string<uint8_t>::fromLatin1 (
          const pfs::byte_string & latin1
        , ConvertState * state)
{
    return fromLatin1(reinterpret_cast<const char *>(latin1.constData()), latin1.length(), state);
}


/**
 * @param utf8 string expected in UTF-8 format
 * @param size size of @c utf8 in code points (bytes)
 * @param state ConvertState pointer to store state of conversion
 * @return result of conversion represented as UTF-8 string instance.
 */
template <>
mbcs_string<uint8_t> mbcs_string<uint8_t>::fromUtf8 (
          const char * utf8
        , size_t size
        , ConvertState * state)
{
    if (!utf8)
        return mbcs_string<uint8_t>();

    mbcs_string<uint8_t> r;
    const uint8_t * cursor = reinterpret_cast<const uint8_t *>(utf8);
    const uint8_t * end = cursor + size;
    size_t invalidChars = 0;
    size_t nremain = 0;
    uint32_t min_uc = 0; // for 'Overlong' encodings recognition
    size_type len = 0;

    uint8_t replacement[6];
    uint8_t encodedChar[6];
    size_t replacementSize = state
            ? state->replacementChar.encode<uint8_t>(replacement, 6)
            : ucchar(ucchar::ReplacementChar).encode<uint8_t>(replacement, 6);

    impl_class * d = r.base_class::cast();

    while (cursor < end) {
        ucchar ch;

        cursor = advance_utf8_char (
                  cursor
                , end
                , min_uc
                , & nremain
                , & invalidChars
                , & ch);

        if (ch.isValid()) {
            // skip the BOM
            if (ch == ucchar::BomChar)
                continue;

            size_t sz = ch.encodeUtf8(encodedChar, 6);
            d->append(encodedChar, sz);
            ++len;
        } else {
            if (nremain > 0) {
                for (size_t j = nremain; j > 0; --j) {
                    d->append(replacement, replacementSize);
                    ++invalidChars;
                    ++len;

                }
                cursor += nremain;
                PFS_ASSERT(cursor >= end);
            } else {
                d->append(replacement, replacementSize);
                ++cursor;
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
mbcs_string<uint8_t> mbcs_string<uint8_t>::fromUtf8 (
          const char * utf8
        , ConvertState * state)
{
	if (!utf8)
		return mbcs_string<uint8_t>();

	return mbcs_string<uint8_t>::fromUtf8(utf8, strlen(utf8), state);
}


template <>
mbcs_string<uint8_t> mbcs_string<uint8_t>::fromUtf8 (
          const byte_string & utf8
        , ConvertState * state)
{
	return fromUtf8(reinterpret_cast<const char *>(utf8.constData()), utf8.length(), state);
}


template <>
mbcs_string<uint8_t> mbcs_string<uint8_t>::fromUtf16 (
          const uint16_t * utf16
        , size_t size
        , ConvertState * state)
{
    if (!utf16)
        return mbcs_string<uint8_t>();

    mbcs_string<uint8_t> r;
    mbcs_string<uint16_t> s(mbcs_string<uint16_t>::fromUtf16(utf16, size, state));

    mbcs_string<uint16_t>::const_iterator it = s.cbegin();
    mbcs_string<uint16_t>::const_iterator itEnd = s.cend();

    for (; it != itEnd; ++it) {
        ucchar ch = *it;
        r.append(1, ch);
    }

    return r;
}

template <>
mbcs_string<uint8_t> mbcs_string<uint8_t>::toUtf8 () const
{
	return *this;
}

template <>
mbcs_string<uint16_t> mbcs_string<uint8_t>::toUtf16 () const
{
	mbcs_string<uint16_t> r;

	mbcs_string<uint8_t>::const_iterator it = this->cbegin();
	mbcs_string<uint8_t>::const_iterator itEnd = this->cend();

	for (; it != itEnd; ++it) {
		ucchar ch = *it;
		r.append(1, ch);
	}

	return r;
}



} // pfs
