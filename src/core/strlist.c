/*
 * strlist.c
 *
 *  Created on: Feb 16, 2012
 *      Author: wladt
 */


#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/str.h>

static CwtStrList*     __create       (void);
static void            __free         (CwtStrList *psl);
/*static CwtStrList*     __clone        (CwtStrList *psl);*/
static void            __clear        (CwtStrList *psl);
static size_t          __size         (CwtStrList *psl);
static void            __insert       (CwtStrListIterator *before, const CWT_CHAR *s, size_t n);
static void            __prepend      (CwtStrList *psl, const CWT_CHAR *s, size_t n);
static void            __append       (CwtStrList *psl, const CWT_CHAR *s, size_t n);
static void            __remove       (CwtStrListIterator *before);
static void            __removeFirst  (CwtStrList *psl);
static void            __removeLast   (CwtStrList *psl);

static CWT_CHAR*       __cat          (CwtStrList *psl);
static CWT_CHAR*       __catDelim     (CwtStrList *psl, const CWT_CHAR *delim);
static int             __splitSkip    (CwtStrList *psl, const CWT_CHAR *str
		                              , size_t (*skip)(const CWT_CHAR *tail, size_t tail_len, void *delim)
		                              , void *delim, const CwtQuotePair *qpairs);
static int  		   __split        (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delim, const CwtQuotePair *qpairs);
static int  		   __splitAny     (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delims, const CwtQuotePair *qpairs);
static const CWT_CHAR* __at           (CwtStrList *psl, size_t i);
static void            __begin        (CwtStrList *psl, CwtStrListIterator *iter);
/*static void            __beginFrom    (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);*/
static void            __rbegin       (CwtStrList *psl, CwtStrListIterator *iter);
/*static void            __rbeginFrom   (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);*/
static BOOL            __hasMore      (CwtStrListIterator *iter);
static const CWT_CHAR* __next         (CwtStrListIterator *iter);
/*static CwtStrListElem* __elem         (CwtStrListIterator *iter);*/
static void            __toArray      (CwtStrList *psl, const CWT_CHAR *argv[], size_t *argc);

static const CwtQuotePair*   __singleQuotesPair(void) { static const CwtQuotePair qp[] = {{_T('\''), _T('\'')}, {0, 0}}; return qp; }
static const CwtQuotePair*   __doubleQuotesPair(void) { static const CwtQuotePair qp[] = {{_T('"'), _T('"')}, {0, 0}} ; return qp; }
static const CwtQuotePair*   __quotesPair(void) { static const CwtQuotePair qp[] = {{_T('\''), _T('\'')}, {_T('"'), _T('"')}, {0, 0}} ; return qp; }
static const CWT_CHAR*       __whitespaces(void) { static const CWT_CHAR *ws = _T(" \t\n\v\f\r"); return ws; }

static CwtStrListNS __cwtStrListNS = {
	  __create
	, __free
/*	, __clone*/
	, __clear
	, __size

	, __insert
	, __prepend
	, __append
	, __append /* add */
	, __remove
	, __removeFirst
	, __removeLast

	, __cat
	, __catDelim
	, __splitSkip
	, __split
	, __splitAny
	, __at
	, __begin
/*	, __beginFrom*/
	, __rbegin
/*	, __rbeginFrom*/
	, __hasMore
	, __next
/*	, __elem*/
	, __toArray

	, __singleQuotesPair
	, __doubleQuotesPair
	, __quotesPair
	, __whitespaces
};

static CwtStrNS  *__strNS  = NULL;
static CwtListNS *__listNS = NULL;

DLL_API_EXPORT CwtStrListNS* cwtStrListNS(void)
{
	if( !__strNS ) {
		__strNS  = cwtStrNS();
		__listNS = cwtListNS();
	}
	return &__cwtStrListNS;
}


static inline CwtStrList* __create(void)
{
	return (CwtStrList*)__listNS->createPtr(cwtFree);
}

static inline void __free(CwtStrList *psl)
{
	__listNS->free(psl);
}


/*
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
*/


static inline void __clear(CwtStrList *psl)
{
	__listNS->clear(psl);
}


static inline size_t __size(CwtStrList *psl)
{
	return __listNS->size(psl);
}

static inline void __insert(CwtStrListIterator *before, const CWT_CHAR *str, size_t n)
{
	CWT_CHAR *s = __strNS->strndup(str, n);
	__listNS->insert(before, s);
}


static inline void __prepend(CwtStrList *psl, const CWT_CHAR *str, size_t n)
{
	CWT_CHAR *s = __strNS->strndup(str, n);
	__listNS->prepend(psl, s);
}


static void __append(CwtStrList *psl, const CWT_CHAR *str, size_t n)
{
	CWT_CHAR *s = __strNS->strndup(str, n);
	__listNS->append(psl, s);
}


static inline void __remove(CwtStrListIterator *before)
{
	__listNS->remove(before);
}

static void __removeFirst(CwtStrList *psl)
{
	__listNS->removeFirst(psl);
}

static void __removeLast(CwtStrList *psl)
{
	__listNS->removeLast(psl);
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

	str = __strNS->strdup(stringNS->cstr(sbuf));
	stringNS->free(sbuf);

	return str;
}


/*
 * Helper function for @c splitSkip function
 * @param qbegin
 * @param qpairs
 * @return
 */
static CWT_CHAR __find_end_quote(CWT_CHAR qbegin, const CwtQuotePair *qpairs)
{
	const CwtQuotePair *qpair = qpairs;

	while( qpair->begin != 0 ) {
		if( qpair->begin == qbegin ) {
			return qpair->end;
		}
		qpair++;
	}

	return 0;
}

/*
 *  Helper function for skip function
 * */
static size_t __skip_string_delim(const CWT_CHAR *tail, size_t tail_len, void *delim)
{
	CwtStrNS *strNS = cwtStrNS();
	const CWT_CHAR *delim_str = (const CWT_CHAR*)delim;
	size_t delim_len = strNS->strlen(delim_str);

	if( tail[0] == delim_str[0] && tail_len >= delim_len ) {
		if( tail == strNS->strstr(tail, delim) ) {
			return delim_len;
		}
	}

	return 0;
}

/*
 *  Helper function for splitAny function
 * */
static size_t __skip_anychar_delim(const CWT_CHAR *tail, size_t tail_len, void *delim)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *delim_str = (CWT_CHAR*)delim;
	size_t delim_len = strNS->strlen(delim_str);
	size_t i, j;
	size_t nskip = 0;

	for( j = 0; j < tail_len; j++ ) {
		BOOL found = FALSE;

		for( i = 0; i < delim_len; i++ ) {
			if( tail[j] == delim_str[i] ) {
				found = TRUE;
				nskip++;
				break;
			}
		}

		if( !found )
			break;
	}

	return nskip;
}

/**
 * @brief Splits string with delimiters into tokens
 *
 * @param psl		String list to store tokens
 * @param str       String to split
 * @param skip      Callback defines delimiter, returns number of characters to skip
 * @param qpairs    Array quote character pair. It must be terminated with empty element {0, 0}
 * @return 			0 If str is @c NULL
 * 					> 0 number of tokens
 * 					-1 if string @c str has unbalanced quotes
 */
static int __splitSkip(CwtStrList *psl, const CWT_CHAR *str
		, size_t (*skip)(const CWT_CHAR *tail, size_t tail_len, void *delim)
		, void *delim, const CwtQuotePair *qpairs)
{
	CwtStrNS *strNS = cwtStrNS();

	size_t i, ibegin;
	size_t skip_len = 0;
	size_t str_len = 0;
	CWT_CHAR qend = 0; /* end quote */
	BOOL esc = FALSE;

	CWT_ASSERT(psl);

	if( !str )
		return 0;

	str_len = strNS->strlen(str);

	for( i = 0, ibegin = 0; i < str_len; i++ ) {

		if( esc ) {
			esc = FALSE;
			continue;
		}

		if( str[i] == _T('\\') ) {
			esc = TRUE;
			continue;
		}

		if( qpairs ) {
			/* end quote */
			if( qend && str[i] == qend ) {
				qend = 0;
				continue;
			}

			if( qend )
				continue;

			qend = __find_end_quote(str[i], qpairs);

			if( qend )
				continue;
		}

		/* may be delimiter begins */
		skip_len = skip(&str[i], str_len - i, delim);
		if( skip_len > 0  ) {
			__append(psl, &str[ibegin], i - ibegin);
			i += skip_len;
			ibegin = i;
			i--;
		}
	}

	/* unbalanced quote */
	if( qpairs && qend ) {
		__clear(psl);
		return -1;
	}

	if( i > ibegin ) {
		__append(psl, &str[ibegin], i - ibegin);
	}

	return (int)__size(psl);
}

static int __split(CwtStrList *psl, const CWT_CHAR *str, const CWT_CHAR *delim, const CwtQuotePair *qpairs)
{
	return __splitSkip(psl, str, __skip_string_delim, (void*)delim, qpairs);
}


static int __splitAny(CwtStrList *psl, const CWT_CHAR *str, const CWT_CHAR *delims, const CwtQuotePair *qpairs)
{
	return __splitSkip(psl, str, __skip_anychar_delim, (void*)delims, qpairs);
}


static inline const CWT_CHAR* __at(CwtStrList *psl, size_t index)
{
	return (CWT_CHAR*)__listNS->at(psl, index);
}


static inline void __begin(CwtStrList *psl, CwtStrListIterator *it)
{
	__listNS->begin(psl, it);
}

/*
static void __beginFrom(CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter)
{
	CWT_ASSERT(psl);

	iter->sl = psl;
	iter->node = pelem;
	iter->forward = TRUE;
}
*/

static inline void __rbegin(CwtStrList *psl, CwtStrListIterator *it)
{
	__listNS->rbegin(psl, it);
}

/*
static void __rbeginFrom(CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter)
{
	CWT_ASSERT(psl);

	iter->sl = psl;
	iter->node = pelem;
	iter->forward = FALSE;
}
*/


static inline BOOL __hasMore(CwtStrListIterator *it)
{
	return __listNS->hasMore(it);
}

static inline const CWT_CHAR* __next(CwtStrListIterator *it)
{
	return (CWT_CHAR*)__listNS->next(it);
}


/* call this function before strlist_next
 * */
/*
static CwtStrListElem* __elem(CwtStrListIterator *iter)
{
	CWT_ASSERT(iter);
	return iter->node;
}
*/


/**
 * @fn CwtStrListNS::toArray(CwtStrList *psl, CWT_CHAR *argv[], size_t *argc)
 * @brief Fill in array with strings from string list.
 *
 * @param psl String list.
 * @param argv Array to store strings.
 * @param argc On input contains size of array @c argv. On output contains actual number of stored strings
 *             (less or equal to @c *argc on input).
 */
static void __toArray(CwtStrList *psl, const CWT_CHAR *argv[], size_t *argc)
{
	size_t i;
	CwtStrListIterator it;

	CWT_ASSERT(psl);
	CWT_ASSERT(argc);
	CWT_ASSERT(argv);

	if( *argc == 0 )
		return;

	__begin(psl, &it);
	i = 0;

	while( __hasMore(&it) && i < *argc ) {
		argv[i] = __next(&it);
		i++;
	}

	*argc = i;
}
