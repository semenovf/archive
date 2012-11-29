/*
 * stack.h
 *
 *  Created on: 31.07.2012
 *      Author: wladt
 */

#ifndef __CWT_STACK_H__
#define __CWT_STACK_H__

#include <cwt/list.h>

typedef CwtList CwtStack;

EXTERN_C_END

typedef struct _CwtStackNS {
	CwtStack* (*create)    (size_t sizeofData, void (*data_cleanup)(void*));
	CwtStack* (*createPtr) (void (*data_cleanup)(void*));
	void      (*free)      (CwtStack*);
	void      (*clear)     (CwtStack*);
	BOOL      (*isEmpty)   (CwtStack*);
	void      (*push)      (CwtStack *stack, void *data);
	BOOL      (*pop)       (CwtStack *stack, void *data);
	BOOL      (*peek)      (CwtStack *stack, void *data);
} CwtStackNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtStackNS* cwt_stack_ns(void);
EXTERN_C_END


#endif /* __CWT_STACK_H__ */
