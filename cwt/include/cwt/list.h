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
	struct _CwtListNode *n;
	struct _CwtListNode *p;
	char                 d[1];
} CwtListNode;


typedef struct _CwtList {
	struct _CwtListNode* f;
	struct _CwtListNode* l;
	size_t count;
	size_t datasz;
	void (*data_cleanup)(void*);
	BOOL   is_ptr;
} CwtList;


typedef struct _CwtListIterator
{
	struct _CwtList     *list;
	struct _CwtListNode *node; /* current node */
	BOOL forward;
} CwtListIterator;


typedef struct _CwtListNS {
	void             (*init)        (CwtList*, size_t sizeofData, void (*data_cleanup)(void*));
	void             (*initPtr)     (CwtList*, void (*data_cleanup)(void*));
	CwtList*         (*create)      (size_t sizeofData, void (*data_cleanup)(void*));
	CwtList*         (*createPtr)   (void (*data_cleanup)(void*));
	void             (*destroy)     (CwtList*);
	void             (*free)        (CwtList*);
	void             (*clear)       (CwtList*);
	size_t           (*size)        (CwtList*);
	BOOL             (*isEmpty)     (CwtList*);

	void             (*insert)      (CwtListIterator *before, void *data);
	void             (*prepend)     (CwtList*, void *data);
	void             (*append)      (CwtList*, void *data);

	void             (*remove)      (CwtListIterator *before);
	void             (*removeFirst) (CwtList*);
	void             (*removeLast)  (CwtList*);

	void             (*traverse)    (CwtList*, int (*callback)(void *data, void *extra), void *extra);
	void             (*rtraverse)   (CwtList*, int (*callback)(void *data, void *extra), void *extra);
	BOOL             (*find)        (CwtListIterator *it, const void *data);

	void*	         (*at)          (CwtList*, size_t index);
	void*            (*first)       (CwtList*);
	void*            (*last)        (CwtList*);
	void             (*begin)       (CwtList *list, CwtListIterator *it);
	void             (*rbegin)      (CwtList *list, CwtListIterator *it);
	BOOL             (*hasMore)     (CwtListIterator *it);
	void*            (*next)        (CwtListIterator *it);
	void*            (*data)        (CwtListIterator *it);
} CwtListNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtListNS* cwt_list_ns(void);
EXTERN_C_END

#endif /* __CWT_LIST_H__ */
