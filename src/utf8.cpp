/*
 * utf8.cpp
 *
 *  Created on: 18 апр. 2015 г.
 *      Author: wladt
 */


#include "pfs/ucchar.hpp"

namespace pfs {

bool can_advance_utf8_char (const char * begin, const char * end)
{
	const char * p = begin;

	if (*p < 128)
		return end - begin > 0;
	else if ((*p & 0xE0) == 0xC0)
		return end - begin > 1;
	else if ((*p & 0xF0) == 0xE0)
		return end - begin > 2;
	else if ((*p & 0xF8) == 0xF0)
		return end - begin > 3;
	else if ((*p & 0xFC) == 0xF8)
		return end - begin > 4;
	else if ((*p & 0xFE) == 0xFC)
		return end - begin > 5;

	return false;
}

//
// TODO rewrite as advance_utf16_char
//
/**
 * @brief Encodes Unicode character to UTF8 representation.
 *
 * @param utf
 * @return Number of bytes required to store Unicode character encoded to UTF8.
 */
size_t ucchar::encodeUtf8 (char * utf, size_t sz)
{
	char * cursor = utf;

    if (_value < 0x80) {
    	PFS_ASSERT(sz >= 1);
        *cursor++ = char(_value);
    } else if (_value < 0x0800) {
    	PFS_ASSERT(sz >= 2);
    	*cursor++ = 0xC0 | byte_t(_value >> 6);
    	*cursor++ = 0x80 | byte_t(_value & 0x3f);
    } else if (_value < 0x10000) {
    	PFS_ASSERT(sz >= 3);
    	*cursor++ = 0xE0 | byte_t(_value >> 12);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    } else if (_value < 0x200000) {
    	PFS_ASSERT(sz >= 4);
    	*cursor++ = 0xF0 | byte_t(_value >> 18);
    	*cursor++ = 0x80 | (byte_t(_value >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    } else if (_value < 0x4000000) {
    	PFS_ASSERT(sz >= 5);
    	*cursor++ = 0xF8 | byte_t(_value >> 24);
    	*cursor++ = 0x80 | (byte_t(_value >> 18) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    } else if (_value < 0x80000000) {
    	PFS_ASSERT(sz >= 6);
    	*cursor++ = 0xFC | byte_t(_value >> 30);
    	*cursor++ = 0x80 | (byte_t(_value >> 24) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 18) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    }
    //*cursor = '\0';

    return size_t(cursor - utf);
}


int ucchar::decodeUtf8 (const char * units, size_t len)
{
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition
	int n = ucchar::decodeUtf8(units, len, _value, min_uc);
	return n > 0 && !ucchar::isValid(_value, min_uc) ? -1 : n;
}

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
 * @brief Decodes UTF8 bytes sequence into Unicode code point.
 *
 * @param units  code units to decode.
 * @param len    Length of the bytes for decode.
 * @param uc     Resulting Unicode code point.
 * @param min_uc Minimal Unicode code point for overlong sequence recognition.
 * @return -1 if error,
 *         -2 if code units too few to decode,
 *         number of UTF-8 code units converted to Unicode code point.
 */
int ucchar::decodeUtf8 (const char * units, size_t len, uint32_t & uc, uint32_t & min_uc)
{
	byte_t ch = byte_t(units[0]);
	int need = 0;
	min_uc = 0;
	uc = 0;

	if (ch < 128) {
		uc = uint32_t(ch);
		return 1;
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
		if ((byte_t(units[i]) & 0xC0) == 0x80) {
			uc = (uc << 6) | (byte_t(units[i]) & 0x3F);
		} else {
			return -1;
		}
	}

	if (uc == 0xFEFF) {
		return 0; // skip the BOM
	}

	return need + 1;
}

} // pfs
