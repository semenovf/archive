/*
 * list.h
 *
 *  Created on: 09.12.2011
 *      Author: wladt
 */

#ifndef __CWT_LIST_H__
#define __CWT_LIST_H__

#include <cwt/types.h>

/* http://en.wikipedia.org/wiki/Linked_list */
/* Single linked list */
/* Double linked list */

typedef struct _CwtListNode
{
	struct _CwtListNode* next;
	void *data;
} CwtListNode;


typedef struct _CwtList {
	struct _CwtListNode* first;
	struct _CwtListNode* last;
	void (*free_node)(void*);
	void (*free_data)(void*);
} CwtList;


typedef struct _CwtListIterator
{
	struct _CwtList *list;
	struct _CwtListNode *node; /* current node */
} CwtListIterator;


typedef struct _CwtListNS {
	CwtList*      (*create)      (void);
	void          (*registerCleanup) (CwtList *list, void (*free_node)(void*), void (*free_data)(void*));
	void          (*init)        (CwtList*);
	void          (*free)        (CwtList*);
	void          (*insertAfter) (CwtListNode *node, CwtListNode *new_node);
	void          (*insertFirst) (CwtList *list, CwtListNode *new_node);
	void          (*insertLast)  (CwtList *list, CwtListNode *new_node);
	CwtListNode*  (*removeFirst) (CwtList *list);
	CwtListNode*  (*removeLast)  (CwtList *list);
	CwtListNode*  (*removeAfter) (CwtListNode *node);
	void          (*traverse)    (CwtList *list, int (*callback)(void *data, void *extra), void *extra);
	CwtListNode*  (*find)        (CwtList *list, void *data);
	void          (*clear)       (CwtList *list);
	void          (*begin)       (CwtList *list, CwtListIterator *it);
	BOOL          (*hasMore)     (CwtListIterator *it);
	void*         (*next)        (CwtListIterator *it);
} CwtListNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtListNS* cwtListNS(void);
EXTERN_C_END

#endif /* __CWT_LIST_H__ */
