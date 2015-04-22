/*
 * utf16string.cpp
 *
 *  Created on: Apr 18, 2015
 *      Author: wladt
 */

#include "pfs/utility.hpp"
#include "pfs/mbcs_string.hpp"
#include "pfs/byte_string.hpp"

template class DLL_API std::allocator<uint16_t>;
template class DLL_API std::basic_string<uint16_t>;

namespace pfs {

extern const uint16_t * advance_utf16_char (
          const uint16_t * begin
        , const uint16_t * end
        , size_t * pnremain
        , size_t * pinvalid
        , ucchar * pch);

extern const uint16_t * advance_utf16_char (
          const uint16_t * begin
        , ucchar * pch);

extern const uint16_t * backward_utf16_char (
          const uint16_t * rbegin
        , ucchar * pch);


// see http://www.codesynthesis.com/~boris/blog/2010/01/18/dll-export-cxx-templates/
// Forward declaration to avoid
// `specialization of ‘static mbcs_string_impl<uint16_t>::const_pointer mbcs_string_impl<uint16_t>::decrement ...’
// after instantiation'
template <>
mbcs_string_impl<uint16_t>::const_pointer mbcs_string_impl<uint16_t>::decrement (
		  const_pointer start
		, difference_type n);


/**
 * @brief Increments char pointer by @c n UTF16-encoded chars
 * @param start start position
 * @param n     number of utf16-encoded chars
 * @return      incremented position in successful or @c nullptr if out of bounds or invalid char found.
 *
 * @see https://www.ietf.org/rfc/rfc2781.txt (2.2 Decoding UTF-16)
 */
template <>
mbcs_string_impl<uint16_t>::const_pointer mbcs_string_impl<uint16_t>::increment (
		  const_pointer start
		, difference_type n)
{
	if (n < 0)
		return mbcs_string_impl<uint16_t>::decrement(start, 0 - n);

	const_pointer p = start;

	while (n-- > 0) {
		p = advance_utf16_char(p, nullptr);
	}

	return p;
}


/**
 * @brief Decrements char pointer by @c n utf16-encoded chars
 * @param start start position
 * @param n     number of utf16-encoded chars
 * @return      incremented position in successful or @c nullptr if out of bounds or invalid char found.
 */
template <>
mbcs_string_impl<uint16_t>::const_pointer mbcs_string_impl<uint16_t>::decrement (
		  const_pointer start
		, difference_type n)
{
	if (n < 0)
		return increment(start, 0 - n);

	const_pointer p = start;

	while (n-- > 0) {
		p = backward_utf16_char(p, nullptr);
	}

	return p;
}

/**
 * @brief Calculates the length in utf16-encoded chars between @c from and @c to pointers.
 * @param from start pointer
 * @param to   end pointer
 * @return length in utf16-encoded chars between @c from and @c to pointers.
 */
template <>
mbcs_string_impl<uint16_t>::size_type mbcs_string_impl<uint16_t>::length (
		  const_pointer from
		, const_pointer to)
{
	size_type r = 0;

	if (from > to)
		pfs_swap(from, to);

	const_pointer p = from;

	while (p < to) {
		p = advance_utf16_char(p, nullptr);
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
mbcs_string<uint16_t> mbcs_string<uint16_t>::fromLatin1 (
		  const char * latin1
		, size_t n
		, ConvertState * state)
{
	if (!latin1)
		return mbcs_string<uint16_t>();

	mbcs_string<uint16_t> r;
	mbcs_string<uint8_t> s(mbcs_string<uint8_t>::fromLatin1(latin1, n, state));

	mbcs_string<uint8_t>::const_iterator it = s.cbegin();
	mbcs_string<uint8_t>::const_iterator itEnd = s.cend();

	for (; it != itEnd; ++it) {
		ucchar ch = *it;
		r.append(1, ch);
	}

	return r;
}

/**
 *
 * @param latin1
 * @param state
 * @return
 */
template <>
mbcs_string<uint16_t> mbcs_string<uint16_t>::fromLatin1 (
		  const char * latin1
		, ConvertState * state)
{
	if (!latin1)
		return mbcs_string<uint16_t>();

	return fromLatin1(latin1, strlen(latin1), state);
}

template <>
mbcs_string<uint16_t> mbcs_string<uint16_t>::fromLatin1 (
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
mbcs_string<uint16_t> mbcs_string<uint16_t>::fromUtf8 (
		  const char * utf8
		, size_t size
		, ConvertState * state)
{
	if (!utf8)
		return mbcs_string<uint16_t>();

	mbcs_string<uint16_t> r;
	mbcs_string<uint8_t> s(mbcs_string<uint8_t>::fromUtf8(utf8, size, state));

	mbcs_string<uint8_t>::const_iterator it = s.cbegin();
	mbcs_string<uint8_t>::const_iterator itEnd = s.cend();

	for (; it != itEnd; ++it) {
		ucchar ch = *it;
		r.append(1, ch);
	}

	return r;
}

template <>
mbcs_string<uint16_t> mbcs_string<uint16_t>::fromUtf8 (
		  const char * utf8
		, ConvertState * state)
{
	if (!utf8)
		return mbcs_string<uint16_t>();

	return fromUtf8(utf8, strlen(utf8), state);
}

template <>
mbcs_string<uint16_t> mbcs_string<uint16_t>::fromUtf8 (
		  const byte_string & utf8
		, ConvertState * state)
{
	return fromUtf8(reinterpret_cast<const char *>(utf8.constData()), utf8.length(), state);
}

template <>
mbcs_string<uint16_t> mbcs_string<uint16_t>::fromUtf16 (
          const uint16_t * utf16
        , size_t size
        , ConvertState * state)
{
    if (!utf16)
        return mbcs_string<uint16_t>();

    mbcs_string<uint16_t> r;
    const uint16_t * cursor = reinterpret_cast<const uint16_t *>(utf16);
    const uint16_t * end = cursor + size;
    size_t invalidChars = 0;
    size_t nremain = 0;
    size_type len = 0;

    uint16_t replacement[2];
    uint16_t encodedChar[2];
    size_t replacementSize = state
            ? state->replacementChar.encode<uint16_t>(replacement, 2)
            : ucchar(ucchar::ReplacementChar).encode<uint16_t>(replacement, 2);

    impl_class * d = r.base_class::cast();

    while (cursor < end) {
        ucchar ch;

        cursor = advance_utf16_char(
                  cursor
                , end
                , & nremain
                , & invalidChars
                , & ch);

        if (ch.isValid()) {
            // skip the BOM
            if (ch == ucchar::BomChar)
                continue;

            size_t sz = ch.encodeUtf16(encodedChar, 2);
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
mbcs_string<uint8_t> mbcs_string<uint16_t>::toUtf8 () const
{
	mbcs_string<uint8_t> r;

	mbcs_string<uint16_t>::const_iterator it = this->cbegin();
	mbcs_string<uint16_t>::const_iterator itEnd = this->cend();

	for (; it != itEnd; ++it) {
		ucchar ch = *it;
		r.append(1, ch);
	}

	return r;
}

template <>
mbcs_string<uint16_t> mbcs_string<uint16_t>::toUtf16 () const
{
	return *this;
}

} // pfs
