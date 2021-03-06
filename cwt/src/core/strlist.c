/*
 * strlist.c
 *
 *  Created on: Feb 16, 2012
 *      Author: wladt
 */


#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/str.h>

static void            sl_init       (CwtStrList *psl);
static CwtStrList*     __create      (void);
static void            sl_destroy    (CwtStrList *psl);
static void            __free        (CwtStrList *psl);
/*static CwtStrList*     __clone       (CwtStrList *psl);*/
static void            __clear       (CwtStrList *psl);
static size_t          __size        (CwtStrList *psl);
static void            __insert      (CwtStrListIterator *before, const CWT_CHAR *s);
static void            __prepend     (CwtStrList *psl, const CWT_CHAR *s);
static void            __append      (CwtStrList *psl, const CWT_CHAR *s);
static void            __insert_n    (CwtStrListIterator *before, const CWT_CHAR *s, size_t n);
static void            __prepend_n   (CwtStrList *psl, const CWT_CHAR *s, size_t n);
static void            __append_n    (CwtStrList *psl, const CWT_CHAR *s, size_t n);

static void            __remove      (CwtStrListIterator *before);
static void            __removeFirst (CwtStrList *psl);
static void            __removeLast  (CwtStrList *psl);

static CWT_CHAR*       __cat         (CwtStrList *psl);
static CWT_CHAR*       __catDelim    (CwtStrList *psl, const CWT_CHAR *delim);
static int             __splitSkip   (CwtStrList *psl, const CWT_CHAR *str
		                             , size_t (*skip)(const CWT_CHAR *tail, size_t tail_len, void *delim)
		                             , void *delim, const CwtQuotePair *qpairs, UINT flags);
static int  		   __split       (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delim, const CwtQuotePair *qpairs, UINT flags);
static int  		   __splitAny    (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delims, const CwtQuotePair *qpairs, UINT flags);
static const CWT_CHAR* __at          (CwtStrList *psl, size_t i);
static void            __begin       (CwtStrList *psl, CwtStrListIterator *iter);
/*static void            __beginFrom   (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);*/
static void            __rbegin      (CwtStrList *psl, CwtStrListIterator *iter);
/*static void            __rbeginFrom  (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);*/
static BOOL            __hasMore     (CwtStrListIterator *iter);
static const CWT_CHAR* __next        (CwtStrListIterator *iter);
/*static CwtStrListElem* __elem        (CwtStrListIterator *iter);*/
static void            __toArray     (CwtStrList *psl, CWT_CHAR **argv, size_t *argc);
static BOOL            __find        (CwtStrList *psl, const CWT_CHAR *s, size_t *index);

static CwtStrListNS __cwtStrListNS = {
		sl_init
	, __create
	, sl_destroy
	, __free
/*	, __clone*/
	, __clear
	, __size
	, __insert
	, __prepend
	, __append
	, __append /* add */
	, __insert_n
	, __prepend_n
	, __append_n
	, __append_n /* add_n */
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
	, __find
};

static CwtStrNS  *__str_ns  = NULL;
static CwtListNS *__list_ns = NULL;

DLL_API_EXPORT CwtStrListNS* cwt_strlist_ns(void)
{
	if( !__str_ns ) {
		__str_ns  = cwt_str_ns();
		__list_ns = cwt_list_ns();
	}
	return &__cwtStrListNS;
}


static void sl_init (CwtStrList *psl)
{
	__list_ns->initPtr((CwtList*)psl, cwt_free);
}


static inline CwtStrList* __create(void)
{
	return (CwtStrList*)__list_ns->createPtr(cwt_free);
}

static void sl_destroy (CwtStrList *psl)
{
	__list_ns->destroy((CwtList*)psl);
}

static inline void __free(CwtStrList *psl)
{
	__list_ns->free(psl);
}


static inline void __clear(CwtStrList *psl)
{
	__list_ns->clear(psl);
}


static inline size_t __size(CwtStrList *psl)
{
	return __list_ns->size(psl);
}

static inline void __insert (CwtStrListIterator *before, const CWT_CHAR *str)
{
	CWT_CHAR *s = __str_ns->strDup(str);
	__list_ns->insert(before, s);
}

static inline void __prepend (CwtStrList *psl, const CWT_CHAR *str)
{
	CWT_CHAR *s = __str_ns->strDup(str);
	__list_ns->prepend(psl, s);
}

static inline void __append (CwtStrList *psl, const CWT_CHAR *str)
{
	CWT_CHAR *s = __str_ns->strDup(str);
	__list_ns->append(psl, s);
}


static inline void __insert_n(CwtStrListIterator *before, const CWT_CHAR *str, size_t n)
{
	CWT_CHAR *s = __str_ns->strNdup(str, n);
	__list_ns->insert(before, s);
}

static inline void __prepend_n (CwtStrList *psl, const CWT_CHAR *str, size_t n)
{
	CWT_CHAR *s = __str_ns->strNdup(str, n);
	__list_ns->prepend(psl, s);
}

static inline void __append_n  (CwtStrList *psl, const CWT_CHAR *str, size_t n)
{
	CWT_CHAR *s = __str_ns->strNdup(str, n);
	__list_ns->append(psl, s);
}

static inline void __remove(CwtStrListIterator *before)
{
	__list_ns->remove(before);
}

static void __removeFirst(CwtStrList *psl)
{
	__list_ns->removeFirst(psl);
}

static void __removeLast(CwtStrList *psl)
{
	__list_ns->removeLast(psl);
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
	CwtStringNS *stringNS = cwt_string_ns();
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

	str = __str_ns->strDup(stringNS->cstr(sbuf));
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
 * Helper function for @c splitSkip function
 *
 * @param q
 * @param qpairs
 * @return
 */
static BOOL __is_begin_quote(CWT_CHAR q, const CwtQuotePair *qpairs)
{
	const CwtQuotePair *qpair = qpairs;

	while( qpair->begin != 0 ) {
		if( qpair->begin == q ) {
			return TRUE;
		}
		qpair++;
	}

	return FALSE;
}

/*
static BOOL __is_end_quote(CWT_CHAR q, const CwtQuotePair *qpairs)
{
	const CwtQuotePair *qpair = qpairs;

	while( qpair->end != 0 ) {
		if( qpair->end == q ) {
			return TRUE;
		}
		qpair++;
	}

	return FALSE;
}
*/



/*
 *  Helper function for skip function
 * */
static size_t __skip_string_delim(const CWT_CHAR *tail, size_t tail_len, void *delim)
{
	const CWT_CHAR *delim_str = (const CWT_CHAR*)delim;
	size_t delim_len = __str_ns->strLen(delim_str);

	if( tail[0] == delim_str[0] && tail_len >= delim_len ) {
		if( tail == __str_ns->strStr(tail, delim) ) {
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
	CWT_CHAR *delim_str = (CWT_CHAR*)delim;
	size_t delim_len = __str_ns->strLen(delim_str);
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

/*
 *  Helper function for split function
 * */
static void __append_with_flags(CwtStrList *psl, const CWT_CHAR *str, size_t n, const CwtQuotePair *qpairs, UINT flags)
{
	size_t i = 0;
	size_t j = n - 1;

	if( n > 0 ) {
		if (flags & CWT_TRIM_WHITESPACES) {

			while ((i < n) && __str_ns->isSpace(str[i]))
				i++;

			while ((j > i) && __str_ns->isSpace(str[j]))
				j--;
		}

		if( (flags & CWT_STRIP_BOUNDING_QUOTES) && __is_begin_quote(str[i], qpairs) ) {
			CWT_CHAR q = __find_end_quote(str[i], qpairs);
			CWT_ASSERT(q);
			CWT_ASSERT(q == str[j]);
			j--;
			i++;
		}

		CWT_ASSERT(j + 1 >= i);
		__append_n(psl, &str[i], j - i + 1);
	} else {
		__append(psl, _T(""));
	}
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
		, void *delim, const CwtQuotePair *qpairs, UINT flags)
{
	CwtStrNS *strNS = cwt_str_ns();

	size_t i, ibegin;
	size_t skip_len = 0;
	size_t str_len = 0;
	CWT_CHAR qend = 0; /* end quote */
	BOOL esc = FALSE;

	CWT_ASSERT(psl);

	if( !str )
		return 0;

	str_len = strNS->strLen(str);

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
			__append_with_flags(psl, &str[ibegin], i - ibegin, qpairs, flags);
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
		__append_with_flags(psl, &str[ibegin], i - ibegin, qpairs, flags);
	}

	return (int)__size(psl);
}

static int __split(CwtStrList *psl, const CWT_CHAR *str, const CWT_CHAR *delim, const CwtQuotePair *qpairs, UINT flags)
{
	return __splitSkip(psl, str, __skip_string_delim, (void*)delim, qpairs, flags);
}


static int __splitAny(CwtStrList *psl, const CWT_CHAR *str, const CWT_CHAR *delims, const CwtQuotePair *qpairs, UINT flags)
{
	return __splitSkip(psl, str, __skip_anychar_delim, (void*)delims, qpairs, flags);
}


static inline const CWT_CHAR* __at(CwtStrList *psl, size_t index)
{
	return (CWT_CHAR*)__list_ns->at(psl, index);
}


static inline void __begin(CwtStrList *psl, CwtStrListIterator *it)
{
	__list_ns->begin(psl, it);
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
	__list_ns->rbegin(psl, it);
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
	return __list_ns->hasMore(it);
}

static inline const CWT_CHAR* __next(CwtStrListIterator *it)
{
	return (CWT_CHAR*)__list_ns->next(it);
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
static void __toArray(CwtStrList *psl, CWT_CHAR **argv, size_t *argc)
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
		argv[i] = (CWT_CHAR*)__next(&it);
		i++;
	}

	*argc = i;
}


static BOOL __find(CwtStrList *psl, const CWT_CHAR *s, size_t *index)
{
	size_t i = 0;
	CwtStrListIterator it;

	CWT_ASSERT(psl);

	__begin(psl, &it);

	while( __hasMore(&it) ) {
		if( __str_ns->strEq(s, __next(&it)) ) {
			if( index ) {
				*index = i;
			}
			return TRUE;
		}
		i++;
	}

	return FALSE;
}
