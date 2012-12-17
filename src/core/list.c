/*
 * list.c
 *
 *  Created on: 09.12.2011
 *      Author: wladt
 */



#include <cwt/list.h>
#include <cwt/str.h>

static void          list_init         (CwtList*, size_t sizeofData, void (*data_cleanup)(void*));
static void          list_init_ptr     (CwtList*, void (*data_cleanup)(void*));
static CwtList*      list_create       (size_t sizeofData, void (*data_cleanup)(void*));
static CwtList*      list_create_ptr   (void (*data_cleanup)(void*));
static void          list_destroy      (CwtList*);
static void          list_free         (CwtList*);
static void          list_clear        (CwtList*);
static size_t        list_size         (CwtList*);
static BOOL          list_is_empty     (CwtList*);
static void          list_insert       (CwtListIterator *before, void *data);
static void          list_prepend      (CwtList *list, void *data);
static void          list_append       (CwtList *list, void *data);
static void          list_remove       (CwtListIterator *before);
static void          list_remove_first (CwtList *list);
static void          list_remove_last  (CwtList *list);
static void          list_traverse     (CwtList *list, int (*callback)(void *data, void *extra), void *extra);
static void          list_rtraverse    (CwtList*, int (*callback)(void *data, void *extra), void *extra);
static BOOL          list_find         (CwtListIterator *it, const void *data);

static void*	     list_at           (CwtList*, size_t index);
static void*         list_first        (CwtList*);
static void*         list_last         (CwtList*);

static void          list_begin        (CwtList *list, CwtListIterator *it);
static void          list_rbegin       (CwtList *list, CwtListIterator *it);
static BOOL          list_has_more     (CwtListIterator *it);
static void*         list_next         (CwtListIterator *it);
static void*         list_data         (CwtListIterator *it);

static CwtListNS __cwt_list_ns = {
	  list_init
	, list_init_ptr
	, list_create
	, list_create_ptr
	, list_destroy
	, list_free
	, list_clear
	, list_size
	, list_is_empty
	, list_insert
	, list_prepend
	, list_append
	, list_remove
	, list_remove_first
	, list_remove_last
	, list_traverse
	, list_rtraverse
	, list_find
	, list_at

	, list_first
	, list_last

	, list_begin
	, list_rbegin
	, list_has_more
	, list_next
	, list_data
};


static CwtStrNS *__str_ns = NULL;

/**
 *
 */
DLL_API_EXPORT CwtListNS* cwt_list_ns(void)
{
	if( !__str_ns ) {
		__str_ns = cwt_str_ns();
	}
	return &__cwt_list_ns;
}

/**
 *
 * @param list
 * @param sizeofData
 * @param data_cleanup
 */
static void list_init(CwtList *list, size_t sizeofData, void (*data_cleanup)(void*))
{
	CWT_ASSERT(list);
	cwt_bzero(list, sizeof(CwtList));
	list->datasz = sizeofData;
	list->data_cleanup = data_cleanup;
}

static void list_init_ptr (CwtList *list, void (*data_cleanup)(void*))
{
	list_init(list, sizeof(void*), data_cleanup);
}

/**
 *
 * @param sizeofData
 * @param data_cleanup
 * @return
 */
static CwtList* list_create(size_t sizeofData, void (*data_cleanup)(void*))
{
	CwtList *list = CWT_MALLOC(CwtList);
	list_init(list, sizeofData, data_cleanup);
	return list;
}

static CwtList* list_create_ptr (void (*data_cleanup)(void*))
{
	CwtList *list = CWT_MALLOC(CwtList);
	list_init(list, sizeof(void*), data_cleanup);
	list->is_ptr = TRUE;
	return list;
}


static void list_destroy (CwtList *list)
{
	CWT_ASSERT(list);
	list_clear(list);
}
/**
 *
 * @param list
 */
static void list_free(CwtList *list)
{
	if( list ) {
		list_clear(list);
		CWT_FREE(list);
	}
}

/**
 *
 * @param list
 */
static void list_clear(CwtList *list)
{
	CwtListNode *node = list->f;

	while( node ) {
		if( node->d && list->data_cleanup) {
			void *p = list->is_ptr ? *(void**)node->d : node->d;
			list->data_cleanup(p);
		}

		list->f = node->n;
		CWT_FREE(node);
	    node = list->f;
	}

	list->f = NULL;
	list->l = NULL;
	list->count = 0;
}


static inline size_t list_size(CwtList *list)
{
	return list->count;
}

static inline BOOL list_is_empty(CwtList *list)
{
	return list->count > 0 ? TRUE : FALSE;
}

/**
 * @brief Inserts @c data in front of the item pointed to by the iterator @c before.
 * @param before Iterator pointed to the item before that @c data will be inserted.
 * @param data Value will be inserted.
 */
static void list_insert (CwtListIterator *before, void *data)
{
	CwtListNode *n;
	CwtListNode *bnode;
	CwtListNode *anode;
	CwtList     *list;

	list = before->list;

	n = (CwtListNode*)cwt_malloc(sizeof(CwtListNode*) * 2 + list->datasz);

	if( list->is_ptr )
		__str_ns->memcpy(n->d, &data, list->datasz);
	else
		__str_ns->memcpy(n->d, data, list->datasz);

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
static void list_prepend(CwtList *list, void *data)
{
	CwtListNode *n;

	n = (CwtListNode*)cwt_malloc(sizeof(CwtListNode*) * 2 + list->datasz);
	if( list->is_ptr )
		__str_ns->memcpy(n->d, &data, list->datasz);
	else
		__str_ns->memcpy(n->d, data, list->datasz);

	n->n = NULL;
	n->p = NULL;

	if( list->f ) {
		n->n = list->f;
		list->f->p = n;
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
static void list_append(CwtList *list, void *data)
{
	CwtListNode *n;

	n = (CwtListNode*)cwt_malloc(sizeof(CwtListNode*) * 2 + list->datasz);
	__str_ns->memcpy(n->d, list->is_ptr ? &data : data, list->datasz);

	n->n = NULL;
	n->p = NULL;

	if( list->l ) {
		n->p = list->l;
		list->l->n = n;
	}

	list->l = n;

	if( !list->f ) {
		list->f = n;
	}

	list->count++;

}


static void list_remove(CwtListIterator *it)
{
	CwtList     *list;
	CwtListNode *n;
	CwtListNode *nbefore;
	CwtListNode *nafter;

	list = it->list;
	n = it->node;

	if( !n )
		return;

	nbefore = n->p;
	nafter  = n->n;

	if( nbefore )
		nbefore->n = nafter;

	if( nafter )
		nafter->p = nbefore;

	if( list->f == n )
		list->f = nafter;

	if( list->l == n )
		list->l = nbefore;

	if( n->d && list->data_cleanup) {
		list->data_cleanup(list->is_ptr ? *(void**)n->d : n->d);
	}

	CWT_FREE(n);
	CWT_ASSERT(list->count > 0);
	list->count--;
}

/**
 *
 * @param list
 */
static void list_remove_first(CwtList *list)
{
	CwtListNode *n;

	/* List is empty */
	if( !list->f )
		return;

	n = list->f;
	list->f = n->n;

	if( list->l == n )
		list->l = NULL;

	if( n->d && list->data_cleanup) {
		list->data_cleanup(list->is_ptr ? *(void**)n->d : n->d);
	}

	CWT_FREE(n);
	CWT_ASSERT(list->count > 0);

	list->count--;
}


/**
 *
 * @param list
 */
static void list_remove_last(CwtList *list)
{
	CwtListNode *n;

	/* List is empty */
	if( !list->l )
		return;

	n = list->l;
	list->l = n->p;

	if( list->f == n )
		list->f = NULL;

	if( n->d && list->data_cleanup) {
		list->data_cleanup(list->is_ptr ? *(void**)n->d : n->d);
	}

	CWT_FREE(n);
	CWT_ASSERT(list->count > 0);

	list->count--;
}


/**
 *
 * @param list
 * @param callback
 * @param extra
 */
static void list_traverse(CwtList *list, int (*callback)(void *data, void *extra), void *extra)
{
	CwtListNode *node;

	node = list->f;

	while( node ) {
		if( callback( list->is_ptr ? *(void**)node->d : node->d, extra) )
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
static void list_rtraverse(CwtList *list, int (*callback)(void *data, void *extra), void *extra)
{
	CwtListNode *node;

	node = list->l;

	while( node ) {
		if( callback( list->is_ptr ? *(void**)node->d : node->d, extra) )
			break;
	    node = node->p;
	}
}


static void* list_first(CwtList *list)
{
	if( !list->f )
		return NULL;

	return list->is_ptr ? *(void**)list->f->d : list->f->d;
}

static void* list_last(CwtList *list)
{
	if( !list->l )
		return NULL;

	return list->is_ptr ? *(void**)list->l->d : list->l->d;
}

/**
 *
 * @param list
 * @param data
 * @param it List iterator, may be NULL.
 * @return
 */
static BOOL list_find(CwtListIterator *it, const void *data)
{
	CwtList *list;
	CwtListNode *node;

	CWT_ASSERT(it);

	list = it->list;
	node = it->node;

	while( node ) {

	    if( list->is_ptr
	    		? *(const void**)node->d == data
	    		: __str_ns->memcmp(data, node->d, list->datasz) == 0 ) {

			it->forward = TRUE;
			it->list = list;
			it->node = node;

			return TRUE;
	    }
	    node = node->n;
	}
	return FALSE;
}


static void* list_at(CwtList *list, size_t index)
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

	return list->is_ptr ? *(void**)node->d : node->d;
}

/**
 *
 * @param list
 * @param it
 */
static void list_begin(CwtList *list, CwtListIterator *it)
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
static void list_rbegin(CwtList *list, CwtListIterator *it)
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
static BOOL list_has_more(CwtListIterator *it)
{
	return it->node ? TRUE : FALSE;
}

/**
 *
 * @param it
 */
static void* list_next(CwtListIterator *it)
{
	CwtListNode *node = it->node;

	if( it->forward ) {
		it->node = it->node->n;
	} else {
		it->node = it->node->p;
	}

	return it->list->is_ptr ? *(void**)node->d : node->d;
}

static void* list_data (CwtListIterator *it)
{
	if( it ) {
		return it->list->is_ptr ? *(void**)it->node->d : it->node->d;
	}

	return NULL;
}

