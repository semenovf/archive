#ifndef __CWT_TXTCODEC_H__
#define __CWT_TXTCODEC_H__

#include <cwt/types.h>

typedef struct _CwtTextCodecNS {
	char*             (*toLatin1_n)   (const CWT_CHAR *s, size_t n);
	CWT_CHAR*         (*fromLatin1_n) (const char *s, size_t n);
	char*             (*toUtf8_n)     (const CWT_CHAR *s, size_t n);
	CWT_CHAR*         (*fromUtf8_n)   (const char *s, size_t n);
	char*             (*toMBCS_n)     (const CWT_CHAR *s, const CWT_CHAR *csname, size_t n);
	CWT_CHAR*         (*fromMBCS_n)   (const char *s, const CWT_CHAR *csname, size_t n);
	char*             (*toLatin1)     (const CWT_CHAR *s);
	CWT_CHAR*         (*fromLatin1)   (const char *s);
	char*             (*toUtf8)       (const CWT_CHAR *s);
	CWT_CHAR*         (*fromUtf8)     (const char *s);
	char*             (*toMBCS)       (const CWT_CHAR *s, const CWT_CHAR *csname);
	CWT_CHAR*         (*fromMBCS)     (const char *s, const CWT_CHAR *csname);
} CwtTextCodecNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtTextCodecNS* cwt_textcodec_ns(void);

DLL_API_EXPORT wchar_t*  cwt_decode_utf8(const char *utf8, size_t len);
DLL_API_EXPORT char*     cwt_encode_utf8(const wchar_t *s, size_t len);

EXTERN_C_END

#endif /*__CWT_TXTCODEC_H__*/
