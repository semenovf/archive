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

EXTERN_C_BEGIN

DLL_API_EXPORT void init_loggers(void);

DLL_API_EXPORT void print_trace(const CHAR* msg);
DLL_API_EXPORT void print_debug(const CHAR* msg);
DLL_API_EXPORT void print_info(const CHAR* msg);
DLL_API_EXPORT void print_warn(const CHAR* msg);
DLL_API_EXPORT void print_error(const CHAR* msg);
DLL_API_EXPORT void printf_trace(const CHAR* format, ...);
DLL_API_EXPORT void printf_debug(const CHAR* format, ...);
DLL_API_EXPORT void printf_info(const CHAR* format, ...);
DLL_API_EXPORT void printf_warn(const CHAR* format, ...);
DLL_API_EXPORT void printf_error(const CHAR* format, ...);

DLL_API_EXPORT void set_printer(LOGGER_TYPE type
	, void (*printer)(const CHAR* msg)
	, const CHAR* prefix);

DLL_API_EXPORT void set_default_printers(void);

EXTERN_C_END

#endif /* _LOGGER_H_ */

