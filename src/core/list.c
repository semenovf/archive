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


static CwtList*      __list_create      (void);
static void          __list_registerCleanup (CwtList *list, void (*free_node)(void*), void (*free_data)(void*));
static void          __list_init        (CwtList*);
static void          __list_free        (CwtList*);
static void          __list_insertAfter (CwtListNode *node, CwtListNode *new_node);
static void          __list_insertFirst (CwtList *list, CwtListNode *new_node);
static CwtListNode*  __list_removeFirst (CwtList *list);
static CwtListNode*  __list_removeAfter (CwtListNode *node);
static void          __list_traverse    (CwtList *list, int (*callback)(void *data, void *extra), void *extra);
static CwtListNode*  __list_find        (CwtList *list, void *data);
static void          __list_clear       (CwtList *list, void (*free_node)(void*), void (*free_data)(void*));
static void          __list_begin       (CwtList *list, CwtListIterator *it);
static BOOL          __list_hasMore     (CwtListIterator *it);
static void*         __list_next        (CwtListIterator *it);

static CwtStack*     __stack_create      (void);
static void          __stack_registerCleanup(CwtStack*, void (*free_data)(void*));
static void          __stack_init        (CwtStack*);
static void          __stack_push        (CwtStack*, void *data);
static void*         __stack_pop         (CwtStack*);
static void*         __stack_peek        (CwtStack*);
static BOOL          __stack_isEmpty     (CwtStack*);
static void          __stack_begin       (CwtStack*, CwtStackIterator *it);
static BOOL          __stack_hasMore     (CwtStackIterator *it);
static void*         __stack_next        (CwtStackIterator *it);


static CwtListNS __cwtListNS = {
	  __list_create
	, __list_registerCleanup
	, __list_init
	, __list_free
	, __list_insertAfter
	, __list_insertFirst
	, __list_removeFirst
	, __list_removeAfter
	, __list_traverse
	, __list_find
	, __list_clear
	, __list_begin
	, __list_hasMore
	, __list_next
};

static CwtStackNS __cwtStackNS = {
	  __stack_create
	, __stack_registerCleanup
	, __stack_init
	, __list_free
	, __list_clear
	, __stack_push
	, __stack_pop
	, __stack_peek
	, __stack_isEmpty
	, __stack_begin
	, __stack_hasMore
	, __stack_next
};


DLL_API_EXPORT CwtListNS* cwtListNS(void)
{
	return &__cwtListNS;
}

DLL_API_EXPORT CwtStackNS* cwtStackNS(void)
{
	return &__cwtStackNS;
}


static CwtList* __list_create(void)
{
	CwtList *list = CWT_MALLOC(CwtList);
	__list_init(list);
	return list;
}

static void __list_registerCleanup (CwtList *list, void (*free_node)(void*), void (*free_data)(void*))
{
	CWT_ASSERT(list);
	list->free_node = free_node;
	list->free_data = free_data;
}

static void __list_init(CwtList *list)
{
	CWT_ASSERT(list);
	list->first = NULL;
	list->free_node = NULL;
	list->free_data = NULL;
}


static void __list_free(CwtList *list)
{
	if( list ) {
		__list_clear(list);
		CWT_FREE(list);
	}
}

static void __list_insertAfter(CwtListNode *node, CwtListNode *new_node)
{
	CWT_ASSERT(node);
	CWT_ASSERT(new_node);
    new_node->next = node->next;
    node->next     = new_node;
}


static void __list_insertFirst(CwtList *list, CwtListNode *new_node)
{
	CWT_ASSERT(list);
	CWT_ASSERT(new_node);
	new_node->next = list->first;
	list->first = new_node;
}


static CwtListNode* __list_removeFirst(CwtList *list)
{
	CwtListNode *obsolete;
	CWT_ASSERT(list);
    obsolete = list->first;

	if( obsolete )
		list->first = list->first->next;
    return obsolete;
}

static CwtListNode* __list_removeAfter(CwtListNode *node)
{
	CwtListNode *obsolete;
	CWT_ASSERT(node);

	obsolete = node->next;

    if( obsolete )
    	node->next = node->next->next;
    return obsolete;
}

static void __list_traverse(CwtList *list, int (*callback)(void *data, void *extra), void *extra)
{
	CwtListNode *node;

	CWT_ASSERT(list);

	node = list->first;

	while( node ) {
	    if( callback(node->data, extra) )
	    	break;
	    node = node->next;
	}
}


static CwtListNode* __list_find(CwtList *list, void *data)
{
	CwtListNode *node;

	CWT_ASSERT(list);

	node = list->first;

	while( node ) {
	    if( node->data == data )
	    	return node;
	    node = node->next;
	}
	return NULL;
}

static void __list_clear(CwtList *list)
{
	CwtListNode *node = list->first;

	while( node ) {
		if( node->data && list->free_data )
			list->free_data(node->data);

		list_remove_first(list);

		if( list->free_node )
			list->free_node(node);

	    node = list->first;
	}

	list->first = NULL;
}


static void __list_begin(CwtList *list, CwtListIterator *it)
{
	CWT_ASSERT(list);
	CWT_ASSERT(it);
	it->list = list;
	it->node = list->first;
}

static BOOL __list_hasMore(CwtListIterator *it)
{
	CWT_ASSERT(it);
	return it->node ? TRUE : FALSE;
}

static void* __list_next(CwtListIterator *it)
{
	CwtListNode *node;
	CWT_ASSERT(it);
	node = it->node;
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




static CwtStack* __stack_create(void)
{
	CwtStack *stack = CWT_MALLOC(CwtStack);
	__stack_init(stack);
	return stack;
}

static void __stack_registerCleanup(CwtStack *stack, void (*free_data)(void*))
{
	CWT_ASSERT(stack);
	stack->free_node = cwtFree;
	stack->free_data = free_data;
}


static void __stack_init(CwtStack *stack)
{
	CWT_ASSERT(stack);
	__list_init(stack);
	stack->free_node = cwtFree;
}


static void __stack_push(CwtStack *stack, void *data)
{
	CwtStackNode *node = CWT_MALLOC(CwtStackNode);
	node->data = data;
	list_insert_first(stack, node);
}



static void* __stack_pop(CwtStack *stack)
{
	void *data = NULL;

	CWT_ASSERT(stack);

	if( !__stack_isEmpty(stack) ) {
		CwtStackNode *node = __list_removeFirst(stack);

		data = node->data;
		CWT_FREE(node);
	}
	return data;
}

static void* __stack_peek(CwtStack *stack)
{
	void *data = NULL;

	CWT_ASSERT(stack);

	if( !__stack_isEmpty(stack) ) {
		CwtStackNode *node = stack->first;
		data = node->data;
	}
	return data;
}

static BOOL __stack_isEmpty(CwtStack *stack)
{
	CWT_ASSERT(stack);
	return stack->first == NULL ? TRUE : FALSE;
}

static void __stack_begin(CwtStack *stack, CwtStackIterator *it)
{
	CWT_ASSERT(stack);
	it->stack = stack;
	it->node = stack->first;
}

static BOOL __stack_hasMore(CwtStackIterator *it)
{
	return it->node ? TRUE : FALSE;
}

static void *__stack_next(CwtStackIterator *it)
{
	CwtStackNode *node = it->node;
	it->node = it->node->next;
	return node->data;
}

