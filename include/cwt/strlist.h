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
#include <cwt/str.h>
#include <cwt/list.h>

/* Flags for split-family functions */
#define CWT_TRIM_WHITESPACES      0x0001 /* trim bounding whitespaces */
#define CWT_STRIP_BOUNDING_QUOTES 0x0002 /* strip bounding quotes */


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
                                   , void *delim, const CwtQuotePair *qpairs, UINT flags);
	int 		    (*split)       (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delim, const CwtQuotePair *qpairs, UINT flags);
	int 		    (*splitAny)    (CwtStrList *psl, const CWT_CHAR *s, const CWT_CHAR *delims, const CwtQuotePair *qpairs, UINT flags);

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
} CwtStrListNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtStrListNS* cwtStrListNS();
EXTERN_C_END;



#endif /* __CWT_STRLIST_H__ */
