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


static CwtList*      __list_create      (size_t sizeofData, void (*data_cleanup)(void*));
static void          __list_init        (CwtList*, size_t sizeofData, void (*data_cleanup)(void*));
static void          __list_free        (CwtList*);
static void          __list_clear       (CwtList*);
static BOOL          __list_isEmpty     (CwtList*);
static void          __list_insert      (CwtListIterator *before, void *data);
static void          __list_prepend     (CwtList *list, void *data);
static void          __list_append      (CwtList *list, void *data);
static void          __list_removeFirst (CwtList *list);
static void          __list_removeLast  (CwtList *list);
static void          __list_traverse    (CwtList *list, int (*callback)(void *data, void *extra), void *extra);
static void          __list_rtraverse   (CwtList*, int (*callback)(void *data, void *extra), void *extra);
static BOOL          __list_find        (CwtList *list, void *data, CwtListIterator *it);

static void*         __list_first       (CwtList*);
static void*         __list_last        (CwtList*);

static void          __list_begin       (CwtList *list, CwtListIterator *it);
static void          __list_rbegin      (CwtList *list, CwtListIterator *it);
static BOOL          __list_hasMore     (CwtListIterator *it);
static void*         __list_next        (CwtListIterator *it);



static CwtListNS __cwtListNS = {
	  __list_create
	, __list_init
	, __list_free
	, __list_clear
	, __list_isEmpty
	, __list_insert
	, __list_prepend
	, __list_append
	, __list_removeFirst
	, __list_removeLast
	, __list_traverse
	, __list_rtraverse
	, __list_find

	, __list_first
	, __list_last

	, __list_begin
	, __list_rbegin
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

static CwtStrNS *__strNS = NULL;

/**
 *
 */
DLL_API_EXPORT CwtListNS* cwtListNS(void)
{
	if( !__strNS ) {
		__strNS = cwtStrNS();
	}
	return &__cwtListNS;
}

/**
 *
 */
DLL_API_EXPORT CwtStackNS* cwtStackNS(void)
{
	return &__cwtStackNS;
}


/**
 *
 * @param sizeofData
 * @param data_cleanup
 * @return
 */
static CwtList* __list_create(size_t sizeofData, void (*data_cleanup)(void*))
{
	CwtList *list = CWT_MALLOC(CwtList);
	__list_init(list, sizeofData, data_cleanup);
	return list;
}


/**
 *
 * @param list
 * @param sizeofData
 * @param data_cleanup
 */
static void __list_init(CwtList *list, size_t sizeofData, void (*data_cleanup)(void*))
{
	CWT_ASSERT(list);
	cwtStrNS()->bzero(list, sizeof(CwtList));
	list->datasz = sizeofData;
	list->data_cleanup = data_cleanup;
}


/**
 *
 * @param list
 */
static void __list_free(CwtList *list)
{
	if( list ) {
		__list_clear(list);
		CWT_FREE(list);
	}
}

/**
 *
 * @param list
 */
static void __list_clear(CwtList *list)
{
	CwtListNode *node = list->f;

	while( node ) {
		if( node->d && list->data_cleanup)
			list->data_cleanup(node->d);

		list->f = node->n;
		CWT_FREE(node);
	    node = list->f;
	}

	list->f = NULL;
	list->l = NULL;
	list->count = 0;
}


static BOOL __list_isEmpty(CwtList *list)
{
	return list->count > 0 ? TRUE : FALSE;
}

/**
 * @brief Inserts @c data in front of the item pointed to by the iterator @c before.
 * @param before Iterator pointed to the item before that @c data will be inserted.
 * @param data Value will be inserted.
 */
static void __list_insert (CwtListIterator *before, void *data)
{
	CwtListNode *n;
	CwtListNode *bnode;
	CwtListNode *anode;
	CwtList     *list;

	list = before->list;

	n = (cwtMalloc*)(sizeof(CwtListNode) * 2 + list->datasz);
	__strNS->memcpy(n->d, data, list->datasz);

	bnode = before->node;
	anode = before->node->n;

	n->p = bnode;
	n->n = anode;
	bnode->n = n;

	if( list->l == bnode ) { /* before points to the last node */
		list->l = n;
	} else {
		anode->p = n;
	}

	list->count++;
}


/**
 * @brief Inserts @c data at the beginning of the list.
 *
 * @param list
 * @param data
 */
static void __list_prepend(CwtList *list, void *data)
{
	CwtListNode *n;

	n = (cwtMalloc*)(sizeof(CwtListNode) * 2 + list->datasz);
	__strNS->memcpy(n->d, data, list->datasz);

	if( list->f ) {
		n->n = list->f;
	}

	list->f = n;

	if( !list->l ) {
		list->l = n;
	}

	list->count++;
}

/**
 * @brief Inserts @ data at the end of the list.
 *
 * @param list
 * @param data
 */
static void __list_append(CwtList *list, void *data)
{
	CwtListNode *n;

	n = (cwtMalloc*)(sizeof(CwtListNode) * 2 + list->datasz);
	__strNS->memcpy(n->d, data, list->datasz);

	if( list->l ) {
		n->p = list->l;
	}

	list->l = n;

	if( !list->f ) {
		list->f = n;
	}

	list->count++;

}

/**
 *
 * @param list
 */
static void __list_removeFirst(CwtList *list)
{
	CwtListNode *n;

	/* List is empty */
	if( !list->f )
		return;

	n = list->f;
	list->f = n->n;

	if( list->l == n )
		list->l = NULL;

	if( n->d && list->data_cleanup)
		list->data_cleanup(n->d);

	CWT_FREE(n);

	list->count--;
}


/**
 *
 * @param list
 */
static void __list_removeLast(CwtList *list)
{
	CwtListNode *n;

	/* List is empty */
	if( !list->l )
		return;

	n = list->l;
	list->l = n->p;

	if( list->f == n )
		list->f = NULL;

	if( n->d && list->data_cleanup)
		list->data_cleanup(n->d);

	CWT_FREE(n);

	list->count--;
}


/**
 *
 * @param list
 * @param callback
 * @param extra
 */
static void __list_traverse(CwtList *list, int (*callback)(void *data, void *extra), void *extra)
{
	CwtListNode *node;

	node = list->f;

	while( node ) {
	    if( callback(node->d, extra) )
	    	break;
	    node = node->n;
	}
}


/**
 *
 * @param list
 * @param callback
 * @param extra
 */
static void __list_rtraverse(CwtList *list, int (*callback)(void *data, void *extra), void *extra)
{
	CwtListNode *node;

	node = list->l;

	while( node ) {
	    if( callback(node->d, extra) )
	    	break;
	    node = node->p;
	}
}


static void* __list_first(CwtList *list)
{
	if( !list->f )
		return NULL;

	return list->f->d;
}

static void* __list_last(CwtList *list)
{
	if( !list->l )
		return NULL;

	return list->l->d;
}

/**
 *
 * @param list
 * @param data
 * @param it
 * @return
 */
static BOOL __list_find(CwtList *list, void *data, CwtListIterator *it)
{
	CwtListNode *node;

	node = list->f;

	while( node ) {
	    if( __strNS->memcmp(data, node->d, list->datasz) == 0 ) {
	    	it->forward = TRUE;
	    	it->list = list;
	    	it->node = node;
	    	return TRUE;
	    }
	    node = node->n;
	}
	return FALSE;
}

/**
 *
 * @param list
 * @param it
 */
static void __list_begin(CwtList *list, CwtListIterator *it)
{
	it->list = list;
	it->node = list->f;
	it->forward = TRUE;
}

/**
 *
 * @param list
 * @param it
 */
static void __list_rbegin(CwtList *list, CwtListIterator *it)
{
	it->list = list;
	it->node = list->l;
	it->forward = FALSE;
}

/**
 *
 * @param it
 * @return
 */
static BOOL __list_hasMore(CwtListIterator *it)
{
	return it->node ? TRUE : FALSE;
}

/**
 *
 * @param it
 */
static void* __list_next(CwtListIterator *it)
{
	CwtListNode *node = it->node;

	if( it->forward ) {
		it->node = it->node->n;
	} else {
		it->node = it->node->p;
	}
	return node->d;
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
		CwtStackNode *node = stack->f;
		data = node->data;
	}
	return data;
}

static BOOL __stack_isEmpty(CwtStack *stack)
{
	CWT_ASSERT(stack);
	return stack->f == NULL ? TRUE : FALSE;
}

static void __stack_begin(CwtStack *stack, CwtStackIterator *it)
{
	CWT_ASSERT(stack);
	it->stack = stack;
	it->node = stack->f;
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

