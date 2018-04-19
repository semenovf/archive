/**
 * @file   unicode.c
 * @author wladt
 * @date   Dec 12, 2012 10:23:19 AM
 *
 * @brief
 */


#include "cwt/unicode.h"


static size_t unicode_ucs2len (const CWT_UCS2 *ucs2);
static size_t unicode_ucs4len (const CWT_UCS4 *ucs4);

extern CWT_UCS2* cwt_decode_utf8_ucs2 (const char *utf8, size_t len);
extern char*     cwt_encode_ucs2_utf8 (const CWT_UCS2 *s, size_t len);
extern CWT_UCS4* cwt_decode_utf8_ucs4 (const char *utf8, size_t len);
extern char*     cwt_encode_ucs4_utf8 (const CWT_UCS4 *s, size_t len);


static CwtUnicodeNS __unicode_ns = {
	  unicode_ucs2len
	, unicode_ucs4len

	, cwt_decode_utf8_ucs2
	, cwt_encode_ucs2_utf8
	, cwt_decode_utf8_ucs4
	, cwt_encode_ucs4_utf8
};


DLL_API_EXPORT CwtUnicodeNS* cwt_unicode_ns(void)
{
	return &__unicode_ns;
}


static size_t unicode_ucs2len (const CWT_UCS2 *ucs2)
{
	size_t len = 0;
	CWT_ASSERT(ucs2);
	while (ucs2[len++] != (CWT_UCS2)0) ;
	return --len;
}

static size_t unicode_ucs4len (const CWT_UCS4 *ucs4)
{
	size_t len = 0;
	CWT_ASSERT(ucs4);
	while (ucs4[len++] != (CWT_UCS4)0) ;
	return --len;
}
