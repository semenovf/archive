/*
 * list.h
 *
 *  Created on: 09.12.2011
 *      Author: wladt
 */

#ifndef __CWT_STACK_H__
#define __CWT_STACK_H__

#include <cwt/list.h>

typedef CwtList CwtStack;
typedef CwtListNode CwtStackNode;

typedef struct _CwtStackIterator
{
	CwtStack *stack;
	CwtStackNode *node; /* current node */
} CwtStackIterator;

EXTERN_C_END

typedef struct _CwtStackNS {
	CwtStack*     (*create)      (void);
	void          (*registerCleanup) (CwtStack *list, void (*free_data)(void*));
	void          (*init)        (CwtStack*);
	void          (*free)        (CwtStack*);
	void          (*clear)       (CwtStack *list);
	void          (*push)        (CwtStack *stack, void *data);
	void*         (*pop)         (CwtStack *stack);
	void*         (*peek)        (CwtStack *stack);
	BOOL          (*isEmpty)     (CwtStack *stack);
	void          (*begin)       (CwtStack *list, CwtStackIterator *it);
	BOOL          (*hasMore)     (CwtStackIterator *it);
	void*         (*next)        (CwtStackIterator *it);
} CwtStackNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtStackNS* cwtStackNS(void);
EXTERN_C_END


#endif /* __CWT_STACK_H__ */
