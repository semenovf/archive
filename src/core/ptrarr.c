/**
 * @file   ptrarr.h
 * @author wladt
 * @date   Nov 2, 2012 3:59:44 PM
 *
 * @brief
 */


#include <cwt/ptrarr.h>

CWT_BEGIN_DEF_VECTOR_NS(CwtPtrArrayNS, CwtPtrArray, void*)
CWT_END_DEF_VECTOR_NS(CwtPtrArrayNS)


DLL_API_EXPORT CwtPtrArrayNS* cwtPtrArrayNS(void)
{
	return &__CwtPtrArrayNS;
}

CWT_VECTOR_METHODS(CwtPtrArray,void*)
