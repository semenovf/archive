#ifndef __CWT_STRING_H__
#define __CWT_STRING_H__

#include <cwt/global.h>
#include <cwt/types.h>
#include <string.h>


typedef struct CwtStringNS {
	size_t            (*strftime)     (CWT_CHAR *buf, size_t sz, const CWT_CHAR *format, const struct tm* ptm);
	size_t            (*strlen)       (const CWT_CHAR*);
	CWT_CHAR*         (*strcpy)       (CWT_CHAR *dest, const CWT_CHAR *src);
	CWT_CHAR*         (*strncpy)      (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	CWT_CHAR*         (*strchr)       (CWT_CHAR *s, CWT_CHAR ch);
	CWT_CHAR*         (*strstr)       (const CWT_CHAR *s, const CWT_CHAR *substr);
	CWT_CHAR*         (*strrstr)      (const CWT_CHAR *s, const CWT_CHAR *substr);
	BOOL              (*streq)        (const CWT_CHAR *s1, const CWT_CHAR *s2);
	BOOL              (*strieq)       (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strcmp)       (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strncmp)      (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n);
	int 			  (*stricmp)      (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strnicmp)     (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n);
	CWT_CHAR*         (*strdup)       (const CWT_CHAR *s);
	CWT_CHAR*         (*strndup)      (const CWT_CHAR *s, size_t n);
	CWT_CHAR*         (*strcat)       (CWT_CHAR *dest, const CWT_CHAR *src);
	CWT_CHAR*         (*strncat)      (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	CWT_CHAR*		  (*strtok)       (CWT_CHAR *tok, const CWT_CHAR *delim);
	CWT_CHAR*         (*strerror)     (int);
	LONG              (*strtol)       (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	LONG              (*strtoul)      (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	LONGLONG          (*strtoll)      (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	LONGLONG          (*strtoull)     (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	int               (*isalpha)      (CWT_CHAR);

	void*             (*memcpy)       (void *to, const void *from, size_t sz);
	void*             (*memmove)      (void *to, const void *from, size_t sz);
	void*             (*memset)       (void *block, int c, size_t sz);
	void*             (*bzero)        (void *block, size_t sz);

	LONGLONG          (*toLONGLONG)   (const CWT_CHAR *str, int radix, BOOL *ok);
	ULONGLONG         (*toULONGLONG)  (const CWT_CHAR *str, int radix, BOOL *ok);
	LONG              (*toLONG)       (const CWT_CHAR *str, int radix, BOOL *ok);
	ULONG             (*toULONG)      (const CWT_CHAR *str, int radix, BOOL *ok);
	INT               (*toINT)        (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT              (*toUINT)       (const CWT_CHAR *str, int radix, BOOL *ok);
	SHORT             (*toSHORT)      (const CWT_CHAR *str, int radix, BOOL *ok);
	USHORT            (*toUSHORT)     (const CWT_CHAR *str, int radix, BOOL *ok);
	SBYTE             (*toSBYTE)      (const CWT_CHAR *str, int radix, BOOL *ok);
	BYTE              (*toBYTE)       (const CWT_CHAR *str, int radix, BOOL *ok);

	INT8              (*toINT8)       (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT8             (*toUINT8)      (const CWT_CHAR *str, int radix, BOOL *ok);
	INT16             (*toINT16)      (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT16            (*toUINT16)     (const CWT_CHAR *str, int radix, BOOL *ok);
	INT32             (*toINT32)      (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT32            (*toUINT32)     (const CWT_CHAR *str, int radix, BOOL *ok);
	INT64             (*toINT64)      (const CWT_CHAR *str, int radix, BOOL *ok);
	UINT64            (*toUINT64)     (const CWT_CHAR *str, int radix, BOOL *ok);

	CWT_CHAR          (*toupper)      (CWT_CHAR ch);
	CWT_CHAR          (*tolower)      (CWT_CHAR ch);
	void              (*toupperStr)   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	void              (*tolowerStr)   (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	void              (*chomp)        (CWT_CHAR *s);
	CWT_CHAR          (*charAt)       (const CWT_CHAR *s, size_t i);
} CwtStringNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtStringNS* cwtStringNS(void);
EXTERN_C_END

extern const CWT_CHAR* CWT_CONST_EMPTYSTR;
extern const CWT_CHAR* CWT_CONST_NULLSTR;

#define CWT_STRING_OR_EMPTYSTR(s) ((s) ? (s) : CWT_CONST_EMPTYSTR)
#define CWT_STRING_OR_NULLSTR(s) ((s) ? (s) : CWT_CONST_NULLSTR)

#endif /*__CWT_STRING_H__*/
