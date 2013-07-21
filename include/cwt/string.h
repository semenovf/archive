/*
 * string.h
 *
 *  Created on: 12 июля 2013 г.
 *      Author: wladt
 */

#ifndef __CWT_STRING_H__
#define __CWT_STRING_H__

#include <cwt/cwt.h>
#include <string.h>

#ifdef CWT_CC_MSVC
#	define strcasecmp _stricmp
#endif

EXTERN_C_BEGIN

DLL_API char* cwt_strerror(int errn, char *buf, size_t buflen);

EXTERN_C_END


#endif /* __CWT_STRING_H__ */
