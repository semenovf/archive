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

/*
struct string_list_t
{
	int count;
	dlist_t strings;
};
*/


CwtStringListPtr cwtNewStringList(void)
{
	CwtStringListPtr sl;

	sl = CWT_MALLOC(CwtStringList);
	CWT_ASSERT(sl);

	sl->strings.first = NULL;
	sl->strings.last = NULL;
	sl->count = 0;
	return sl;
}

void cwtDeleteStringList(CwtStringListPtr sl)
{
	cwtDListClear(&sl->strings, cwtFree, cwtFree);
	CWT_FREE(sl);
}

void cwtStringListClear(CwtStringListPtr sl)
{
	cwtDListClear(&sl->strings, cwtFree, cwtFree);
	sl->count = 0;
}

static int _strlist_dup_data(void *data, void *extra)
{
	DListNode *node = CWT_MALLOC(DListNode);

	if( data )
		node->data = cwtStrNS()->strdup((const CWT_CHAR*)data);
	else
		node->data = NULL;

	cwtDListInsertLast((DList*)extra, node);
	return 0;
}

CwtStringListPtr cwtStringListClone(CwtStringListPtr orig)
{
	CwtStringListPtr clone = cwtNewStringList();
	dlist_traverse(&orig->strings, _strlist_dup_data, &clone->strings);
	clone->count = orig->count;
	return clone;
}

size_t cwtStringListSize(CwtStringListPtr sl)
{
	return sl->count;
}

void cwtStringListInsertAfter(CwtStringListPtr sl, CwtStringListElemPtr elem, const CWT_CHAR *text)
{
	CwtStringListElemPtr e =  CWT_MALLOC(CwtStringListElem);
	e->data = cwtStrNS()->strdup(text);
	if( !elem ) {
		dlist_insert_first(&sl->strings, e);
	} else {
		dlist_insert_after(&sl->strings, elem, e);
	}
	sl->count++;
}

void cwtStringListInsertBefore(CwtStringListPtr sl, CwtStringListElemPtr elem, const CWT_CHAR *text)
{
	CwtStringListElemPtr e =  CWT_MALLOC(CwtStringListElem);
	e->data = cwtStrNS()->strdup(text);

	if( !elem ) {
		dlist_insert_first(&sl->strings, e);
	} else {
		dlist_insert_before(&sl->strings, elem, e);
	}
	sl->count++;
}

void cwtStringListInsertFirst(CwtStringListPtr sl, const CWT_CHAR *text)
{
	CwtStringListElemPtr e =  CWT_MALLOC(CwtStringListElem);
	e->data = cwtStrNS()->strdup(text);
	dlist_insert_first(&sl->strings, e);
	sl->count++;
}

void cwtStringListInsertLast(CwtStringListPtr sl, const CWT_CHAR *text)
{
	CwtStringListElemPtr e =  CWT_MALLOC(CwtStringListElem);
	e->data = cwtStrNS()->strdup(text);
	dlist_insert_last(&sl->strings, e);
	sl->count++;
}


void cwtStringListAppend(CwtStringListPtr sl, const CWT_CHAR* text)
{
	cwtStringListInsertLast(sl, text);
}

void cwtStringListPrepend(CwtStringListPtr sl, const CWT_CHAR* text)
{
	cwtStringListInsertFirst(sl, text);
}

void cwtStringListRemove(CwtStringListPtr sl, CwtStringListElemPtr elem)
{
	dlist_remove(&sl->strings, elem);
	CWT_FREE(elem->data);
	CWT_FREE(elem);
}

void cwtStringListRemoveFirst(CwtStringListPtr sl)
{
	cwtStringListRemove(sl, sl->strings.first);
}

void cwtStringListRemoveLast(CwtStringListPtr sl)
{
	cwtStringListRemove(sl, sl->strings.last);
}


void cwtStringListBegin(CwtStringList *sl, CwtStringListIterator *si)
{
	si->sl = sl;
	si->node = sl->strings.first;
	si->forward = TRUE;
}

void cwtStringListBeginFrom(CwtStringList *sl, CwtStringListElemPtr node, CwtStringListIterator *si)
{
	si->sl = sl;
	si->node = node;
	si->forward = TRUE;
}

void cwtStringListRbegin(CwtStringList *sl, CwtStringListIterator *si)
{
	si->sl = sl;
	si->node = sl->strings.last;
	si->forward = FALSE;
}

void cwtStringListRbeginFrom(CwtStringList *sl, CwtStringListElemPtr node, CwtStringListIterator *si)
{
	si->sl = sl;
	si->node = node;
	si->forward = FALSE;
}


BOOL cwtStringListHasMore(CwtStringListIterator *si)
{
	return si->node ? TRUE : FALSE;
}

CWT_CHAR* cwtStringListNext(CwtStringListIterator *si)
{
	CwtStringListElem *node = si->node;
	if( si->forward ) {
		si->node = si->node->next;
	} else {
		si->node = si->node->prev;
	}
	return (CWT_CHAR*)node->data;
}

/* call this function before strlist_next
 * */
CwtStringListElemPtr cwtStringListNode(CwtStringListIterator *si)
{
	CWT_ASSERT(si);
	return si->node;
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
void cwtStringListSplit(CwtStringListPtr sl, const CWT_CHAR *str, const CWT_CHAR *delim)
{
	const CWT_CHAR *ptr_begin, *ptr_end, *ptr;
	size_t delim_len = 0;
	CwtStrNS *strNS = cwtStrNS();

	if( !str )
		return;

	if( delim )
		delim_len = strNS->strlen(delim);

	if( delim_len == 0 ) {
		cwtStringListAppend(sl, str);
		return;
	}

	ptr = str;
	ptr_begin = ptr;
	ptr_end = ptr + strNS->strlen(str);

	while( ptr < ptr_end ) {
		if( ptr == strNS->strstr(ptr, delim) ) {
			CWT_CHAR *s = strNS->strndup(ptr_begin, (size_t)(ptr-ptr_begin));
			cwtStringListAppend(sl, s);
			CWT_FREE(s);
			ptr += delim_len;
			ptr_begin = ptr;
			continue;
		}

		ptr++;
	}

	if( ptr > ptr_begin ) {
		cwtStringListAppend(sl, ptr_begin);
	}
}

void cwtStringListSplitAny(CwtStringListPtr sl, const CWT_CHAR *str, const CWT_CHAR *delims)
{
	const CWT_CHAR *ptr_begin, *ptr_end, *ptr;
	size_t delims_len = 0;
	size_t i;
	BOOL delim_ok = FALSE;
	CwtStrNS *ns = cwtStrNS();

	if( !str )
		return;

	if( delims )
		delims_len = ns->strlen(delims);

	if( delims_len == 0 ) {
		cwtStringListAppend(sl, str);
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
					cwtStringListAppend(sl, s);
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
		cwtStringListAppend(sl, ptr_begin);
	}
}

/**
 * Concatenates strings into one.
 *
 * @param sl string list
 * @return new allocated concatenated string
 */
CWT_CHAR* cwtStringListCatDelim(CwtStringListPtr sl, const CWT_CHAR *delim)
{
	CwtStringNS *stringNS = cwtStringNS();
	CwtStrNS *strNS = cwtStrNS();
	CwtString *sbuf;
	CWT_CHAR *str;
	CwtStringListIterator it;


	sbuf = stringNS->create();

	cwtStringListBegin(sl, &it);

	while(cwtStringListHasMore(&it)) {
		if( delim && stringNS->length(sbuf) > 0 )
			stringNS->append(sbuf, delim);
		stringNS->append(sbuf, (CWT_CHAR*)cwtStringListNext(&it));
	}

	str = strNS->strdup(stringNS->cstr(sbuf));
	stringNS->free(sbuf);

	return str;
}

CWT_CHAR* cwtStringListCat(CwtStringListPtr sl)
{
	return cwtStringListCatDelim(sl, NULL);
}


CWT_CHAR* cwtStringListAt(CwtStringListPtr sl, size_t index)
{
	size_t i;
	CwtStringListIterator it;
	CWT_CHAR *str = NULL;

	if( index >= sl->count )
		return (CWT_CHAR*)NULL;

	cwtStringListBegin(sl, &it);
	i = 0;

	while( cwtStringListHasMore(&it) && i++ <= index ) {
		str = cwtStringListNext(&it);
	}

	return str;
}
