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

static inline int utf8decoder_fill_buffer(InputStream &is, ByteArray bytes, size_t maxSize, bool &atEnd)
{
	size_t buflen = bytes.length();
	atEnd = false;

	if (buflen < maxSize) {
		bytes.reserve(maxSize);

		ssize_t nbytes = is.read(bytes.data(), maxSize - buflen);
		if (nbytes < 0)
			return ssize_t(-1);

		if (!nbytes)
			atEnd = true;

		bytes.resize(int(buflen + nbytes));
	}

	return ssize_t(bytes.length());
}


// return
//		-1 if error.
// 		-2 if code units too few.
// 		number of UTF-8 code units converted to Unicode code point minus 1.
static inline int utf8decoder_convert_bytes(const byte_t *bytes, int len, uint_t &uc, Char &remainChar)
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
	if (need >= len)
		return -2;

	for (int i = 1; i < len; ++i) {
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
		remainChar = Char::lowSurrogate(uc);
	} else if ((uc < min_uc)              // overlong sequence
			|| Char::isSurrogate(uc)      // UTF16 surrogate
			|| Char::isNonCharacter(uc)   // non-character
			|| uc > Char::MaxCodePoint) { // non-character
		return -1;
	}

	return need;
}

ssize_t Utf8Decoder::read(Char chars[], size_t len)
{
	if(!len)
		return 0;

	bool atEnd = false;
	Char *pchars = chars;
	size_t totalLen = 0; // total chars read (less or equals to len)

	while (totalLen < len) {
		ssize_t buflen = utf8decoder_fill_buffer(*m_is, m_buffer, BufSize, atEnd);

		// no more bytes in the stream nor in the buffer
		if (!buflen <= 0)
			return buflen < 0 ? ssize_t(-1) : 0;

		buflen = m_buffer.length();
		const uchar_t *bytes = reinterpret_cast<uchar_t*>(m_buffer.data());

		if (!m_headerDone) {
			if (buflen == 1 && bytes[0] == 0xef)
				return 0;
			if (buflen == 2 && bytes[0] == 0xef && bytes[1] == 0xbb)
				return 0;
			if (buflen >= 3 && bytes[0] == 0xef && bytes[1] == 0xbb && bytes[2] == 0xbf) {
				// starts with a byte order mark
				bytes += 3;
				buflen -= 3;
			}
		}
		m_headerDone = true;

		if (m_remainChar != Char::MaxCodePoint) {
			*pchars++ = m_remainChar;
			++totalLen;
			m_remainChar == Char::MaxCodePoint; // reset remain char
		}

		int invalid = 0;

		for (int i = 0; i < buflen && totalLen < len; i++) {

			uint_t uc = 0;
			int n = utf8decoder_convert_bytes(reinterpret_cast<const byte_t*>(bytes + i) , buflen-i, uc, m_remainChar);

			if (n == -2) { // to few bytes
				break;
			} else if (n == -1) { // error
				uc = m_replacement.unicode();
				++invalid;
			} else if (uc == 0xFEFF) {
				continue; // skip the BOM
			} else {
				i += n;

				if (uc > CWT_USHORT_MAX) {
					uc = m_replacement.unicode();
					++invalid;
				}
			}

			*pchars++ = uc;
			++totalLen;
		}
	}

	CWT_ASSERT(totalLen <= CWT_SSIZE_MAX);
	return ssize_t(totalLen);
}


#ifdef __COMEMNT__

String Utf8TextCodec::decode(const char *bytes, size_t len, CodecState *state)
{
	if(!len)
		return String();

	bool headerdone = false;
	ushort replacement = Char::ReplacementChar;
	int need = 0;
	int error = -1;
	uint uc = 0;
	uint min_uc = 0;
	if (state) {
		if (state->flags & TextCodec::IgnoreHeader)
			headerdone = true;
		if (state->flags & TextCodec::NullReplacement)
			replacement = Char::Null;
		need = state->remain_chars;
		if (need) {
			uc = state->data[0];
			min_uc = state->data[1];
		}
	}
	if (!headerdone && len > 3
		&& (uchar_t)bytes[0] == 0xef && (uchar_t)bytes[1] == 0xbb && (uchar_t)bytes[2] == 0xbf) {
		// starts with a byte order mark
		bytes += 3;
		len -= 3;
		headerdone = true;
	}

	String result(need + len + 1, 0); // worst case
	ushort *pchars = (ushort *)result.unicode();
	uchar_t ch;
	int invalid = 0;

	for (size_t i = 0; i < len; ++i) {
		ch = bytes[i];
		if (need) {
			if ((ch&0xc0) == 0x80) {
				uc = (uc << 6) | (ch & 0x3f);
				--need;
				if (!need) {
					// utf-8 bom composes into 0xfeff code point
					bool nonCharacter;
					if (!headerdone && uc == 0xfeff) {
						// don't do anything, just skip the BOM
					} else if (!(nonCharacter = Char::isNonCharacter(uc))
							&& Char::requiresSurrogates(uc)
							&& uc <= Char::MaxCodePoint) {
						// surrogate pair
						CWT_ASSERT((pchars - (ushort*)result.unicode()) + 2 < result.length());
						*pchars++ = Char::hiSurrogate(uc);
						*pchars++ = Char::lowSurrogate(uc);
					} else if ((uc < min_uc) || Char::isSurrogate(uc) || nonCharacter || uc > Char::MaxCodePoint) {
						// error: overlong sequence, UTF16 surrogate or non-character
						*pchars++ = replacement;
						++invalid;
					} else {
						*pchars++ = uc;
					}
					headerdone = true;
				}
			} else {
				// error
				i = error;
				*pchars++ = replacement;
				++invalid;
				need = 0;
				headerdone = true;
			}
		} else {
			if (ch < 128) {
				*pchars++ = ushort(ch);
				headerdone = true;
			} else if ((ch & 0xe0) == 0xc0) {
				uc = ch & 0x1f;
				need = 1;
				error = i;
				min_uc = 0x80;
				headerdone = true;
			} else if ((ch & 0xf0) == 0xe0) {
				uc = ch & 0x0f;
				need = 2;
				error = i;
				min_uc = 0x800;
			} else if ((ch&0xf8) == 0xf0) {
				uc = ch & 0x07;
				need = 3;
				error = i;
				min_uc = 0x10000;
				headerdone = true;
			} else {
				// error
				*pchars++ = replacement;
				++invalid;
				headerdone = true;
			}
		}
	}
	if (!state && need > 0) {
		// unterminated UTF sequence
		for (size_t i = error; i < len; ++i) {
			*pchars++ = replacement;
			++invalid;
		}
	}
	result.truncate(pchars - (ushort *)result.unicode());
	if (state) {
		state->invalid_chars += invalid;
		state->remain_chars = need;
		if (headerdone)
			state->flags |= TextCodec::IgnoreHeader;
		state->data[0] = need ? uc : 0;
		state->data[1] = need ? min_uc : 0;
	}

	return result;
}

ByteArray Utf8TextCodec::encode(const Char *chars, size_t len, CodecState *state)
{
	ByteArray bytes;

	if(!len)
		return ByteArray();

	byte_t replacement = '?';
	size_t rlen = 3 * len;
	int surrogate_high = -1;

	if (state) {
		if (state->flags & TextCodec::NullReplacement)
			replacement = 0;
		if (!(state->flags & TextCodec::IgnoreHeader))
			rlen += 3;
		if (state->remain_chars)
			surrogate_high = state->data[0];
	}

	bytes.resize(rlen);
	uchar_t* cursor = (uchar_t*)bytes.data();
	const Char *ch = chars;
	int invalid = 0;

	if (state && !(state->flags & TextCodec::IgnoreHeader)) {
		*cursor++ = 0xef;
		*cursor++ = 0xbb;
		*cursor++ = 0xbf;
	}

	const Char *end = ch + len;

	while (ch < end) {
		uint u = ch->unicode();
		if (surrogate_high >= 0) {
			if (ch->isLowSurrogate()) {
				u = Char::surrogatePairToUcs4(surrogate_high, u);
				surrogate_high = -1;
			} else {
				// high surrogate without low
				*cursor = replacement;
				++ch;
				++invalid;
				surrogate_high = -1;
				continue;
			}
		} else if (ch->isLowSurrogate()) {
				// low surrogate without high
				*cursor = replacement;
				++ch;
				++invalid;
				continue;
		} else if (ch->isHiSurrogate()) {
			surrogate_high = u;
			++ch;
			continue;
		}

		if (u < 0x80) {
			*cursor++ = (uchar_t)u;
		} else {
			if (u < 0x0800) {
				*cursor++ = 0xc0 | ((uchar_t) (u >> 6));
			} else {
				// is it one of the Unicode non-characters?
				if (Char::isNonCharacter(u)) {
					*cursor++ = replacement;
					++ch;
					++invalid;
					continue;
				}

				if (Char::requiresSurrogates(u)) {
					*cursor++ = 0xf0 | ((uchar_t) (u >> 18));
					*cursor++ = 0x80 | (((uchar_t) (u >> 12)) & 0x3f);
				} else {
					*cursor++ = 0xe0 | (((uchar_t) (u >> 12)) & 0x3f);
				}
				*cursor++ = 0x80 | (((uchar_t) (u >> 6)) & 0x3f);
			}
			*cursor++ = 0x80 | ((uchar_t) (u&0x3f));
		}
		++ch;
	}

	bytes.resize(cursor - (const uchar_t*)bytes.data());
	if (state) {
		state->invalid_chars += invalid;
		state->flags |= TextCodec::IgnoreHeader;
		state->remain_chars = 0;
		if (surrogate_high >= 0) {
			state->remain_chars = 1;
			state->data[0] = surrogate_high;
		}
	}

	return bytes;
}

#endif

CWT_NS_END
