/**
 * @file utf8string_static.cpp
 *
 * Created on: Nov 25, 2013
 * Author: wladt
 */

#include "utf8string_p.hpp"
#include <cstring>

namespace pfs {

/**
 *
 * @param latin1
 * @param length
 * @param pok
 * @param state
 * @return
 *
 * @note Need call Utf8String::calculateLength() after series of call this method with non-null @c state argument.
 */
utf8string utf8string::fromLatin1 (const char * latin1, size_t length, ConvertState * state)
{
	utf8string r;
	const char * end = latin1 + length;
	size_t invalidChars = 0;

	char replacement[7];
	size_t replacementSize = state
			? state->replacementChar.encodeUtf8(replacement)
			: ucchar(ucchar::ReplacementChar).encodeUtf8(replacement);

	while (latin1 < end) {
		if (*latin1 < 127) {
			r._pimpl->append(1, *latin1);
		} else {
			r._pimpl->append(replacement, replacementSize);
			++invalidChars;
		}
		++latin1;
	}

	if (state)
		state->invalidChars += invalidChars;

	r.updateLength();
	return r;
}

utf8string utf8string::fromLatin1 (const char * latin1, ConvertState * state)
{
	return fromLatin1(latin1, strlen(latin1), state);
}

utf8string utf8string::fromUtf8 (const char * utf8, ConvertState * state)
{
	return fromUtf8(utf8, strlen(utf8), state);
}

/**
 *
 * @param utf8
 * @param size
 * @param pok
 * @param state
 * @return
 */
utf8string utf8string::fromUtf8 (const char * utf8, size_t size, ConvertState * state)
{
	utf8string r;
	const char *cursor = utf8;
	const char *end = utf8 + size;
	size_t invalidChars = 0;
	size_t nremain = 0;
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition

	char replacement[7];
	size_t replacementSize = state
			? state->replacementChar.encodeUtf8(replacement)
			: ucchar(ucchar::ReplacementChar).encodeUtf8(replacement);

	while (cursor < end) {
		uint32_t uc = 0;
		int n = ucchar::decodeUtf8(cursor, size_t(end - cursor), uc, min_uc);

		if (n == -1) { // error
			r._pimpl->append(replacement, replacementSize);
			++invalidChars;
			++cursor;
		} else if (n == -2) {
			if (state) {
				nremain = size_t(end - cursor);
			} else {
				for (size_t j = size_t(end - cursor); j > 0; --j) {
					r._pimpl->append(replacement, replacementSize);
					++invalidChars;
				}
			}
			cursor = end;
		} else {
			if (!ucchar::isValid(uc, min_uc)) {
				r._pimpl->append(replacement, replacementSize);
				++invalidChars;
				++cursor;
			} else {
				r._pimpl->append(cursor, size_t(n) + 1);
				cursor += size_t(n) + 1;
			}
		}
	}

	if (state) {
		state->invalidChars += invalidChars;
		state->nremain = nremain;
	}

	r.updateLength();

	return r;
}


static const int __halfShift  = 10; /* used for shifting by 10 bits */
static const uint32_t __halfBase = uint32_t(0x0010000);

utf8string utf8string::fromUtf16 (const uint16_t * utf16, size_t size, ConvertState * state)
{
	utf8string r;
	size_t invalidChars = 0;
	size_t nremain = 0;
	const uint16_t * source = utf16;
	const uint16_t * sourceEnd   = utf16 + size;

	uint32_t replacementChar = state ? uint32_t(state->replacementChar) : ucchar::ReplacementChar;

	while (source < sourceEnd) {
		uint32_t ch = *source++;

		/* If we have a surrogate pair, convert to uint32_t first. */
		if (ucchar::isHiSurrogate(ch)) {
			/* If the 16 bits following the high surrogate are in the source buffer... */
			if (source < sourceEnd) {
				uint32_t ch2 = *source;

				/* If it's a low surrogate, convert to uint32_t. */
				if (ucchar::isLowSurrogate(ch2)) {
					ch = ((ch - ucchar::HiSurrogateStart) << __halfShift)
					+ (ch2 - ucchar::LowSurrogateStart) + __halfBase;
					++source;
				} else { /* it's an unpaired high surrogate */
					++invalidChars;
					ch = replacementChar;
					++source;
				}
			} else { /* We don't have the 16 bits following the high surrogate. */
				nremain = 1;
				break;
			}
		} else if (ucchar::isLowSurrogate(ch)) {
			++invalidChars;
			ch = replacementChar;
		}

		r.append(utf8string(1, ch));
	}

	if (state) {
		state->invalidChars += invalidChars;
		state->nremain = nremain;
	}

	r.updateLength();

	return r;
}

#ifdef __COMMENT__

const Utf8String Utf8String::EndOfLine(1, UChar('\n'));

/**
 * @brief Returns string with escaping characters specified by @c escaped array.
 *
 * @param str     Source string.
 * @param escaped Array of characters need to escape.
 * @param nescaped Number of characters in @c escaped.
 * @return String with escaping specified characters.
 */
Utf8String Utf8String::escape (const Utf8String & str, const UChar escaped[], size_t nescaped)
{
	Utf8String r;
	r.pimpl->reserve(str.size());

	Utf8String::const_iterator it = str.cbegin();
	Utf8String::const_iterator itEnd = str.cend();

	while (it != itEnd) {
		for (size_t i = 0; i < nescaped; ++i) {
			if (*it == escaped[i]) {
				r.append(1, UChar('\\'));
				r.append(1, *it);
				break;
			}
		}

		++it;
	}

	return r;
}

/**
 * @brief Returns string with escaping special characters.
 *
 * @details Escaped characters are:
 * 		@arg &#92;b (bell), &#92;t (horizontal tab), &#92;v (vertical tab),
 * 		@arg &#92;n (line feed), &#92;r (carriage return)
 * 		@arg &#34; (quotation mark), &#39; (apostrophe)
 * 		@arg &#92; (backslash)
 *
 * @param str Source string.
 * @return String with escaping special characters.
 */
Utf8String Utf8String::escape (const Utf8String & str)
{
	Utf8String r;
	static UChar __replacement_chars[] = {
		    0,   1,  2,   3,  4,  5,  6, 'b',  8, 't'
		, 'n', 'v', 12, 'r', 14, 15, 16,  17, 18,  19
		,  20,  21, 22,  23, 24, 25, 26,  27, 28,  29
		,  30,  31
	};

	r.pimpl->reserve(str.size());

	Utf8String::const_iterator it = str.cbegin();
	Utf8String::const_iterator itEnd = str.cend();

	while (it != itEnd) {
		if ((*it).unicode() < 32) {
			r.append(1, UChar('\\'));
			r.append(1, __replacement_chars[(*it).unicode()]);

		} else if ( *it == UChar('\\')
				|| *it == UChar('\'')
				|| *it == UChar('"')) {

			r.append(1, UChar('\\'));
			r.append(1, *it);
		} else {
			r.append(1, *it);
		}
		++it;
	}

	return r;
}

#endif

} // pfs
