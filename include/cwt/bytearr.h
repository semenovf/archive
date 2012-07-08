/*
 * bytearr.h
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */

#ifndef __CWT_BYTEARR_H__
#define __CWT_BYTEARR_H__

#include <cwt/types.h>
#include <cwt/vector.h>

CWT_BEGIN_DECL_VECTOR_NS(CwtByteArrayNS, CwtByteArray, BYTE)
const char*    (*cstr)        (CwtByteArray *ba);
CWT_END_DECL_VECTOR_NS(CwtByteArrayNS)

EXTERN_C_BEGIN
DLL_API_EXPORT CwtByteArrayNS* cwtByteArrayNS(void);
EXTERN_C_END


#endif /* __CWT_BYTEARR_H__ */
