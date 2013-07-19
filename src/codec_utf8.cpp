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

/*
template class Utf8UcsCodec<uint16_t, 0xffff>;
template class Utf8UcsCodec<uint32_t, 0x10ffff>;
*/

template ssize_t Utf8Ucs2Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
template ssize_t Utf8Ucs4Codec::convert(dest_char_type output[], size_t osize, const orig_char_type input[], size_t isize, size_t * remain);
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

/*
	if (uc == 0xFEFF) {
		return 0; // skip the BOM
	}
*/

	return need;
}


template <typename T, uint32_t MaxCP>
ssize_t Utf8UcsCodec<T, MaxCP>::convert(T output[], size_t osize, const char input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	if (!osize || !isize)
		return 0;

	const char *pbytes = input;
	const char *pbytesEnd = input + isize;
	T *pchars = output;
	T *pcharsEnd = output + osize;
	*remain = isize;

	if (!m_headerDone) {
		if (isize == 1 && byte_t(input[0]) == 0xef) {
			return 0;
		} if (isize == 2 && byte_t(input[0]) == 0xef && byte_t(input[1]) == 0xbb) {
			return 0;
		} if (isize >= 3 && byte_t(input[0]) == 0xef && byte_t(input[1]) == 0xbb && byte_t(input[2]) == 0xbf) {
			// starts with a byte order mark
			pbytes += 3;
		}
	}
	m_headerDone = true;

	for (; pbytes < pbytesEnd && pchars < pcharsEnd; ++pbytes) {
		uint32_t uc = 0;
		uint32_t min_uc = 0; // for 'Overlong' encodings recognition
		int n = __utf8_decode_bytes(pbytes, size_t(pbytesEnd - pbytes), uc, min_uc);

		// Code units too few
		if (n == -2) {
			break;
		}

		// Error, add replacement char, one byte will be skipped.
		if (n == -1) {
			*pchars++ = m_replacementChar;
			++m_invalidCount;
			continue;
		}

		// Skip the BOM
		if (uc == 0xFEFF) {
			pbytes += n; // n = 2 for BOM (0xEF, 0xBB, 0xBF)
			continue;
		}

		if ((uc < min_uc)                        // overlong sequence
				|| Unicode::isSurrogate(uc)      // UTF16 surrogate
				|| Unicode::isNonCharacter(uc)   // non-character
				|| uc > MaxCodePoint) {          // non-character

			// As for n == -1
			*pchars++ = m_replacementChar;
			++m_invalidCount;
			continue;
		}

		*pchars++ = T(uc);
		pbytes += n;
	}

	*remain = size_t(pbytesEnd - pbytes);

	return ssize_t(pchars - output);
}

// Returns number of bytes need to encode the unicode char
static inline ssize_t __utf8_encode_ucs(char *utf8, size_t size, uint32_t ucs4)
{
	char *cursor = utf8;

    if (ucs4 < 0x80) {
    	if (size < 1)
    		return ssize_t(-1);
        *cursor++ = char(ucs4);
    } else if (ucs4 < 0x0800) {
    	if (size < 2)
    		return ssize_t(-1);
    	*cursor++ = 0xC0 | byte_t(ucs4 >> 6);
    	*cursor++ = 0x80 | byte_t(ucs4 & 0x3f);
    } else if (ucs4 < 0x10000) {
    	if (size < 3)
    		return ssize_t(-1);

    	*cursor++ = 0xE0 | byte_t(ucs4 >> 12);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(ucs4 & 0x3F);
    } else if (ucs4 < 0x200000) {
    	if (size < 4)
    		return ssize_t(-1);

    	*cursor++ = 0xF0 | byte_t(ucs4 >> 18);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(ucs4 & 0x3F);
    } else if (ucs4 < 0x4000000) {
    	if (size < 5)
    		return ssize_t(-1);

    	*cursor++ = 0xF8 | byte_t(ucs4 >> 24);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 18) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(ucs4 & 0x3F);
    } else if (ucs4 < 0x80000000) {
    	if (size < 6)
    		return ssize_t(-1);
    	*cursor++ = 0xFC | byte_t(ucs4 >> 30);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 24) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 18) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(ucs4 >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(ucs4 & 0x3F);
    }

    return size_t(cursor - utf8);
}

template <typename T, uint32_t MaxCP>
ssize_t UcsUtf8Codec<T, MaxCP>::convert(char output[], size_t osize, const T input[], size_t isize, size_t * remain)
{
	CWT_ASSERT(remain);

	char *pbytes = output;
	char *pbytesEnd = output + osize;
	const T *pchars = input;
	const T *pcharsEnd = input + isize;
	*remain = isize;

	if (!m_headerDone) {
		if (osize > 2) {
			*pbytes++ = 0xef;
			*pbytes++ = 0xbb;
			*pbytes++ = 0xbf;
			m_headerDone = true;
		} else {
			return 0;
		}
	}

	while (pbytes < pbytesEnd && pchars < pcharsEnd) {

		uint32_t ucs4 = uint32_t(*pchars);

	    if (Unicode::isNonCharacter(ucs4) || ucs4 > MaxCodePoint) {
	    	*pbytes++ = m_replacementChar;
	        ++m_invalidCount;
	        continue;
	    } else {
			ssize_t n = __utf8_encode_ucs(pbytes, size_t(pbytesEnd - pbytes), ucs4);
			if (n < 0) // not enough space
				break;
			pbytes += n;
	    }
	    ++pchars;
	}

	*remain = size_t(pcharsEnd - pchars);
	return ssize_t(pbytes - output);
}

} // namespace io

CWT_NS_END


