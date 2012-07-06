/*
 * dlist.h
 *
 *  Created on: 25.06.2012
 *      Author: wladt
 */

#ifndef __CWT_DLIST_H__
#define __CWT_DLIST_H__

#include <cwt/types.h>

/* http://en.wikipedia.org/wiki/Linked_list */
/* Double linked list */

struct DListNode
{
	struct DListNode* next;
	struct DListNode* prev;
	void *data;
};

struct DList {
	struct DListNode* first;
	struct DListNode* last;
};

struct DListIterator
{
	struct DList *list;
	struct DListNode *node; /* current node */
	BOOL forward;
};

typedef struct DListNode DListNode;
typedef struct DListNode CwtDListNode;
typedef struct DList DList;
typedef struct DList CwtDList;
typedef struct DListIterator DListIterator;
typedef struct DListIterator CwtDListIterator;

EXTERN_C_BEGIN

DLL_API_EXPORT void       dlist_init(DList *dlist);
DLL_API_EXPORT void       dlist_insert_after(DList *list, DListNode *node, DListNode *new_node);
DLL_API_EXPORT void       dlist_insert_before(DList *list, DListNode *node, DListNode *new_node);
DLL_API_EXPORT void       dlist_insert_first(DList *list, DListNode *new_node);
DLL_API_EXPORT void       dlist_insert_last(DList *list, DListNode *new_node);
DLL_API_EXPORT DListNode* dlist_remove(DList *list, DListNode* node);
DLL_API_EXPORT DListNode* dlist_remove_first(DList *dlist);
DLL_API_EXPORT DListNode* dlist_remove_last(DList *dlist);
DLL_API_EXPORT void       dlist_traverse(DList *list, int (*callback)(void *data, void *extra), void *extra);
DLL_API_EXPORT void       dlist_back_traverse(DList *list, int (*callback)(void *data, void *extra), void *extra);
DLL_API_EXPORT DListNode* dlist_find(DList *list, void *data);
DLL_API_EXPORT void       dlist_clear(DList *list, void (*free_node)(void*), void (*free_data)(void*));
DLL_API_EXPORT DListNode* dlist_at(DList *list, size_t index);

DLL_API_EXPORT void       dlist_begin(DList *dlist, DListIterator *dt);
DLL_API_EXPORT void       dlist_rbegin(DList *dlist, DListIterator *dt);
DLL_API_EXPORT BOOL       dlist_has_more(DListIterator *dt);
DLL_API_EXPORT void*      dlist_next(DListIterator *dt);


#define cwtDListInit         dlist_init
#define cwtDListInsertAfter  dlist_insert_after
#define cwtDListInsertBefore dlist_insert_before
#define cwtDListInsertFirst  dlist_insert_first
#define cwtDListInsertLast   dlist_insert_last
#define cwtDListRemove       dlist_remove
#define cwtDListRemoveFirst  dlist_remove_first
#define cwtDListRemoveLast   dlist_remove_last
#define cwtDListTraverse     dlist_traverse
#define cwtDListBackTraverse dlist_back_traverse
#define cwtDListFind         dlist_find
#define cwtDListClear        dlist_clear
#define cwtDListAt           dlist_at

#define cwtDListBegin        dlist_begin
#define cwtDListRbegin       dlist_rbegin
#define cwtDListHasMore      dlist_has_more
#define cwtDListNext         dlist_next

EXTERN_C_END

#endif /* __CWT_DLIST_H__ */
