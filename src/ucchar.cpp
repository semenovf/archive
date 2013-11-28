/*
 * uchar.cpp
 *
 *  Created on: Aug 14, 2013
 *      Author: wladt
 */

#include "../include/pfs/ucchar.hpp"

namespace pfs {

bool ucchar::isSpace() const
{
// TODO need to use Unicode Database (UCD) to extend the notion of 'space' symbol.
	return _value == uint32_t(' ')
		|| _value == uint32_t('\t')
		|| _value == uint32_t('\n')
		|| _value == uint32_t('\v')
		|| _value == uint32_t('\f')
		|| _value == uint32_t('\r')
		? true : false;
}

// FIXME need full support of Unicode standard
ucchar ucchar::toLower () const
{
	if (_value < 128) {
		if (char(_value) >= 'A' && char(_value) <= 'Z')
			return ucchar(_value + 0x20);
	}
	return ucchar(_value);
}

// FIXME need full support of Unicode standard
ucchar ucchar::toUpper () const
{
	if (_value < 128) {
		if (char(_value) >= 'a' && char(_value) <= 'z')
			return ucchar(_value - 0x20);
	}
	return ucchar(_value);
}

/**
 * @brief Encodes Unicode character to UTF8 representation.
 *
 * @param utf8
 * @return Number of bytes required to store Unicode character encoded to UTF8.
 */
size_t ucchar::encodeUtf8 (char utf8[7])
{
	char * cursor = utf8;

    if (_value < 0x80) {
        *cursor++ = char(_value);
    } else if (_value < 0x0800) {
    	*cursor++ = 0xC0 | byte_t(_value >> 6);
    	*cursor++ = 0x80 | byte_t(_value & 0x3f);
    } else if (_value < 0x10000) {
    	*cursor++ = 0xE0 | byte_t(_value >> 12);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    } else if (_value < 0x200000) {
    	*cursor++ = 0xF0 | byte_t(_value >> 18);
    	*cursor++ = 0x80 | (byte_t(_value >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    } else if (_value < 0x4000000) {
    	*cursor++ = 0xF8 | byte_t(_value >> 24);
    	*cursor++ = 0x80 | (byte_t(_value >> 18) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    } else if (_value < 0x80000000) {
    	*cursor++ = 0xFC | byte_t(_value >> 30);
    	*cursor++ = 0x80 | (byte_t(_value >> 24) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 18) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 12) & 0x3F);
    	*cursor++ = 0x80 | (byte_t(_value >> 6)  & 0x3F);
    	*cursor++ = 0x80 | byte_t(_value & 0x3F);
    }
    *cursor = '\0';

    return size_t(cursor - utf8);
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
 *         number of UTF-8 code units converted to Unicode code point minus 1.
 */
int ucchar::decodeUtf8(const char * units, size_t len, uint32_t & uc, uint32_t & min_uc)
{
	byte_t ch = byte_t(units[0]);
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
		if ((byte_t(units[i]) & 0xC0) == 0x80) {
			uc = (uc << 6) | (byte_t(units[i]) & 0x3F);
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
} // pfs

