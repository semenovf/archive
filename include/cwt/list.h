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

struct ListNode
{
	struct ListNode* next;
	void *data;
};


struct List {
	struct ListNode* first;
};


struct ListIterator
{
	struct List *list;
	struct ListNode *node; /* current node */
};


typedef struct ListNode ListNode;
typedef struct ListNode CwtListNode;
typedef struct List List;
typedef struct List CwtList;
typedef struct ListIterator ListIterator;
typedef struct ListIterator CwtListIterator;

EXTERN_C_BEGIN

DLL_API_EXPORT void       list_init(List *list);
DLL_API_EXPORT void       list_insert_after(ListNode *node, ListNode *new_node);
DLL_API_EXPORT void       list_insert_first(List *list, ListNode *new_node);
DLL_API_EXPORT ListNode*  list_remove_first(List *list);
DLL_API_EXPORT ListNode*  list_remove_after(ListNode *node);
DLL_API_EXPORT void       list_traverse(List *list, int (*callback)(void *data, void *extra), void *extra);
DLL_API_EXPORT ListNode*  list_find(List *list, void *data);
DLL_API_EXPORT void       list_clear(List *list, void (*free_node)(void*), void (*free_data)(void*));
DLL_API_EXPORT void       list_begin(List *list, ListIterator *it);
DLL_API_EXPORT BOOL       list_has_more(ListIterator *it);
DLL_API_EXPORT void*      list_next(ListIterator *it);

#define cwtListInit            list_init
#define cwtListInsertAfter     list_insert_after
#define cwtListListInsertFirst list_insert_first
#define cwtListRemoveFirst     list_remove_first
#define cwtListRemoveAfter     list_remove_after
#define cwtListTraverse        list_traverse
#define cwtListFind            list_find
#define cwtListClear           list_clear
#define cwtListBegin           list_begin
#define cwtListHasMore         list_has_more
#define cwtListNext            list_next

EXTERN_C_END

#endif /* __CWT_LIST_H__ */
