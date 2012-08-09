/*
 * string.h
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */

#ifndef __CWT_STRING_H__
#define __CWT_STRING_H__

#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/vector.h>

CWT_BEGIN_DECL_VECTOR_NS(CwtStringNS, CwtString, CWT_CHAR)
void               (*appendChar)  (CwtString *s, CWT_CHAR ch);
void               (*appendChars) (CwtString *s, const CWT_CHAR *elems, size_t n);
void               (*prependChar) (CwtString *s, CWT_CHAR ch);
void               (*insertChar)  (CwtString *s, CWT_CHAR ch, size_t pos);
void               (*removeChar)  (CwtString *s, size_t pos);
void               (*append)      (CwtString *s, const CWT_CHAR *str);
const CWT_CHAR*    (*cstr)        (CwtString *s);
int                (*sprintf)     (CwtString *s, const CWT_CHAR *format, ...);
int                (*vsprintf)    (CwtString *s, const CWT_CHAR *format, va_list args);
CWT_END_DECL_VECTOR_NS(CwtStringNS)

EXTERN_C_BEGIN
DLL_API_EXPORT CwtStringNS* cwtStringNS(void);
EXTERN_C_END


#endif /* __CWT_STRING_H__ */

