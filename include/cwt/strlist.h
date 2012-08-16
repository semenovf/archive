/*
 * strlist.h
 *
 *  Created on: Feb 16, 2012
 *      Author: wladt
 */

#ifndef __CWT_STRLIST_H__
#define __CWT_STRLIST_H__

#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/list.h>


typedef struct CwtQuotePair {
	CWT_CHAR begin;
	CWT_CHAR end;
} CwtQuotePair;


typedef CwtList CwtStrList;
typedef CwtListIterator CwtStrListIterator;


typedef struct CwtStrListNS {
	CwtStrList*     (*create)      (void);
	void            (*free)        (CwtStrList *psl);
	void            (*clear)       (CwtStrList *psl);
	size_t          (*size)        (CwtStrList *psl);

	void            (*insert)      (CwtStrListIterator *before, const CWT_CHAR *s, size_t n);
	void            (*prepend)     (CwtStrList *psl, const CWT_CHAR *s, size_t n);
	void            (*append)      (CwtStrList *psl, const CWT_CHAR *s, size_t n);
	void            (*add)         (CwtStrList *psl, const CWT_CHAR *s, size_t n);

	void            (*remove)      (CwtStrListIterator *before);
	void            (*removeFirst) (CwtStrList *psl);
	void            (*removeLast)  (CwtStrList *psl);

	CWT_CHAR*       (*cat)         (CwtStrList *psl);
	CWT_CHAR*       (*catDelim)    (CwtStrList *psl, const CWT_CHAR *delim);
	int             (*splitSkip)   (CwtStrList *psl, const CWT_CHAR *str
                                   , size_t (*skip)(const CWT_CHAR *tail, size_t tail_len, void *delim)
                                   , void *delim, const CwtQuotePair *qpairs);
	int 		    (*split)       (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delim, const CwtQuotePair *qpairs);
	int 		    (*splitAny)    (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delims, const CwtQuotePair *qpairs);

	const CWT_CHAR*	(*at)          (CwtStrList *psl, size_t index);
	void            (*begin)       (CwtStrList *psl, CwtStrListIterator *iter);
/*	void            (*beginFrom)   (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);*/
	void            (*rbegin)      (CwtStrList *psl, CwtStrListIterator *iter);
/*	void            (*rbeginFrom)  (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);*/
	BOOL            (*hasMore)     (CwtStrListIterator *iter);
	const CWT_CHAR* (*next)        (CwtStrListIterator *iter);
/*	CwtStrListElem* (*elem)        (CwtStrListIterator *iter);*/
	void            (*toArray)     (CwtStrList*, const CWT_CHAR *argv[], size_t *argc);
	BOOL            (*find)        (CwtStrList*, const CWT_CHAR *s, size_t *index);

	const CwtQuotePair*   (*singleQuotesPair)(void);
	const CwtQuotePair*   (*doubleQuotesPair)(void);
	const CwtQuotePair*   (*quotesPair)(void);
	const CWT_CHAR*       (*whitespaces)(void);
} CwtStrListNS;


#define CWT_QUOTES_SINGLE cwtStrListNS()->singleQuotesPair()
#define CWT_QUOTES_DOUBLE cwtStrListNS()->doubleQuotesPair()
#define CWT_QUOTES_BOTH   cwtStrListNS()->quotesPair()
#define CWT_WHITESPACES   cwtStrListNS()->whitespaces() /* ' ', '\t', '\n', '\v', '\f', '\r' */

/*
#define CWT_QP_SINGLEQUOTES {{_T('\''), _T('\'')}, {0, 0}}
#define CWT_QP_DOUBLEQUOTES {{_T('"'), _T('"')}, {0, 0}}
#define CWT_QP_QUOTES       {{_T('\''), _T('\'')}, {_T('"'), _T('"')}, {0, 0}}
*/

EXTERN_C_BEGIN
DLL_API_EXPORT CwtStrListNS* cwtStrListNS();
EXTERN_C_END;



#endif /* __CWT_STRLIST_H__ */
