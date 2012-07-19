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
#include <cwt/dlist.h>


typedef struct CwtQuotePair {
	CWT_CHAR begin;
	CWT_CHAR end;
} CwtQuotePair;

typedef struct CwtStrList
{
	size_t count;
	DList strings;
} CwtStrList;

typedef struct DListNode  CwtStrListElem;

typedef struct CwtStrListIterator
{
	CwtStrList *sl;
	CwtStrListElem *node; /* current node */
	BOOL forward;
} CwtStrListIterator;


typedef struct CwtStrListNS {
	CwtStrList*     (*create)      (void);
	void            (*init)        (CwtStrList *psl);
	void            (*destroy)     (CwtStrList *psl);
	void            (*free)        (CwtStrList *psl);
	CwtStrList*     (*clone)       (CwtStrList *psl);
	void            (*clear)       (CwtStrList *psl);
	size_t          (*size)        (CwtStrList *psl);
	void            (*insertAfter) (CwtStrList *psl, CwtStrListElem *pelem, const CWT_CHAR *text, size_t n);
	void            (*insertBefore)(CwtStrList *psl, CwtStrListElem *pelem, const CWT_CHAR *text, size_t n);
	void            (*insertFirst) (CwtStrList *psl, const CWT_CHAR *s, size_t n);
	void            (*insertLast)  (CwtStrList *psl, const CWT_CHAR *s, size_t n);
	void            (*append)      (CwtStrList *psl, const CWT_CHAR *s, size_t n);
	void            (*add)         (CwtStrList *psl, const CWT_CHAR *s, size_t n);
	void            (*prepend)     (CwtStrList *psl, const CWT_CHAR *s, size_t n);
	void            (*remove)      (CwtStrList *psl, CwtStrListElem *pelem);
	void            (*removeFirst) (CwtStrList *psl);
	void            (*removeLast)  (CwtStrList *psl);
	CWT_CHAR*       (*cat)         (CwtStrList *psl);
	CWT_CHAR*       (*catDelim)    (CwtStrList *psl, const CWT_CHAR *delim);
	int             (*splitSkip)   (CwtStrList *psl, const CWT_CHAR *str
                                   , size_t (*skip)(const CWT_CHAR *tail, size_t tail_len, void *delim)
                                   , void *delim, CwtQuotePair *qpairs);
	int 		    (*split)       (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delim, CwtQuotePair *qpairs);
	int 		    (*splitAny)    (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delims, CwtQuotePair *qpairs);
	CWT_CHAR*	    (*at)          (CwtStrList *psl, size_t i);
	void            (*begin)       (CwtStrList *psl, CwtStrListIterator *iter);
	void            (*beginFrom)   (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);
	void            (*rbegin)      (CwtStrList *psl, CwtStrListIterator *iter);
	void            (*rbeginFrom)  (CwtStrList *psl, CwtStrListElem *pelem, CwtStrListIterator *iter);
	BOOL            (*hasMore)     (CwtStrListIterator *iter);
	CWT_CHAR*       (*next)        (CwtStrListIterator *iter);
	CwtStrListElem* (*elem)        (CwtStrListIterator *iter);
} CwtStrListNS;

EXTERN_C_BEGIN
extern CwtQuotePair *CWT_QP_SINGLEQUOTES; /* ' */
extern CwtQuotePair *CWT_QP_DOUBLEQUOTES; /* " */
extern CwtQuotePair *CWT_QP_QUOTES;       /* ", ' */
DLL_API_EXPORT CwtStrListNS* cwtStrListNS();

EXTERN_C_END

#endif /* __CWT_STRLIST_H__ */
