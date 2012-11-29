#ifndef __CWT_TXTCODEC_H__
#define __CWT_TXTCODEC_H__

#include <cwt/types.h>
/*
#include <cwt/string.h>
#include <cwt/bytearr.h>
*/

typedef struct _CwtTextCodecNS {
	char*             (*toLatin1)     (const CWT_CHAR *s, size_t n);
	CWT_CHAR*         (*fromLatin1)   (const char *s, size_t n);
	char*             (*toUtf8)       (const CWT_CHAR *s, size_t n);
	CWT_CHAR*         (*fromUtf8)     (const char *s, size_t n);
	char*             (*toMBCS)       (const CWT_CHAR *s, const CWT_CHAR *csname, size_t n);
	CWT_CHAR*         (*fromMBCS)     (const char *s, const CWT_CHAR *csname, size_t n);
} CwtTextCodecNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtTextCodecNS* cwt_textcodec_ns(void);
EXTERN_C_END

#endif /*__CWT_TXTCODEC_H__*/
