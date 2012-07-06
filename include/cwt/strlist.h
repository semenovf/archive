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


typedef struct CwtStringList
{
	size_t count;
	DList strings;
} CwtStringList, *CwtStringListPtr;

typedef struct DListNode  CwtStringListElem;
typedef struct DListNode* CwtStringListElemPtr;

struct CwtStringListIterator
{
	CwtStringList *sl;
	CwtStringListElem *node; /* current node */
	BOOL forward;
};
typedef struct CwtStringListIterator CwtStringListIterator;

EXTERN_C_BEGIN

DLL_API_EXPORT CwtStringListPtr     cwtNewStringList(void);
DLL_API_EXPORT void                 cwtDeleteStringList(CwtStringListPtr sl);
DLL_API_EXPORT void                 cwtStringListClear(CwtStringListPtr sl);
DLL_API_EXPORT CwtStringListPtr     cwtStringListClone(CwtStringListPtr sl);
DLL_API_EXPORT size_t               cwtStringListSize(CwtStringListPtr sl);
DLL_API_EXPORT void                 cwtStringListInsertAfter(CwtStringListPtr sl, CwtStringListElemPtr elem, const CHAR *text);
DLL_API_EXPORT void                 cwtStringListInsertBefore(CwtStringListPtr sl, CwtStringListElemPtr elem, const CHAR *text);
DLL_API_EXPORT void                 cwtStringListInsertFirst(CwtStringListPtr sl, const CHAR *text);
DLL_API_EXPORT void                 cwtStringListInsertLast(CwtStringListPtr sl, const CHAR *text);
DLL_API_EXPORT void                 cwtStringListAppend(CwtStringListPtr sl, const CHAR* str);
#define cwtStringListAdd(sl,str)    cwtStringListAppend(sl, str)
DLL_API_EXPORT void                 cwtStringListPrepend(CwtStringListPtr sl, const CHAR* str);
DLL_API_EXPORT void                 cwtStringListRemove(CwtStringListPtr sl, CwtStringListElemPtr s);
DLL_API_EXPORT void                 cwtStringListRemoveFirst(CwtStringListPtr sl);
DLL_API_EXPORT void                 cwtStringListRemoveLast(CwtStringListPtr sl);
DLL_API_EXPORT CHAR*                cwtStringListCat(CwtStringListPtr sl);
DLL_API_EXPORT CHAR*                cwtStringListCatDelim(CwtStringListPtr sl, const CHAR *delim);
DLL_API_EXPORT void 		        cwtStringListSplit(CwtStringListPtr sl, const CHAR *s, const CHAR *delim);
DLL_API_EXPORT void 		        cwtStringListSplitAny(CwtStringListPtr sl, const CHAR *s, const CHAR *delims);
DLL_API_EXPORT CHAR*		        cwtStringListAt(CwtStringListPtr sl, size_t i);
DLL_API_EXPORT void                 cwtStringListBegin(CwtStringList *sl, CwtStringListIterator *si);
DLL_API_EXPORT void                 cwtStringListBeginFrom(CwtStringList *sl, CwtStringListElemPtr node, CwtStringListIterator *si);
DLL_API_EXPORT void                 cwtStringListRbegin(CwtStringList *sl, CwtStringListIterator *si);
DLL_API_EXPORT void                 cwtStringListRbeginFrom(CwtStringList *sl, CwtStringListElemPtr node, CwtStringListIterator *si);
DLL_API_EXPORT BOOL                 cwtStringListHasMore(CwtStringListIterator *si);
DLL_API_EXPORT CHAR*                cwtStringListNext(CwtStringListIterator *si);
DLL_API_EXPORT CwtStringListElemPtr cwtStringListNode(CwtStringListIterator *si);

EXTERN_C_END

#endif /* __CWT_STRLIST_H__ */
