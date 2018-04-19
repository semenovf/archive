/*
 * unitype.h
 *
 *  Created on: 02.08.2012
 *      Author: wladt
 */

#ifndef __CWT_UNITYPE_H__
#define __CWT_UNITYPE_H__

#include <cwt/types.h>
#include <cwt/string.h>

typedef struct _CwtUniType {
	CwtTypeEnum type;
	union {
		LONGLONG  llong_val;
		float     float_val;
        double    double_val;
        void     *ptr;
	} value;
    size_t capacity;    /* size of buffer pointed by ptr */
    size_t length;      /* length in chars for CwtType_TEXT, or BLOB size */
} CwtUniType;

typedef struct _CwtUniTypeNS {
	CwtUniType* (*create)        (void);
	void        (*free)          (CwtUniType *ut);
	void        (*init)          (CwtUniType *ut);
	void        (*destroy)       (CwtUniType *ut);
	CwtTypeEnum (*type)          (CwtUniType *ut);
	BOOL        (*eq)            (CwtUniType *ut0, CwtUniType *ut1);
/*	BOOL        (*canCast)       (CwtUniType *ut, CwtTypeEnum type);*/
	BOOL        (*set)           (CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz);
	BOOL        (*setFromString) (CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
	BOOL        (*setBOOL)       (CwtUniType *ut, BOOL b);
	BOOL        (*setCHAR)       (CwtUniType *ut, CWT_CHAR ch);
	BOOL        (*setSBYTE)      (CwtUniType *ut, SBYTE n);
	BOOL        (*setBYTE)       (CwtUniType *ut, BYTE n);
	BOOL        (*setSHORT)      (CwtUniType *ut, SHORT n);
	BOOL        (*setUSHORT)     (CwtUniType *ut, USHORT n);
	BOOL        (*setINT)        (CwtUniType *ut, INT n);
	BOOL        (*setUINT)       (CwtUniType *ut, UINT n);
	BOOL        (*setLONG)       (CwtUniType *ut, LONG n);
	BOOL        (*setULONG)      (CwtUniType *ut, ULONG n);
	BOOL        (*setLONGLONG)   (CwtUniType *ut, LONGLONG n);
	BOOL        (*setULONGLONG)  (CwtUniType *ut, ULONGLONG n);
	BOOL        (*setFLOAT)      (CwtUniType *ut, float n);
	BOOL        (*setDOUBLE)     (CwtUniType *ut, double n);
	BOOL        (*setTEXT)       (CwtUniType *ut, const CWT_CHAR *p, size_t length);
	BOOL        (*setBLOB)       (CwtUniType *ut, const void *p, size_t sz);
	BOOL        (*setTIME)       (CwtUniType *ut, const CWT_TIME *p);
	BOOL        (*setDATE)       (CwtUniType *ut, const CWT_TIME *p);
	BOOL        (*setDATETIME)   (CwtUniType *ut, const CWT_TIME *p);

	BOOL        (*toBOOL)        (CwtUniType *ut, BOOL *ok);
	CWT_CHAR    (*toCHAR)        (CwtUniType *ut, BOOL *ok);
	SBYTE       (*toSBYTE)       (CwtUniType *ut, BOOL *ok);
	BYTE        (*toBYTE)        (CwtUniType *ut, BOOL *ok);
	SHORT       (*toSHORT)       (CwtUniType *ut, BOOL *ok);
	USHORT      (*toUSHORT)      (CwtUniType *ut, BOOL *ok);
	INT         (*toINT)         (CwtUniType *ut, BOOL *ok);
	UINT        (*toUINT)        (CwtUniType *ut, BOOL *ok);
	LONG        (*toLONG)        (CwtUniType *ut, BOOL *ok);
	ULONG       (*toULONG)       (CwtUniType *ut, BOOL *ok);
	LONGLONG    (*toLONGLONG)    (CwtUniType *ut, BOOL *ok);
	ULONGLONG   (*toULONGLONG)   (CwtUniType *ut, BOOL *ok);
	float       (*toFLOAT)       (CwtUniType *ut, BOOL *ok);
	double      (*toDOUBLE)      (CwtUniType *ut, BOOL *ok);
	CWT_CHAR*   (*toTEXT)        (CwtUniType *ut, BOOL *ok);
	const void* (*toBLOB)        (CwtUniType *ut, size_t *sz);
	CWT_TIME*   (*toTIME)        (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
	CWT_TIME*   (*toDATE)        (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
	CWT_TIME*   (*toDATETIME)    (CwtUniType *ut, CWT_TIME *tm, BOOL *ok);
	const CWT_CHAR* (*toString)  (CwtUniType *ut, CwtString *str);
} CwtUniTypeNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtUniTypeNS* cwt_unitype_ns(void);
EXTERN_C_END

#endif /* __CWT_UNITYPE_H__ */
