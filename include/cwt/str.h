#ifndef __CWT_STR_H__
#define __CWT_STR_H__

#ifdef _STRING_H
#	error do not need to mix <cwt/str.h> and <string.h>
#endif
#include <time.h>
#include <cwt/types.h>
#include <cwt/string.h>
#include <cwt/bytearr.h>

typedef struct CwtQuotePair {
	CWT_CHAR begin;
	CWT_CHAR end;
} CwtQuotePair;


typedef struct CwtStrNS {
	const CWT_CHAR*   (*strerror)     (int);
	CWT_CHAR*         (*strptime)     (const CWT_CHAR *buf, const CWT_CHAR *fmt, struct tm *tm);
	size_t            (*strftime)     (CWT_CHAR *buf, size_t sz, const CWT_CHAR *format, const struct tm* ptm);
	size_t            (*strlen)       (const CWT_CHAR*);
	CWT_CHAR*         (*strcpy)       (CWT_CHAR *dest, const CWT_CHAR *src);
	CWT_CHAR*         (*strncpy)      (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	CWT_CHAR*         (*strchr)       (CWT_CHAR *s, CWT_CHAR ch);
	CWT_CHAR*         (*strstr)       (const CWT_CHAR *s, const CWT_CHAR *substr);
	CWT_CHAR*         (*strrstr)      (const CWT_CHAR *s, const CWT_CHAR *substr);
	int               (*strcmp)       (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strncmp)      (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n);
	int 			  (*stricmp)      (const CWT_CHAR *s1, const CWT_CHAR *s2);
	int               (*strnicmp)     (const CWT_CHAR *s1, const CWT_CHAR *s2, size_t n);
	BOOL              (*streq)        (const CWT_CHAR *s1, const CWT_CHAR *s2);
	BOOL              (*strieq)       (const CWT_CHAR *s1, const CWT_CHAR *s2);
	CWT_CHAR*         (*strdup)       (const CWT_CHAR *s);
	CWT_CHAR*         (*strndup)      (const CWT_CHAR *s, size_t n);
	CWT_CHAR*         (*strcat)       (CWT_CHAR *dest, const CWT_CHAR *src);
	CWT_CHAR*         (*strncat)      (CWT_CHAR *dest, const CWT_CHAR *src, size_t n);
	CWT_CHAR*		  (*strtok)       (CWT_CHAR *tok, const CWT_CHAR *delim);
	LONG              (*strtol)       (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	ULONG             (*strtoul)      (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	LONGLONG          (*strtoll)      (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	ULONGLONG         (*strtoull)     (const CWT_CHAR *s, CWT_CHAR **endptr, int radix);
	double            (*strtod)       (const CWT_CHAR *s, CWT_CHAR **endptr);
	int               (*isalpha)      (CWT_CHAR);
	int               (*isdigit)      (CWT_CHAR);
	int               (*isspace)      (CWT_CHAR);

	void*             (*memcpy)       (void *to, const void *from, size_t sz);
	void*             (*memmove)      (void *to, const void *from, size_t sz);
	void*             (*memset)       (void *block, int c, size_t sz);
	int               (*memcmp)       (const void *m1, const void *m2, size_t n);
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
	double            (*toDouble)     (const CWT_CHAR *str, BOOL *ok);
	float             (*toFloat)      (const CWT_CHAR *str, BOOL *ok);
	void              (*toTIME)       (const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok);
	void              (*toDATE)       (const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok);
	void              (*toDATETIME)   (const CWT_CHAR *str, CWT_TIME *tm, const CWT_CHAR *format, BOOL *ok);

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

	const CWT_CHAR*   (*constEmptyStr)(void);
	const CWT_CHAR*   (*constNullStr) (void);

	const CwtQuotePair*   (*singleQuotesPair)(void);
	const CwtQuotePair*   (*doubleQuotesPair)(void);
	const CwtQuotePair*   (*quotesPair)(void);
	const CWT_CHAR*       (*whitespaces)(void);

} CwtStrNS;


#define CWT_QUOTES_SINGLE cwtStrNS()->singleQuotesPair()
#define CWT_QUOTES_DOUBLE cwtStrNS()->doubleQuotesPair()
#define CWT_QUOTES_BOTH   cwtStrNS()->quotesPair()
#define CWT_WHITESPACES   cwtStrNS()->whitespaces() /* ' ', '\t', '\n', '\v', '\f', '\r' */

EXTERN_C_BEGIN

DLL_API_EXPORT CwtStrNS* cwtStrNS(void);

#define CWT_STRING_OR_EMPTYSTR(s) ((s) ? (s) : cwtStrNS()->constEmptyStr())
#define CWT_STRING_OR_NULLSTR(s) ((s) ? (s) : cwtStrNS()->constNullStr())

#ifdef strcasecmp
#	undef strcasecmp
#endif
#ifdef strncasecmp
#	undef strncasecmp
#endif

#ifdef CWT_CC_MSC
#	define strcasecmp  CWT_ISO_CPP_NAME(stricmp)
#	define strncasecmp CWT_ISO_CPP_NAME(strnicmp)
DLL_API_EXPORT char* strndup(const char *s, size_t n);
#else
#	define stricmp  CWT_ISO_CPP_NAME(strcasecmp)
#	define strnicmp CWT_ISO_CPP_NAME(strncasecmp)
#endif

EXTERN_C_END

#endif /*__CWT_STR_H__*/
