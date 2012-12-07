/**
 * @file   utf.c
 * @author wladt
 * @date   Dec 7, 2012 2:40:24 PM
 *
 * @brief
 */
#include <cwt/str.h>

/*
 *
 * Bits Last code point 	Byte 1 		Byte 2 		Byte 3 		Byte 4 		Byte 5 		Byte 6
 *  7 	U+007F 				0xxxxxxx
 *  11 	U+07FF 				110xxxxx 	10xxxxxx
 *  16 	U+FFFF 				1110xxxx 	10xxxxxx 	10xxxxxx
 *  21 	U+1FFFFF 			11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 *  26 	U+3FFFFFF 			111110xx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 *  31 	U+7FFFFFFF 			1111110x 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 */
CWT_CHAR* decode_utf8(const char *utf8, size_t len/*, QTextCodec::ConverterState *state*/)
{
	CWT_CHAR *s, ptr;
    CWT_CHAR  repl_char;
    UINT32    uc;
    size_t    i;
    BOOL      ok = FALSE;

    repl_char = cwt_str_ns()->replacementChar();

    s = CWT_MALLOCA(CWT_CHAR, len + 1);
    ptr = s;

	for (i = 0; i < len; i++) {
		if ( utf8[i] < 0x80 ) {
			uc = (UINT32)utf8[i];
		} else if (utf8[i] & 0x0800) {

		}

		if(	(uc >= 0xD800 && uc <= 0xDFFF) || uc >= 0x110000 )
			*ptr++ = repl_char;
		else
			*ptr++ = (CWT_CHAR)uc;
	}

	if (!ok) {
		CWT_FREE(s);
		s = NULL;
	} else {
		*ptr++ = _T('\x0');
	}

	return s;
}

#ifdef __COMMENT__
Input:
   unsigned integer c - the code point of the character to be encoded
Output:
   byte b1, b2, b3, b4 - the encoded sequence of bytes
Algorithm:
   if (c<0x80)
      b1 = c>>0  & 0x7F | 0x00
      b2 = null
      b3 = null
      b4 = null
   else if (c<0x0800)
      b1 = c>>6  & 0x1F | 0xC0
      b2 = c>>0  & 0x3F | 0x80
      b3 = null
      b4 = null
   else if (c<0x010000)
      b1 = c>>12 & 0x0F | 0xE0
      b2 = c>>6  & 0x3F | 0x80
      b3 = c>>0  & 0x3F | 0x80
      b4 = null
   else if (c<0x110000)
      b1 = c>>18 & 0x07 | 0xF0
      b2 = c>>12 & 0x3F | 0x80
      b3 = c>>6  & 0x3F | 0x80
      b4 = c>>0  & 0x3F | 0x80
   end if




    int need = 0;
    int error = -1;
    uint uc = 0;
    uint min_uc = 0;

    if (state) {
        if (state->flags & QTextCodec::IgnoreHeader)
            headerdone = true;
        if (state->flags & QTextCodec::ConvertInvalidToNull)
            replacement = QChar::Null;
        need = state->remainingChars;

        if (need) {
            uc = state->state_data[0];
            min_uc = state->state_data[1];
        }
    }

    if (!headerdone && len > 3
    		&& (uchar)utf8[0] == 0xef
    		&& (uchar)utf8[1] == 0xbb
    		&& (uchar)utf8[2] == 0xbf) {

        // starts with a byte order mark
        utf8 += 3;
        len -= 3;
        headerdone = TRUE;

    }

    QString result(need + len + 1, Qt::Uninitialized); // worst case

    ushort *qch = (ushort *)result.unicode();

    uchar ch;

    int invalid = 0;

    for (int i = 0; i < len; ++i) {

        ch = utf8[i];

        if (need) {

            if ((ch&0xc0) == 0x80) {

                uc = (uc << 6) | (ch & 0x3f);

                --need;

                if (!need) {

                    // utf-8 bom composes into 0xfeff code point

                    BOOL nonCharacter;

                    if (!headerdone && uc == 0xfeff) {

                        // don't do anything, just skip the BOM

                    } else if (!(nonCharacter = isUnicodeNonCharacter(uc)) && uc > 0xffff && uc < 0x110000) {

                        // surrogate pair

                        Q_ASSERT((qch - (ushort*)result.unicode()) + 2 < result.length());

                        *qch++ = QChar::highSurrogate(uc);

                        *qch++ = QChar::lowSurrogate(uc);

                    } else if ((uc < min_uc) || (uc >= 0xd800 && uc <= 0xdfff) || nonCharacter || uc >= 0x110000) {

                        // error: overlong sequence, UTF16 surrogate or non-character

                        *qch++ = replacement;

                        ++invalid;

                    } else {

                        *qch++ = uc;

                    }

                    headerdone = true;

                }

            } else {

                // error

                i = error;

                *qch++ = replacement;

                ++invalid;

                need = 0;

                headerdone = true;

            }

        } else {

            if (ch < 128) {

                *qch++ = ushort(ch);

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

                *qch++ = replacement;

                ++invalid;

                headerdone = true;

            }

        }

    }

    if (!state && need > 0) {

        // unterminated UTF sequence

        for (int i = error; i < len; ++i) {

            *qch++ = replacement;

            ++invalid;

        }

    }

    result.truncate(qch - (ushort *)result.unicode());

    if (state) {

        state->invalidChars += invalid;

        state->remainingChars = need;

        if (headerdone)

            state->flags |= QTextCodec::IgnoreHeader;

        state->state_data[0] = need ? uc : 0;

        state->state_data[1] = need ? min_uc : 0;

    }

    return result;

}
#endif
