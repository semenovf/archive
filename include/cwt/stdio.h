#ifndef __CWT_STDIO_H__
#define __CWT_STDIO_H__

#include <cwt/types.h>

#include <stdio.h>
#include <stdarg.h>

typedef struct CwtStdioNS {
	int (*printf)    (const CWT_CHAR *format, ...);
	int (*sprintf)   (CWT_CHAR *sbuf, const CWT_CHAR *format, ...);
	int (*fprintf)   (FILE *stream, const CWT_CHAR *format, ...);
	int (*vsprintf)  (CWT_CHAR *buffer, const CWT_CHAR *format, va_list argptr);
	int (*snprintf)  (CWT_CHAR *buffer, size_t count, const CWT_CHAR *format, ...);
	int (*vsnprintf) (CWT_CHAR *buffer, size_t count, const CWT_CHAR *format, va_list argptr);
} CwtStdioNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtStdioNS* cwt_stdio_ns(void);
EXTERN_C_END


#endif /*__CWT_STDIO_H__*/
