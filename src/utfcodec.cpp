/**
 * @file   utf8codec.cpp
 * @author wladt
 * @date   Mar 1, 2013 11:16:36 AM
 *
 * @brief
 */

#include "utfcodec_p.hpp"

CWT_NS_BEGIN

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

CWT_NS_END
