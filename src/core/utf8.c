/**
 * @file   utf.c
 * @author wladt
 * @date   Dec 7, 2012 2:40:24 PM
 *
 * @brief
 */
#include "cwt/txtcodec.h"
#include "cwt/str.h"


/*
 *
 * Bits Last code point/ 	Byte 1 		Byte 2 		Byte 3 		Byte 4 		Byte 5 		Byte 6
 *  7 	U+007F 				0xxxxxxx
 *  11 	U+07FF 				110xxxxx 	10xxxxxx
 *  16 	U+FFFF 				1110xxxx 	10xxxxxx 	10xxxxxx
 *  21 	U+1FFFFF 			11110xxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 *  26 	U+3FFFFFF 			111110xx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 *  31 	U+7FFFFFFF 			1111110x 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx 	10xxxxxx
 */
DLL_API_EXPORT wchar_t* cwt_decode_utf8(const char *utf8, size_t len)
{
	wchar_t *s, *ptr;
	wchar_t  repl_char;
    UINT32    uc;
    size_t    i;
    int       need = 0;
    UCHAR     ch;

    repl_char = cwt_str_ns()->replacementChar();

    s = CWT_MALLOCA(wchar_t, len + 1);
    ptr = s;

	for (i = 0; i < len; ++i) {
		ch = (UCHAR)utf8[i];

		if (need) {
			if ((ch & 0xC0) == 0x80) {
				uc = (uc << 6) | (ch & 0x3F);
				--need;

				if (!need) {
					if(	(uc >= 0xD800 && uc <= 0xDFFF) || uc >= 0x110000 )
						*ptr++ = repl_char;
					else
						*ptr++ = (wchar_t)uc;
				}
			} else {
				; /* error */
			}
		} else {
			if ( ch < 0x80 ) {
				uc = (UINT32)ch;
				*ptr++ = (wchar_t)uc;
			} else if ((ch & 0xE0) == 0xC0) {
				uc = ch & 0x1f;
				need = 1;
			} else if ((ch & 0xF0) == 0xE0) {
				uc = ch & 0x0f;
				need = 2;
			} else if ((ch & 0xF8) == 0xF0) {
				uc = ch & 0x07;
				need = 3;
			}
#if CWT_SIZEOF_WCHAR == 4
			else if ((ch & 0xFC) == 0xF8) {
				uc = ch & 0x03;
				need = 4;
			} else if ((ch & 0xFE) == 0xFC) {
				uc = ch & 0x01;
				need = 5;
			}
#endif
			else {
				*ptr++ = repl_char;
			}
		}
	}

	*ptr++ = _T('\x0');

	return s;
}



DLL_API_EXPORT char* cwt_encode_utf8(const wchar_t *s, size_t len)
{
	char *utf8_str;
	wchar_t ch;
	size_t utf8_len, utf8_max_len, i;

	if (!s)
		return NULL;

	if (!len)
		return strdup("");

	utf8_max_len = len * 3 + 1; /* worse case */
	utf8_str = CWT_MALLOCA(char, utf8_max_len);

	for (i = 0, utf8_len = 0; i < len; i++) {
		ch = s[i];

		if (ch < 0x0080) {
			utf8_str[utf8_len++] = (char)ch;
		} else if (ch < 0x0800) {
			utf8_str[utf8_len++] = ((ch >> 6) & 0x1F) | 0xC0;
			utf8_str[utf8_len++] = (ch & 0x3F)        | 0x80;
		} else if (ch < 0x010000) {
			utf8_str[utf8_len++] = ((ch >> 12) & 0x0F) | 0xE0;
			utf8_str[utf8_len++] = ((ch >> 6)  & 0x3F) | 0x80;
			utf8_str[utf8_len++] = (ch & 0x3F)         | 0x80;
		}
#if CWT_SIZEOF_WCHAR == 4
		  else if (ch < 0x200000) {
			utf8_str[utf8_len++] = ((ch >> 18) & 0x07) | 0xF0;
			utf8_str[utf8_len++] = ((ch >> 12) & 0x3F) | 0x80;
			utf8_str[utf8_len++] = ((ch >> 6)  & 0x3F) | 0x80;
			utf8_str[utf8_len++] = (ch & 0x3F)         | 0x80;
		} else if (ch < 0x4000000) {
			utf8_str[utf8_len++] = ((ch >> 24) & 0x03) | 0xF8;
			utf8_str[utf8_len++] = ((ch >> 18) & 0x3F) | 0x80;
			utf8_str[utf8_len++] = ((ch >> 12) & 0x3F) | 0x80;
			utf8_str[utf8_len++] = ((ch >> 6)  & 0x3F) | 0x80;
			utf8_str[utf8_len++] = (ch & 0x3F)         | 0x80;
		} else if (ch < 0x4000000) {
			utf8_str[utf8_len++] = ((ch >> 30) & 0x01) | 0xFC;
			utf8_str[utf8_len++] = ((ch >> 24) & 0x3F) | 0x80;
			utf8_str[utf8_len++] = ((ch >> 18) & 0x3F) | 0x80;
			utf8_str[utf8_len++] = ((ch >> 12) & 0x3F) | 0x80;
			utf8_str[utf8_len++] = ((ch >> 6)  & 0x3F) | 0x80;
			utf8_str[utf8_len++] = (ch & 0x3F)         | 0x80;
		}
#endif
		else {
			;
		}
	}

	utf8_str[utf8_len++] = '\0';
	CWT_ASSERT(utf8_len <= utf8_max_len);

	utf8_str = CWT_REALLOC(utf8_str, utf8_len);
	return utf8_str;
}

