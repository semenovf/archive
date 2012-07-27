/*
 * list.c
 *
 *  Created on: 09.12.2011
 *      Author: wladt
 */



#include <cwt/list.h>
#include <cwt/dlist.h>
#include <cwt/stack.h>

#ifndef NULL
#	define NULL ((void*)0)
#endif


void list_init(List *list)
{
	CWT_ASSERT(list);
	list->first = NULL;
}

void list_traverse(List *list, int (*callback)(void *data, void *extra), void *extra)
{
	ListNode *node = list->first;

	while( node ) {
	    if( callback(node->data, extra) )
	    	break;
	    node = node->next;
	}
}

void list_insert_after(ListNode *node, ListNode *new_node)
{
    new_node->next = node->next;
    node->next     = new_node;
}

void list_insert_first(List *list, ListNode *new_node)
{
     new_node->next = list->first;
     list->first = new_node;
}

ListNode* list_remove_after(ListNode *node)
{
    ListNode *obsolete = node->next;
    if( obsolete )
    	node->next = node->next->next;
    return obsolete;
}

ListNode* list_remove_first(List *list)
{
    ListNode *obsolete = list->first;
	if( obsolete )
		list->first = list->first->next;
    return obsolete;
}

ListNode* list_find(List *list, void *data)
{
	ListNode *node = list->first;

	while( node ) {
	    if( node->data == data )
	    	return node;
	    node = node->next;
	}
	return NULL;
}

void list_clear(List *list, void (*free_node)(void*), void (*free_data)(void*))
{
	ListNode *node = list->first;

	while( node ) {
		if( node->data && free_data )
			free_data(node->data);

		list_remove_first(list);

		if( free_node )
			free_node(node);

	    node = list->first;
	}

	list->first = NULL;
}


void list_begin(List *list, ListIterator *it)
{
	it->list = list;
	it->node = list->first;
}

BOOL list_has_more(ListIterator *it)
{
	return it->node ? TRUE : FALSE;
}

void* list_next(ListIterator *it)
{
	ListNode *node = it->node;
	it->node = it->node->next;
	return node->data;
}


/*--- Double linked list functions ---*/
void dlist_init(DList *dlist)
{
	CWT_ASSERT(dlist);
	dlist->first = NULL;
	dlist->last = NULL;
}

void dlist_insert_after(DList *list, DListNode *node, DListNode *new_node)
{
	new_node->prev = node;
    new_node->next = node->next;
    if( !node->next )
        list->last = new_node;
    else
        node->next->prev = new_node;
    node->next = new_node;
}

void dlist_insert_before(DList *list, DListNode *node, DListNode *new_node)
{
    new_node->prev = node->prev;
    new_node->next = node;
    if( !node->prev )
        list->first = new_node;
    else
        node->prev->next = new_node;
    node->prev = new_node;
}


void dlist_insert_first(DList *list, DListNode *new_node)
{
	if( !list->first ) {
        list->first = new_node;
        list->last  = new_node;
        new_node->prev = NULL;
        new_node->next = NULL;
    } else {
    	dlist_insert_before(list, list->first, new_node);
    }
}

void dlist_insert_last(DList *list, DListNode *new_node)
{
    if( !list->last )
		dlist_insert_first(list, new_node);
	else
		dlist_insert_after(list, list->last, new_node);
}


DListNode* dlist_remove(DList *list, DListNode *node)
{
	if( !node->prev )
		list->first = node->next;
	else
		node->prev->next = node->next;

	if( !node->next )
		list->last = node->prev;
	else
		node->next->prev = node->prev;

	return node;
}


DListNode* dlist_remove_first(DList *dlist)
{
    DListNode *obsolete = dlist->first;
	if( obsolete )
		dlist->first = dlist->first->next;
    return obsolete;
}

DListNode* dlist_remove_last(DList *dlist)
{
    DListNode *obsolete = dlist->last;

	if( obsolete ) {
		DListNode *before_obsolete = obsolete->prev;
		if( before_obsolete ) {
			before_obsolete->next = NULL;
		}

		dlist->last = before_obsolete;
	}
    return obsolete;
}


void dlist_traverse(DList *list, int (*callback)(void *data, void *extra), void *extra)
{
	DListNode *node = list->first;

	while( node ) {
	    if( callback(node->data, extra) )
	    	break;
	    node = node->next;
	}

}

void dlist_back_traverse(DList *list, int (*callback)(void *data, void *extra), void *extra)
{
	DListNode *node = list->last;
	while( node ) {
		 if( callback(node->data, extra) )
			 break;
		 node = node->prev;
	}
}

DListNode* dlist_find(DList *list, void *data)
{
	DListNode *node = list->first;

	while( node ) {
	    if( node->data == data )
	    	return node;
	    node = node->next;
	}
	return NULL;
}

void dlist_clear(DList *list, void (*free_node)(void*), void (*free_data)(void*))
{
	DListNode *node = list->first;

	while( node ) {
		if( node->data && free_data )
			free_data(node->data);

		dlist_remove_first(list);

		if( free_node )
			free_node(node);

	    node = list->first;
	}

	list->first = NULL;
	list->last = NULL;
}


DListNode* dlist_at(DList *list, size_t index)
{
	DListNode* node = list->first;
	size_t i = 0;

	while( node && i++ < index ) {
		node = node->next;
	}

	if( i < index )
		return NULL;

	return node;
}

void dlist_begin(DList *dlist, DListIterator *dt)
{
	dt->list = dlist;
	dt->node = dlist->first;
	dt->forward = TRUE;
}

void dlist_rbegin(DList *dlist, DListIterator *dt)
{
	dt->list = dlist;
	dt->node = dlist->first;
	dt->forward = FALSE;
}


BOOL dlist_has_more(DListIterator *dt)
{
	return dt->node ? TRUE : FALSE;
}

void* dlist_next(DListIterator *dt)
{
	DListNode *node = dt->node;
	if( dt->forward ) {
		dt->node = dt->node->next;
	} else {
		dt->node = dt->node->prev;
	}
	return node->data;
}


void  stack_init(Stack *stack)
{
	CWT_ASSERT(stack);
	stack->first = NULL;
}

void stack_push(Stack *stack, void *data)
{
	StackNode *node = CWT_MALLOC(StackNode);
	node->data = data;
	list_insert_first(stack, node);
}

void* stack_pop(Stack *stack)
{
	void *data = NULL;

	if( !stack_empty(stack) ) {
		StackNode *node = list_remove_first(stack);

		data = node->data;
		CWT_FREE(node);
	}
	return data;
}

void* stack_peek(Stack *stack)
{
	void *data = NULL;

	if( !stack_empty(stack) ) {
		StackNode *node = stack->first;
		data = node->data;
	}
	return data;
}

BOOL stack_empty(Stack *stack)
{
	return stack->first == NULL ? TRUE : FALSE;
}

void stack_begin(Stack *stack, StackIterator *si)
{
	si->stack = stack;
	si->node = stack->first;
}

BOOL stack_has_more(StackIterator *si)
{
	return si->node ? TRUE : FALSE;
}

void *stack_next(StackIterator *si)
{
	StackNode *node = si->node;
	si->node = si->node->next;
	return node->data;
}

