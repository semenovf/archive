/*
 * codec_utf8.cpp
 *
 *  Created on: Jul 18, 2013
 *      Author: wladt
 */


#include "../include/cwt/io/codec_utf8.hpp"
#include <cwt/unicode.hpp>

CWT_NS_BEGIN

namespace io {

template class Utf8UcsCodec<uint16_t, 0x0000, 0xfffd, 0xffff>;
template class Utf8UcsCodec<uint32_t, 0x0000, 0xfffd, 0x10ffff>;

template ssize_t Utf8UcsCodec<uint16_t, 0x0000, 0xfffd, 0xffff>::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
template ssize_t Utf8UcsCodec<uint32_t, 0x0000, 0xfffd, 0x10ffff>::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
template ssize_t Ucs2Utf8Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
template ssize_t Ucs4Utf8Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);

/* UTF-8
 *
 * Bits Last code point/ 	Byte 1 		Byte 2 		Byte 3 		Byte 4 		Byte 5 		Byte 6
 *  7 	U+007F 				0xxxxxxx
 *  11 	U+07FF 				110xxxxx 	10xxxxxx
 *  16 	U+FFFF 				1110xxxx 	10xxxxxx 	10xxxxxx
 *  21 	U+1FFFFF 			11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 *  26 	U+3FFFFFF 			111110xx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 *  31 	U+7FFFFFFF 			1111110x 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 */

// return
//		-1 if error.
// 		-2 if code units too few.
// 		number of UTF-8 code units converted to Unicode code point minus 1.
static inline int __utf8_decode_bytes(const char *bytes, size_t len, uint32_t & uc, uint32_t & min_uc)
{
	byte_t ch = byte_t(bytes[0]);
	int need = 0;

	uc = 0;

	if (ch < 128) {
		uc = uint32_t(ch);
		return 0;
	} else if ((ch & 0xE0) == 0xC0) {
		uc = ch & 0x1F;
		need = 1;
		min_uc = 0x80;
	} else if ((ch & 0xF0) == 0xE0) {
		uc = ch & 0x0F;
		need = 2;
		min_uc = 0x800;
	} else if ((ch & 0xF8) == 0xF0) {
		uc = ch & 0x07;
		need = 3;
		min_uc = 0x10000;
	} else if ((ch & 0xFC) == 0xF8) {
		uc = ch & 0x03;
		need = 4;
		min_uc = 0x200000;
	} else if ((ch & 0xFE) == 0xFC) {
		uc = ch & 0x01;
		need = 5;
		min_uc = 0x4000000;
	} else {
		return -1; // error
	}

	// too few bytes in the buffer
	if (size_t(need) >= len)
		return -2;

	for (int i = 1; i <= need; ++i) {
		if ((byte_t(bytes[i]) & 0xC0) == 0x80) {
			uc = (uc << 6) | (byte_t(bytes[i]) & 0x3F);
		} else {
			return -1;
		}
	}

	if (uc == 0xFEFF) {
		return 0; // skip the BOM
	}

	return need;
}


template <typename T, uint32_t NullChar, uint32_t ReplChar, uint32_t MaxCP>
ssize_t Utf8UcsCodec<T, NullChar, ReplChar, MaxCP>::convert(T output[], size_t osize, const char input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	T *pchars = output;
	ssize_t r = 0;
	*remain = isize;

	if (!m_headerDone) {
		if (isize == 1 && byte_t(input[0]) == 0xef) {
			return 0;
		} if (isize == 2 && byte_t(input[0]) == 0xef && byte_t(input[1]) == 0xbb) {
			return 0;
		} if (isize >= 3 && byte_t(input[0]) == 0xef && byte_t(input[1]) == 0xbb && byte_t(input[2]) == 0xbf) {
			// starts with a byte order mark
			input += 3;
			isize -= 3;
			*remain -= 3;
		}
	}
	m_headerDone = true;

	if (m_remainChar != MaxCodePoint) {
		*pchars++ = m_remainChar;
		++r;
		m_remainChar = MaxCodePoint; // reset remain char
	}

	for (size_t i = 0; i < isize && size_t(r) < osize ; ++i, --*remain) {
		uint32_t uc = 0;
		uint32_t min_uc = 0; // for 'Overlong' encodings recognition
		int n = __utf8_decode_bytes(input , isize, uc, min_uc);

		if (!Unicode::isNonCharacter(uc) && Unicode::requiresSurrogates(uc) && uc <= MaxCodePoint) {
			// surrogate pair
			uc = Unicode::hiSurrogate(uc);
			m_remainChar = Unicode::lowSurrogate(uc);
		} else if ((uc < min_uc)                 // overlong sequence
				|| Unicode::isSurrogate(uc)      // UTF16 surrogate
				|| Unicode::isNonCharacter(uc)   // non-character
				|| uc > MaxCodePoint) {          // non-character
			n = -1;
		}

		if (n == -2) { // too few bytes
			break;
		} else if (n == -1) { // error
			uc = m_replacementChar;
			++m_invalidCount;
		} else if (uc == 0xFEFF) {
			continue; // skip the BOM
		} else {
			i += n;
			*remain -= n;
			if (uc > MaxCodePoint) {
				uc = m_replacementChar;
				++m_invalidCount;
			}
		}

		*pchars++ = T(uc);
		++r;
	}

	return r;
}

static inline size_t __utf8_encode_ucs(char *utf8, uint32_t ucs4, char replacement, size_t & invalid)
{
	size_t r = 0;

    if (ucs4 < 0x80) {
        *utf8++ = char(ucs4);
        ++r;
    } else {
        if (ucs4 < 0x0800) {
            *utf8++ = 0xc0 | byte_t(ucs4 >> 6);
            ++r;
        } else {
            // is it one of the Unicode non-characters?
            if (Unicode::isNonCharacter(ucs4)) {
                *utf8++ = replacement;
                ++r;
                ++invalid;
                return r;
            }

            if (Unicode::requiresSurrogates(ucs4)) {
                *utf8++ = 0xf0 | byte_t(ucs4 >> 18);
                *utf8++ = 0x80 | (byte_t(ucs4 >> 12) & 0x3f);
                r += 2;
            } else {
                *utf8++ = 0xe0 | (byte_t(ucs4 >> 12) & 0x3f);
                ++r;
            }
            *utf8++ = 0x80 | (byte_t(ucs4 >> 6) & 0x3f);
            ++r;
        }
        *utf8++ = 0x80 | byte_t(ucs4 & 0x3f);
        ++r;
    }

    return r;
}

template <typename T>
ssize_t UcsUtf8Codec<T>::convert(char output[], size_t osize, const T input[], size_t isize, size_t * remain)
{
	ssize_t r  = 0;
	char *cursor = output;
	*remain = isize;

	if (!m_headerDone) {
		if (osize > 2) {
			*cursor++ = 0xef;
			*cursor++ = 0xbb;
			*cursor++ = 0xbf;
			m_headerDone = true;
			r += 3;
			osize -= 3;
		} else {
			return 0;
		}
	}

	const T *ch  = input;
	const T *end = ch + isize;

	while (ch < end && osize > 0) {
		char utf8[16];
	    size_t n = __utf8_encode_ucs(utf8, uint32_t(*ch), m_replacementChar, m_invalidCount);
	    CWT_ASSERT(n < 16);

	    if (n <= osize) {
	    	for (size_t i = 0; i < n; ++i)
	    		*cursor++ = utf8[i];
	    	r += n;
	    	osize -= n;
	    	--*remain;
	    	++ch;
	    } else {
	    	break;
	    }
	}

	return r;
}

} // namespace io

CWT_NS_END


