/*
 * list.c
 *
 *  Created on: 09.12.2011
 *      Author: wladt
 */



#include <cwt/list.h>
#include <cwt/str.h>

#ifndef NULL
#	define NULL ((void*)0)
#endif


static CwtList*      __list_create      (size_t sizeofData, void (*data_cleanup)(void*));
static void          __list_free        (CwtList*);
static void          __list_clear       (CwtList*);
static size_t        __list_size        (CwtList*);
static BOOL          __list_isEmpty     (CwtList*);
static void          __list_insert      (CwtListIterator *before, void *data);
static void          __list_prepend     (CwtList *list, void *data);
static void          __list_append      (CwtList *list, void *data);
static void          __list_remove      (CwtListIterator *before);
static void          __list_removeFirst (CwtList *list);
static void          __list_removeLast  (CwtList *list);
static void          __list_traverse    (CwtList *list, int (*callback)(void *data, void *extra), void *extra);
static void          __list_rtraverse   (CwtList*, int (*callback)(void *data, void *extra), void *extra);
static BOOL          __list_find        (CwtList *list, void *data, CwtListIterator *it);

static void*	     __list_at          (CwtList*, size_t index);
static void*         __list_first       (CwtList*);
static void*         __list_last        (CwtList*);

static void          __list_begin       (CwtList *list, CwtListIterator *it);
static void          __list_rbegin      (CwtList *list, CwtListIterator *it);
static BOOL          __list_hasMore     (CwtListIterator *it);
static void*         __list_next        (CwtListIterator *it);
static void*         __list_data        (CwtListIterator *it);

static void          __list_init        (CwtList*, size_t sizeofData, void (*data_cleanup)(void*));

static CwtListNS __cwtListNS = {
	  __list_create
	, __list_free
	, __list_clear
	, __list_size
	, __list_isEmpty
	, __list_insert
	, __list_prepend
	, __list_append
	, __list_remove
	, __list_removeFirst
	, __list_removeLast
	, __list_traverse
	, __list_rtraverse
	, __list_find
	, __list_at

	, __list_first
	, __list_last

	, __list_begin
	, __list_rbegin
	, __list_hasMore
	, __list_next
	, __list_data
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


static inline size_t __list_size(CwtList *list)
{
	return list->count;
}

static inline BOOL __list_isEmpty(CwtList *list)
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

	n = (CwtListNode*)cwtMalloc(sizeof(CwtListNode) * 2 + list->datasz);
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

	n = (CwtListNode*)cwtMalloc(sizeof(CwtListNode) * 2 + list->datasz);
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

	n = (CwtListNode*)cwtMalloc(sizeof(CwtListNode) * 2 + list->datasz);
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


static void __list_remove(CwtListIterator *before)
{
	CwtList     *list;
	CwtListNode *n;
	CwtListNode *nbefore;
	CwtListNode *nafter;

	list = before->list;
	nbefore = before->node;
	n = nbefore->n;

	if( !n )
		return;

	nafter = n->n;
	nbefore->n = nafter;

	if( nafter )
		nafter->p = nbefore;

	if( n->d && list->data_cleanup)
		list->data_cleanup(n->d);

	CWT_FREE(n);
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
 * @param it List iterator, may be NULL.
 * @return
 */
static BOOL __list_find(CwtList *list, void *data, CwtListIterator *it)
{
	CwtListNode *node;

	node = list->f;

	while( node ) {
	    if( __strNS->memcmp(data, node->d, list->datasz) == 0 ) {
	    	if( it ) {
	    		it->forward = TRUE;
	    		it->list = list;
	    		it->node = node;
	    	}
	    	return TRUE;
	    }
	    node = node->n;
	}
	return FALSE;
}


static void* __list_at(CwtList *list, size_t index)
{
	CwtListNode *node;

	if( index >= list->count ) {
		return NULL;
	}

	if( index <= list->count/2 ) {
		node = list->f;
		while( index-- )
			node = node->n;
	} else {
		node = list->l;
		index = list->count - index - 1;
		while( index-- )
			node = node->p;
	}

	return node->d;

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

static void* __list_data (CwtListIterator *it)
{
	if( it ) {
		return it->node->d;
	}

	return NULL;
}

