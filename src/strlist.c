/*
 * strlist.c
 *
 *  Created on: Feb 16, 2012
 *      Author: wladt
 */


#include <string.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/str.h>

static CwtStrList*     __create       (void);
static void            __init         (CwtStrList *psl);
static void            __destroy      (CwtStrList *psl);
static void            __free         (CwtStrList *psl);
static CwtStrList*     __clone        (CwtStrList *psl);
static void            __clear        (CwtStrList *psl);
static size_t          __size         (CwtStrList *psl);
static void            __insertAfter  (CwtStrList *psl, CwtStrListElem *pelem, const CWT_CHAR *text);
static void            __insertBefore (CwtStrList *psl, CwtStrListElem *pelem, const CWT_CHAR *text);
static void            __insertFirst  (CwtStrList *psl, const CWT_CHAR *s);
static void            __insertLast   (CwtStrList *psl, const CWT_CHAR *s);
static void            __append       (CwtStrList *psl, const CWT_CHAR *s);
static void            __prepend      (CwtStrList *psl, const CWT_CHAR *s);
static void            __remove       (CwtStrList *psl, CwtStrListElem *pelem);
static void            __removeFirst  (CwtStrList *psl);
static void            __removeLast   (CwtStrList *psl);
static CWT_CHAR*       __cat          (CwtStrList *psl);
static CWT_CHAR*       __catDelim     (CwtStrList *psl, const CWT_CHAR *delim);
static void 		   __split        (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delim);
static void 		   __splitAny     (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delims);
static CWT_CHAR*	   __at           (CwtStrList *psl, size_t i);
static void            __begin        (CwtStrList *psl, CwtStrListIterator *iter);
static void            __beginFrom    (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);
static void            __rbegin       (CwtStrList *psl, CwtStrListIterator *iter);
static void            __rbeginFrom   (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);
static BOOL            __hasMore      (CwtStrListIterator *iter);
static CWT_CHAR*       __next         (CwtStrListIterator *iter);
static CwtStrListElem* __elem         (CwtStrListIterator *iter);

static CwtStrListNS __cwtStrListNS = {
	  __create
	, __init
	, __destroy
	, __free
	, __clone
	, __clear
	, __size
	, __insertAfter
	, __insertBefore
	, __insertFirst
	, __insertLast
	, __append
	, __append
	, __prepend
	, __remove
	, __removeFirst
	, __removeLast
	, __cat
	, __catDelim
	, __split
	, __splitAny
	, __at
	, __begin
	, __beginFrom
	, __rbegin
	, __rbeginFrom
	, __hasMore
	, __next
	, __elem
};

DLL_API_EXPORT CwtStrListNS* cwtStrListNS(void)
{
	return &__cwtStrListNS;
}


static CwtStrList* __create(void)
{
	CwtStrList *psl;

	psl = CWT_MALLOC(CwtStrList);
	__init(psl);
	return psl;
}

static void __init(CwtStrList *psl)
{
	CWT_ASSERT(psl);
	psl->strings.first = NULL;
	psl->strings.last = NULL;
	psl->count = 0;
}

static void __destroy(CwtStrList *psl)
{
	CWT_ASSERT(psl);
	__clear(psl);
}


static void __free(CwtStrList *psl)
{
	__destroy(psl);
	CWT_FREE(psl);
}


static int __strlist_dup_data_helper(void *data, void *extra)
{
	DListNode *node = CWT_MALLOC(DListNode);

	if( data )
		node->data = cwtStrNS()->strdup((const CWT_CHAR*)data);
	else
		node->data = NULL;

	cwtDListInsertLast((DList*)extra, node);
	return 0;
}

static CwtStrList* __clone(CwtStrList *psl)
{
	CwtStrList *clone = __create();

	CWT_ASSERT(psl);
	dlist_traverse(&psl->strings, __strlist_dup_data_helper, &clone->strings);
	clone->count = psl->count;
	return clone;
}


static void __clear(CwtStrList *psl)
{
	CWT_ASSERT(psl);
	cwtDListClear(&psl->strings, cwtFree, cwtFree);
	psl->count = 0;

}


static size_t __size(CwtStrList *psl)
{
	CWT_ASSERT(psl);
	return psl->count;
}


static void __insertAfter  (CwtStrList *psl, CwtStrListElem *pelem, const CWT_CHAR *s)
{
	CwtStrListElem *e =  CWT_MALLOC(CwtStrListElem);

	CWT_ASSERT(psl);
	e->data = cwtStrNS()->strdup(s);
	if( !pelem ) {
		dlist_insert_first(&psl->strings, e);
	} else {
		dlist_insert_after(&psl->strings, pelem, e);
	}
	psl->count++;
}


static void __insertBefore (CwtStrList *psl, CwtStrListElem *pelem, const CWT_CHAR *s)
{
	CwtStrListElem *e =  CWT_MALLOC(CwtStrListElem);

	CWT_ASSERT(psl);

	e->data = cwtStrNS()->strdup(s);

	if( !pelem ) {
		dlist_insert_first(&psl->strings, e);
	} else {
		dlist_insert_before(&psl->strings, pelem, e);
	}
	psl->count++;
}


static void __insertFirst(CwtStrList *psl, const CWT_CHAR *s)
{
	CwtStrListElem *e =  CWT_MALLOC(CwtStrListElem);

	CWT_ASSERT(psl);

	e->data = cwtStrNS()->strdup(s);
	dlist_insert_first(&psl->strings, e);
	psl->count++;
}


static void __insertLast(CwtStrList *psl, const CWT_CHAR *s)
{
	CwtStrListElem *e =  CWT_MALLOC(CwtStrListElem);

	CWT_ASSERT(psl);

	e->data = cwtStrNS()->strdup(s);
	dlist_insert_last(&psl->strings, e);
	psl->count++;
}


static void __append(CwtStrList *psl, const CWT_CHAR *s)
{
	__insertLast(psl, s);
}

static void __prepend(CwtStrList *psl, const CWT_CHAR *s)
{
	__insertFirst(psl, s);
}

static void __remove(CwtStrList *psl, CwtStrListElem *pelem)
{
	CWT_ASSERT(psl);
	dlist_remove(&psl->strings, pelem);
	CWT_FREE(pelem->data);
	CWT_FREE(pelem);
}

static void __removeFirst(CwtStrList *psl)
{
	CWT_ASSERT(psl);
	__remove(psl, psl->strings.first);
}

static void __removeLast(CwtStrList *psl)
{
	__remove(psl, psl->strings.last);
}


static CWT_CHAR* __cat(CwtStrList *psl)
{
	CWT_ASSERT(psl);
	return __catDelim(psl, NULL);
}


/**
 * Concatenates strings into one.
 *
 * @param sl string list
 * @return new allocated concatenated string
 */
static CWT_CHAR* __catDelim(CwtStrList *psl, const CWT_CHAR *delim)
{
	CwtStringNS *stringNS = cwtStringNS();
	CwtStrNS *strNS = cwtStrNS();
	CwtString *sbuf;
	CWT_CHAR *str;
	CwtStrListIterator it;

	CWT_ASSERT(psl);

	sbuf = stringNS->create();

	__begin(psl, &it);

	while( __hasMore(&it)) {
		if( delim && stringNS->length(sbuf) > 0 )
			stringNS->append(sbuf, delim);
		stringNS->append(sbuf, (CWT_CHAR*)__next(&it));
	}

	str = strNS->strdup(stringNS->cstr(sbuf));
	stringNS->free(sbuf);

	return str;
}


/**
 * Split string with delimiters into tokens
 *
 * @param sl
 * @param s
 * @param delim
 *
 * if @c str is null or is empty string @c sl is unmodified
 * if @c delim is null or is empty string @c sl appends entire @c str
 *
 */
static void __split(CwtStrList *psl, const CWT_CHAR *str, const CWT_CHAR *delim)
{
	const CWT_CHAR *ptr_begin, *ptr_end, *ptr;
	size_t delim_len = 0;
	CwtStrNS *strNS = cwtStrNS();

	CWT_ASSERT(psl);

	if( !str )
		return;

	if( delim )
		delim_len = strNS->strlen(delim);

	if( delim_len == 0 ) {
		__append(psl, str);
		return;
	}

	ptr = str;
	ptr_begin = ptr;
	ptr_end = ptr + strNS->strlen(str);

	while( ptr < ptr_end ) {
		if( ptr == strNS->strstr(ptr, delim) ) {
			CWT_CHAR *s = strNS->strndup(ptr_begin, (size_t)(ptr-ptr_begin));
			__append(psl, s);
			CWT_FREE(s);
			ptr += delim_len;
			ptr_begin = ptr;
			continue;
		}

		ptr++;
	}

	if( ptr > ptr_begin ) {
		__append(psl, ptr_begin);
	}
}


static void __splitAny(CwtStrList *psl, const CWT_CHAR *str, const CWT_CHAR *delims)
{
	const CWT_CHAR *ptr_begin, *ptr_end, *ptr;
	size_t delims_len = 0;
	size_t i;
	BOOL delim_ok = FALSE;
	CwtStrNS *ns = cwtStrNS();

	CWT_ASSERT(psl);

	if( !str )
		return;

	if( delims )
		delims_len = ns->strlen(delims);

	if( delims_len == 0 ) {
		__append(psl, str);
		return;
	}

	ptr = str;
	ptr_begin = ptr;
	ptr_end = ptr + ns->strlen(str);

	while( ptr < ptr_end ) {
		for( i = 0; i < delims_len; i++ ) {
			if( *ptr == delims[i] ) {
				if( !delim_ok ) {
					CWT_CHAR *s = ns->strndup(ptr_begin, (size_t)(ptr-ptr_begin));
					delim_ok = TRUE;
					__append(psl, s);
					CWT_FREE(s);
				}
				break;
			}
			delim_ok = FALSE;
		}

		ptr++;

		if( delim_ok ) {
			ptr_begin = ptr;
		}
	}

	if( ptr > ptr_begin ) {
		__append(psl, ptr_begin);
	}
}


static CWT_CHAR* __at(CwtStrList *psl, size_t index)
{
	size_t i;
	CwtStrListIterator it;
	CWT_CHAR *str = NULL;

	CWT_ASSERT(psl);

	if( index >= psl->count )
		return (CWT_CHAR*)NULL;

	__begin(psl, &it);
	i = 0;

	while( __hasMore(&it) && i++ <= index ) {
		str = __next(&it);
	}

	return str;
}


static void __begin(CwtStrList *psl, CwtStrListIterator *iter)
{
	CWT_ASSERT(psl);

	iter->sl = psl;
	iter->node = psl->strings.first;
	iter->forward = TRUE;
}

static void __beginFrom(CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter)
{
	CWT_ASSERT(psl);

	iter->sl = psl;
	iter->node = pelem;
	iter->forward = TRUE;
}

static void __rbegin(CwtStrList *psl, CwtStrListIterator *iter)
{
	CWT_ASSERT(psl);

	iter->sl = psl;
	iter->node = psl->strings.last;
	iter->forward = FALSE;
}

static void __rbeginFrom(CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter)
{
	CWT_ASSERT(psl);

	iter->sl = psl;
	iter->node = pelem;
	iter->forward = FALSE;
}


static BOOL __hasMore(CwtStrListIterator *iter)
{
	return iter->node ? TRUE : FALSE;
}

static CWT_CHAR* __next(CwtStrListIterator *iter)
{
	CwtStrListElem *node = iter->node;
	if( iter->forward ) {
		iter->node = iter->node->next;
	} else {
		iter->node = iter->node->prev;
	}
	return (CWT_CHAR*)node->data;
}


/* call this function before strlist_next
 * */
static CwtStrListElem* __elem(CwtStrListIterator *iter)
{
	CWT_ASSERT(iter);
	return iter->node;
}
