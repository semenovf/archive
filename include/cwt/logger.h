#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>
#include <cwt/global.h>
#include <cwt/types.h>

enum LOGGER_TYPE
{
	  LOGGER_TRACE
	, LOGGER_DEBUG
	, LOGGER_INFO
	, LOGGER_WARN
	, LOGGER_ERROR
	, LOGGER_COUNT
};
typedef enum LOGGER_TYPE LOGGER_TYPE;


typedef struct _CwtLoggerNS {
	void (*trace)     (const CWT_CHAR* format, ...);
	void (*debug)     (const CWT_CHAR* format, ...);
	void (*info)      (const CWT_CHAR* format, ...);
	void (*warn)      (const CWT_CHAR* format, ...);
	void (*error)     (const CWT_CHAR* format, ...);
	void (*setPrinter)(LOGGER_TYPE type, void (*printer)(const CWT_CHAR* msg), const CWT_CHAR* prefix);
	void (*setDefaultPrinters)(void);
} CwtLoggerNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtLoggerNS* cwt_logger_ns(void);
EXTERN_C_END

EXTERN_C_END

#endif /* _LOGGER_H_ */

