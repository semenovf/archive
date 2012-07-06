#ifndef __CWT_STDIO_H__
#define __CWT_STDIO_H__

#include <cwt/types.h>
#include <stdarg.h>

typedef struct CwtStdioNS {
	int (*vsprintf)(CWT_CHAR *buffer, const CWT_CHAR *format, va_list argptr);
} CwtStdioNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtStdioNS* cwtStdioNS(void);
EXTERN_C_END


#endif /*__CWT_STDIO_H__*/
