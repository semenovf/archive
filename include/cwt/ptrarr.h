/**
 * @file   ptrarr.h
 * @author wladt
 * @date   Nov 2, 2012 3:58:16 PM
 *
 * @brief Pointers array
 */

#ifndef __CWT_PTRARR_H__
#define __CWT_PTRARR_H__

#include <cwt/types.h>
#include <cwt/vector.h>

typedef void* ptr_type;

CWT_BEGIN_DECL_VECTOR_NS(CwtPtrArrayNS, CwtPtrArray, ptr_type)
CWT_END_DECL_VECTOR_NS(CwtPtrArrayNS)

EXTERN_C_BEGIN
DLL_API_EXPORT CwtPtrArrayNS* cwt_ptrarray_ns(void);
EXTERN_C_END


#endif /* __CWT_PTRARR_H__ */
