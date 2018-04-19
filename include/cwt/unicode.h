/**
 * @file   unicode.h
 * @author wladt
 * @date   Dec 12, 2012 10:22:23 AM
 *
 * @brief
 */

#ifndef __CWT_UNICODE_H__
#define __CWT_UNICODE_H__


#include <cwt/types.h>

typedef struct _CwtUnicodeNS {
	size_t (*ucs2len)  (const CWT_UCS2 *ucs2);
	size_t (*ucs4len)  (const CWT_UCS4 *ucs4);

	CWT_UCS2* (*decodeUtf8ToUcs2) (const char *utf8, size_t len);
	char*     (*encodeUcs2ToUtf8) (const CWT_UCS2 *s, size_t len);

	CWT_UCS4* (*decodeUtf8ToUcs4) (const char *utf8, size_t len);
	char*     (*encodeUcs4ToUtf8) (const CWT_UCS4 *s, size_t len);

} CwtUnicodeNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtUnicodeNS* cwt_unicode_ns(void);
EXTERN_C_END


#endif /* __CWT_UNICODE_H__ */
