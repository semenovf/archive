/*
 * utf8string_s.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: wladt
 */

#include "utf8string_p.hpp"

CWT_NS_BEGIN

const Utf8String Utf8String::EndOfLine(1, UChar('\n'));

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

/**
 * @brief Decodes bytes sequence into Unicode code point.
 *
 * @param bytes  Bytes for decode.
 * @param len    Length of the bytes for decode.
 * @param uc     Resulting Unicode code point.
 * @param min_uc Minimal Unicode code point for overlong sequence recognition.
 * @return -1 if error,
 *         -2 if code units too few to decode,
 *         number of UTF-8 code units converted to Unicode code point minus 1.
 */
int Utf8String::decodeBytes(const char *bytes, size_t len, uint32_t & uc, uint32_t & min_uc)
{
	byte_t ch = byte_t(bytes[0]);
	int need = 0;
	min_uc = 0;
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


// Returns number of bytes need to encode the unicode char
//
ssize_t Utf8String::encodeUcs4(char * utf8, size_t size, uint32_t ucs4)
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


Utf8String Utf8String::fromUtf8 (const ByteArray &utf8, bool * pok, ConvertState * state)
{
	return fromUtf8(utf8.data(), utf8.size(), pok, state);
}


Utf8String Utf8String::fromUtf8 (const char * utf8, bool * pok, ConvertState * state)
{
	return fromUtf8(utf8, strlen(utf8), pok, state);
}

/**
 * @brief
 *
 * @param utf8
 * @param size
 * @param pok
 * @param state
 * @return
 *
 * @note Need call Utf8String::calculateLength() after series of call this method with non-null @c state argument.
 */
Utf8String Utf8String::fromUtf8 (const char * utf8, size_t size, bool * pok, ConvertState * state)
{
	Utf8String r;
	const char *cursor = utf8;
	const char *end = utf8 + size;
	bool ok = true;
	size_t invalidChars = 0;
	size_t nremain = 0;
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition

	char replacement[6];
	ssize_t replacementSize = Utf8String::encodeUcs4(replacement, 6
			, state ? uint32_t(state->replacementChar) : UChar::ReplacementChar);
	CWT_ASSERT(replacementSize >= 0);

	while (cursor < end) {
		uint32_t uc = 0;
		int n = Utf8String::decodeBytes(cursor, size_t(end - cursor), uc, min_uc);

		if (n == -1) { // error
			r.pimpl->append(replacement, replacementSize);
			ok = false;
			++invalidChars;
			++cursor;
		} else if (n == -2) {
			if (state) {
				nremain = size_t(end - cursor);
			} else {
				for (size_t j = size_t(end - cursor); j > 0; --j) {
					r.pimpl->append(replacement, replacementSize);
					++invalidChars;
				}
			}
			ok = false;
			cursor = end;
		} else {
			if (!UChar::isValid(uc, min_uc)) {
				r.pimpl->append(replacement, replacementSize);
				++invalidChars;
				ok = false;
				++cursor;
			} else {
				r.pimpl->append(cursor, size_t(n) + 1);
				cursor += size_t(n) + 1;
/*
				for (size_t j = 0; j <= size_t(n); ++j)
					(*r.pimpl)[i++] = *cursor++;
*/
			}
		}
	}

	if (pok)
		*pok = ok;

	if (state) {
		state->invalidChars += invalidChars;
		state->nremain = nremain;
	}

	r.calculateLength();

	return r;
}


Utf8String Utf8String::fromLatin1 (const ByteArray & latin1, bool * pok , ConvertState * state)
{
	return fromLatin1(latin1.data(), latin1.size(), pok, state);
}

static const int halfShift  = 10; /* used for shifting by 10 bits */
static const uint32_t halfBase = uint32_t(0x0010000);

Utf8String Utf8String::fromUtf16 (const uint16_t * utf16, size_t size, bool * pok, ConvertState * state)
{
	Utf8String r;
	size_t invalidChars = 0;
	size_t nremain = 0;
	bool ok = true;
	const uint16_t * source = utf16;
    const uint16_t * sourceEnd   = utf16 + size;

    uint32_t replacementChar = state ? uint32_t(state->replacementChar) : UChar::ReplacementChar;

    while (source < sourceEnd) {
    	uint32_t ch = *source++;

    	/* If we have a surrogate pair, convert to uint32_t first. */
    	if (UChar::isHiSurrogate(ch)) {
    		/* If the 16 bits following the high surrogate are in the source buffer... */
    		if (source < sourceEnd) {
    			uint32_t ch2 = *source;

    			/* If it's a low surrogate, convert to uint32_t. */
    			if (UChar::isLowSurrogate(ch2)) {
    				ch = ((ch - UChar::HiSurrogateStart) << halfShift)
            		+ (ch2 - UChar::LowSurrogateStart) + halfBase;
    				++source;
    			} else { /* it's an unpaired high surrogate */
    				ok = false;
    	    		++invalidChars;
    	    		ch = replacementChar;
    	    		++source;
    			}
    		} else { /* We don't have the 16 bits following the high surrogate. */
    			nremain = 1;
    			break;
    		}
    	} else if (UChar::isLowSurrogate(ch)) {
    		ok = false;
    		++invalidChars;
    		ch = replacementChar;
    	}

    	r.append(Utf8String(1, ch));
    }

	if (pok)
		*pok = ok;

	if (state) {
		state->invalidChars += invalidChars;
		state->nremain = nremain;
	}

    r.calculateLength();
	return r;
}

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
Utf8String Utf8String::fromLatin1 (const char * latin1, size_t length, bool * pok, ConvertState * state)
{
	Utf8String r;
	bool ok = true;
	const char *end = latin1 + length;
	size_t invalidChars = 0;

	char replacement[6];
	ssize_t replacementSize = Utf8String::encodeUcs4(replacement, 6
			, state ? uint32_t(state->replacementChar) : UChar::ReplacementChar);
	CWT_ASSERT(replacementSize >= 0);


	while (latin1 < end) {
		if (*latin1 < 127) {
			r.pimpl->append(1, *latin1);
		} else {
			r.pimpl->append(replacement, replacementSize);
			++invalidChars;
			ok = false;
		}
		++latin1;
	}

	if (pok)
		*pok = ok;

	if (state)
		state->invalidChars += invalidChars;

	r.calculateLength();
	return r;
}

Utf8String Utf8String::fromLatin1 (const char * latin1, bool * pok, ConvertState * state)
{
	return fromLatin1(latin1, strlen(latin1), pok, state);
}


CWT_NS_END
