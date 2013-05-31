/**
 * @file   utf8codec.cpp
 * @author wladt
 * @date   Mar 1, 2013 11:16:36 AM
 *
 * @brief
 */

#include "utfcodec_p.hpp"

CWT_NS_BEGIN


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
static inline int utf8decoder_convert_bytes(const byte_t *bytes, size_t len, uint_t &uc, Char *remainChar)
{
	byte_t ch = bytes[0];
	uint_t min_uc = 0; // for 'Overlong' encodings recognition
	int need = 0;

	uc = 0;

	if (ch < 128) {
		uc = uint_t(ch);
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
		if ((bytes[i] & 0xC0) == 0x80) {
			uc = (uc << 6) | (bytes[i] & 0x3F);
		} else {
			return -1;
		}
	}

	if (uc == 0xFEFF) {
		return 0; // skip the BOM
	}

	if (!Char::isNonCharacter(uc) && Char::requiresSurrogates(uc) && uc <= Char::MaxCodePoint) {
		// surrogate pair
		uc = Char::hiSurrogate(uc);
		*remainChar = Char::lowSurrogate(uc);
	} else if ((uc < min_uc)              // overlong sequence
			|| Char::isSurrogate(uc)      // UTF16 surrogate
			|| Char::isNonCharacter(uc)   // non-character
			|| uc > Char::MaxCodePoint) { // non-character
		return -1;
	}

	return need;
}

String Utf8Decoder::convertToUnicode(const char *bytes, size_t len, size_t *nremainBytes)
{
	String s;
	size_t invalid = 0; // TODO not used out this method yet

	CWT_ASSERT(nremainBytes);
	//buflen = (ssize_t)(m_buffer.length());
	//const byte_t *bytes = (byte_t*)m_buffer.data();

	s.reserve(len);
	Char *pchars = s.data();
	size_t bufsz = 0;
	*nremainBytes = len;

	if (!m_headerDone) {
		if (len == 1 && byte_t(bytes[0]) == 0xef) {
			return String();
		} if (len == 2 && byte_t(bytes[0]) == 0xef && byte_t(bytes[1]) == 0xbb) {
			return String();
		} if (len >= 3 && byte_t(bytes[0]) == 0xef && byte_t(bytes[1]) == 0xbb && byte_t(bytes[2]) == 0xbf) {
			// starts with a byte order mark
			bytes += 3;
			len -= 3;
			*nremainBytes -= 3;
		}
	}
	m_headerDone = true;

	if (m_remainChar != Char::MaxCodePoint) {
		*pchars++ = m_remainChar;
		++bufsz;
		m_remainChar = Char::MaxCodePoint; // reset remain char
	}

	for (size_t i = 0; i < len && bufsz < len; ++i, --*nremainBytes) {

		uint_t uc = 0;
		int n = utf8decoder_convert_bytes(reinterpret_cast<const byte_t*>(bytes + i) , len-i, uc, &m_remainChar);

		if (n == -2) { // too few bytes
			break;
		} else if (n == -1) { // error
			uc = m_replacement.unicode();
			++invalid;
		} else if (uc == 0xFEFF) {
			continue; // skip the BOM
		} else {
			i += n;
			*nremainBytes -= n;
			if (uc > CWT_USHORT_MAX) {
				uc = m_replacement.unicode();
				++invalid;
			}
		}

		*pchars++ = uc;
		++bufsz;
	}

	s.resize(bufsz);
	return s;
}

ByteArray Utf8Encoder::convertFromUnicode(const Char *chars, size_t len)
{
	if (!len)
		return static_cast<ssize_t>(0);

	int invalid = 0;
	ByteArray bytes;
	bytes.resize(len * 3 + 3);
	byte_t *cursor = (byte_t*)bytes.data();

	if (!m_headerDone) {
		*cursor++ = 0xef;
		*cursor++ = 0xbb;
		*cursor++ = 0xbf;
		m_headerDone = true;
	}

	const Char *ch  = chars;
	const Char *end = ch + len;
	while (ch < end) {
	    uint u = ch->unicode();
	    if (m_hiSurrogate >= 0) {
	        if (ch->isLowSurrogate()) {
	            u = Char::surrogatePairToUcs4(m_hiSurrogate, u);
	            m_hiSurrogate = -1;
	        } else {
	            // high surrogate without low
	            *cursor = m_replacement;
	            ++ch;
	            ++invalid;
	            m_hiSurrogate = -1;
	            continue;
	        }
	    } else if (ch->isLowSurrogate()) {
	        // low surrogate without high
	        *cursor = m_hiSurrogate;
	        ++ch;
	        ++invalid;
	        continue;
	    } else if (ch->isHiSurrogate()) {
	    	m_hiSurrogate = u;
	        ++ch;
	        continue;
	    }

	    if (u < 0x80) {
	        *cursor++ = (byte_t)u;
	    } else {
	        if (u < 0x0800) {
	            *cursor++ = 0xc0 | ((byte_t) (u >> 6));
	        } else {
	            // is it one of the Unicode non-characters?
	            if (Char::isNonCharacter(u)) {
	                *cursor++ = m_replacement;
	                ++ch;
	                ++invalid;
	                continue;
	            }

	            if (Char::requiresSurrogates(u)) {
	                *cursor++ = 0xf0 | ((byte_t) (u >> 18));
	                *cursor++ = 0x80 | (((byte_t) (u >> 12)) & 0x3f);
	            } else {
	                *cursor++ = 0xe0 | (((byte_t) (u >> 12)) & 0x3f);
	            }
	            *cursor++ = 0x80 | (((byte_t) (u >> 6)) & 0x3f);
	        }
	        *cursor++ = 0x80 | ((byte_t) (u&0x3f));
	    }
	    ++ch;
	}

	bytes.resize(cursor - (const byte_t*)bytes.data());
	return bytes;
}

CWT_NS_END
